import argparse
import binascii
import struct
import sys
from pathlib import Path
import ast
import os

_MAX_SIZE_V1 = 8188
_SCRIPT_ADDR = 0x3e000

MICROPYTHON_V1_VERSION = '1.0.1'
MICROPYTHON_V2_VERSION = '2.1.2'

#: Filesystem boundaries, this might change with different MicroPython builds.
_MICROBIT_ID_V1 = '9900'
_FS_START_ADDR_V1 = 0x38C00
# UICR value 0x40000 - 0x400 (scratch page) - 0x400 (mag page) = 0x3F800
_FS_END_ADDR_V1 = 0x3F800

_MICROBIT_ID_V2 = '9903'
_FS_START_ADDR_V2 = 0x6D000
# Flash region value 0x73000 - 0x1000 (scratch page) = 0x72000
_FS_END_ADDR_V2 = 0x72000

def compile(python_script, runtime_hex_path, robot_type, path_to_store):
    try:
        hexScript = (Path(runtime_hex_path)/(robot_type + "-runtime.hex")).read_text()
    except Exception as e:
        print("An unexpected error occurred:", e)

    if robot_type == "microbit-v1":
        python_hex = hexlify(python_script)
        micropython_hex = embed_hex(hexScript, python_hex)
    elif robot_type == "microbit-v2":
        micropython_hex = embed_fs_uhex(hexScript, bytefunc(python_script))
    elif robot_type == "calliope-v3":
        lib_path = Path(runtime_hex_path)/"pythonLibraries"
        combined_script = import_external_libraries(lib_path, python_script)
        micropython_hex = embed_fs_uhex(hexScript, bytefunc(combined_script))
    else:
        print("Error in robot type parameter")
        sys.exit(1)
    if not path_to_store:
        print(micropython_hex)
    # path_to_store for later use, storing the hex in the file system like other token based plugins do

def import_external_libraries(lib_path, python_script):
    import_map = extract_imported_imports_and_filenames(python_script)
    for import_statement, filename in import_map.items():
        file_path = get_file_path_from_filename(lib_path, filename)
        if not file_path:
            continue
        python_script = replace_import_by_file_content(
            file_path, python_script, import_statement
        )
    return python_script

def extract_imported_imports_and_filenames(python_script):
    import_map = {}
    tree = ast.parse(python_script)

    for node in ast.walk(tree):
        if isinstance(node, ast.ImportFrom):
            if node.module:
                module_name = node.module.split(".")[0]
                import_statement = f"from {node.module} import {', '.join(name.name for name in node.names)}"
                import_map[import_statement] = module_name

    return import_map

def get_file_path_from_filename(lib_path, filename):
    filename = filename + ".py"
    file_path = os.path.join(lib_path, filename)

    if os.path.isfile(file_path):
        return file_path
    return ""

def replace_import_by_file_content(lib_path, python_script, import_statement):
    file_content = Path(lib_path).read_text()
    updated_script = python_script.replace(import_statement, file_content)
    return updated_script

def bytefunc(raw):
    """
    Compatibility for 2 & 3 str()
    """
    return raw if sys.version_info[0] == 2 else bytes(raw, 'UTF-8')

def pad_hex_string(hex_records_str, alignment=512):
    """
    Adds padding records to a string of Intel Hex records to align the total
    size to the provided alignment value.

    The Universal Hex format needs each section (a section contains the
    micro:bit V1 or V2 data) to be aligned to a 512 byte boundary, as this is
    the common USB block size (or a multiple of this value).

    As a Universal/Intel Hex string only contains ASCII characters, the string
    length must be multiple of 512, and padding records should be added to fit
    this rule.
    """
    padding_needed = len(hex_records_str) % alignment
    if padding_needed:
        # As the padding record data is all "0xFF", the checksum is always 0xF4
        max_data_chars = 32
        max_padding_record = ":{:02x}00000C{}F4\n".format(
            max_data_chars // 2, "F" * max_data_chars
        )
        min_padding_record = ":0000000CF4\n"
        # As there is minimum record length we need to add it to the count
        chars_needed = alignment - (
                (len(hex_records_str) + len(min_padding_record)) % alignment
        )
        # Add as many full padding records as we can fit
        while chars_needed >= len(max_padding_record):
            hex_records_str += max_padding_record
            chars_needed -= len(max_padding_record)
        # Due to the string length of the smallest padding record we might
        #
        if chars_needed > max_data_chars:
            chars_to_fit = chars_needed - (len(min_padding_record) * 2)
            second_to_last_record = ":{:02x}00000C{}F4\n".format(
                chars_to_fit // 2, "F" * chars_to_fit
            )
            hex_records_str += second_to_last_record
            chars_needed -= len(second_to_last_record)
        hex_records_str += ":{:02x}00000C{}F4\n".format(
            chars_needed // 2, "F" * chars_needed
        )
    return hex_records_str

def bytes_to_ihex(addr, data, universal_data_record=False):
    """
    Converts a byte array (of type bytes) into string of Intel Hex records from
    a given address.

    In the Intel Hex format each data record contains only the 2 LSB of the
    address. To set the 2 MSB a Extended Linear Address record is needed first.
    As we don't know where in a Intel Hex file this will be injected, it
    creates a Extended Linear Address record at the top.

    This function can also be used to generate data records for a Universal
    Hex, in that case the micro:bit V1 data is exactly the same as a normal
    Intel Hex, but the V2 data uses a new record type (0x0D) to encode the
    data, so the `universal_data_record` argument is used to select the
    record type.
    """

    def make_record(data):
        checksump = (-(sum(bytearray(data)))) & 0xff
        return ':%s%02X' % (strfunc(binascii.hexlify(data)).upper(), checksump)

    # First create an Extended Linear Address Intel Hex record
    current_ela = (addr >> 16) & 0xffff
    ela_chunk = struct.pack('>BHBH', 0x02, 0x0000, 0x04, current_ela)
    output = [make_record(ela_chunk)]
    # If the data is meant to go into a Universal Hex V2 section, then the
    # record type needs to be 0x0D instead of 0x00 (V1 section still uses 0x00)
    r_type = 0x0D if universal_data_record else 0x00
    # Now create the Intel Hex data records
    for i in range(0, len(data), 16):
        # If we've jumped to the next 0x10000 address we'll need an ELA record
        if ((addr >> 16) & 0xffff) != current_ela:
            current_ela = (addr >> 16) & 0xffff
            ela_chunk = struct.pack('>BHBH', 0x02, 0x0000, 0x04, current_ela)
            output.append(make_record(ela_chunk))
        # Now the data record
        chunk = data[i:min(i + 16, len(data))]
        chunk = struct.pack('>BHB', len(chunk), addr & 0xffff, r_type) + chunk
        output.append(make_record(chunk))
        addr += 16
    return '\n'.join(output)

def script_to_fs(script):
    """
    Convert a Python script (in bytes format) into Intel Hex records, which
    location is configured within the micro:bit MicroPython filesystem and the
    data is encoded in the filesystem format.

    For more info:
     https://github.com/bbcmicrobit/micropython/blob/v1.0.1/source/microbit/filesystem.c
    """
    if not script:
        return ''
    script = bytes(str(script).encode("utf-8"))
    # Convert line endings in case the file was created on Windows.
    script = script.replace(b'\r\n', b'\n')
    script = script.replace(b'\r', b'\n')

    fs_start_address = _FS_START_ADDR_V2
    fs_end_address = _FS_END_ADDR_V2
    universal_data_record = True

    chunk_size = 128       # Filesystem chunks configure in MP to 128 bytes
    chunk_data_size = 126  # 1st & last bytes are the prev/next chunk pointers
    fs_size = fs_end_address - fs_start_address
    # Total file size depends on data and filename length, as uFlash only
    # supports a single file with a known name (main.py) we can calculate it
    main_py_max_size = ((fs_size / chunk_size) * chunk_data_size) - 9
    if len(script) >= main_py_max_size:
        raise ValueError("Python script must be less than {} bytes.".format(
            main_py_max_size
        ))

    # First file chunk opens with:
    # 0xFE - First byte indicates a file start
    # 0x?? - Second byte stores offset where the file ends in the last chunk
    # 0x07 - Third byte is the filename length (7 letters for main.py)
    # Followed by UFT-8 encoded filename (in this case "main.py")
    # Followed by the UFT-8 encoded file data until end of chunk data
    header = b'\xFE\xFF\x07\x6D\x61\x69\x6E\x2E\x70\x79'
    first_chunk_data_size = chunk_size - len(header) - 1
    chunks = []

    # Star generating filesystem chunks
    chunk = header + script[:first_chunk_data_size]
    script = script[first_chunk_data_size:]
    chunks.append(bytearray(chunk + (b'\xff' * (chunk_size - len(chunk)))))
    while len(script):
        # The previous chunk tail points to this one
        chunk_index = len(chunks) + 1
        chunks[-1][-1] = chunk_index
        # This chunk head points to the previous
        chunk = struct.pack('B', chunk_index - 1) + script[:chunk_data_size]
        script = script[chunk_data_size:]
        chunks.append(bytearray(chunk + (b'\xff' * (chunk_size - len(chunk)))))

    # Calculate the end of file offset that goes into the header
    last_chunk_offset = (len(chunk) - 1) % chunk_data_size
    chunks[0][1] = last_chunk_offset
    # Weird edge case: If we have a 0 offset we need a empty chunk at the end
    if last_chunk_offset == 0:
        chunks[-1][-1] = len(chunks) + 1
        chunks.append(bytearray(struct.pack('B', len(chunks)) +
                                (b'\xff' * (chunk_size - 1))))

    # For Python2 compatibility we need to explicitly convert to bytes
    data = b''.join([bytes(c) for c in chunks])
    fs_ihex = bytes_to_ihex(fs_start_address, data, universal_data_record)
    # Add this byte after the fs flash area to configure the scratch page there
    scratch_ihex = bytes_to_ihex(
        fs_end_address, b"\xfd", universal_data_record
    )
    # Remove scratch Extended Linear Address record if we are in the same range
    ela_record_len = 16
    if fs_ihex[:ela_record_len] == scratch_ihex[:ela_record_len]:
        scratch_ihex = scratch_ihex[ela_record_len:]
    return fs_ihex + "\n" + scratch_ihex + "\n"

def embed_fs_uhex(runtime_hex, python_code=None):
    """
    Given a string representing a MicroPython Universal Hex, it will embed a
    Python script encoded into the MicroPython filesystem for each of the
    Universal Hex sections, as the Universal Hex will contain a section for
    micro:bit V1 and a section for micro:bit V2.

    More information about the Universal Hex format:
    https://github.com/microbit-foundation/spec-universal-hex

    Returns a string of the Universal Hex with the embedded filesystem.

    Will raise a ValueError if the Universal Hex doesn't follow the expected
    format.

    If the python_code is missing, it will return the unmodified
    runtime_hex.
    """
    if not python_code:
        return runtime_hex
    python_code = strfunc(python_code)

    # For the V2 section we add the Python code to the filesystem
    fs_hex = script_to_fs(python_code)
    fs_hex = pad_hex_string(fs_hex)
    # In all Sections the fs will be placed at the end of the hex, right
    # before the UICR, this is for compatibility with all DAPLink versions.
    # V2: SoftDevice + MicroPython + regions table + fs + bootloader + UICR
    # V2 can manage the hex out of order.
    # We find the UICR records in the hex file by looking for an Extended
    # Linear Address record with value 0x1000 (:020000041000EA).
    uicr_i = runtime_hex.rfind(':020000041000EA')
    # Now we know where to inject the fs hex block
    return runtime_hex[:uicr_i] + fs_hex + runtime_hex[uicr_i:]


def embed_hex(runtime_hex, python_hex=None):
    """
    Given a string representing the MicroPython runtime hex, will embed a
    string representing a hex encoded Python script into it.

    Returns a string representation of the resulting combination.

    Will raise a ValueError if the runtime_hex is missing.

    If the python_hex is missing, it will return the unmodified runtime_hex.
    """
    if not runtime_hex:
        raise ValueError('MicroPython runtime hex required.')
    if not python_hex:
        return runtime_hex
    py_list = python_hex.split()
    runtime_list = runtime_hex.split()
    embedded_list = []
    # The embedded list should be the original runtime with the Python based
    # hex embedded two lines from the end.
    embedded_list.extend(runtime_list[:-5])
    embedded_list.extend(py_list)
    embedded_list.extend(runtime_list[-5:])
    return '\n'.join(embedded_list) + '\n'

def strfunc(raw):
    """
    Compatibility for 2 & 3 str()
    """
    return str(raw) if sys.version_info[0] == 2 else str(raw, 'utf-8')

def hexlify(script):
    """
    Takes the byte content of a Python script and returns a hex encoded
    version of it.

    Based on the hexlify script in the microbit-micropython repository.
    """
    if not script:
        return ''
    script = bytes(str(script).encode("utf-8"))
    # Convert line endings in case the file was created on Windows.
    script = script.replace(b'\r\n', b'\n')
    script = script.replace(b'\r', b'\n')
    # Add header, pad to multiple of 16 bytes.
    data = b'MP' + struct.pack('<H', len(script)) + script
    # Padding with null bytes in a 2/3 compatible way
    data = data + (b'\x00' * (16 - len(data) % 16))
    if len(data) > _MAX_SIZE_V1:
        # 'MP' = 2 bytes, script length is another 2 bytes.
        raise ValueError("Python script must be less than 8188 bytes.")
    # Convert to .hex format.
    output = [':020000040003F7']  # extended linear address, 0x0003.
    addr = _SCRIPT_ADDR
    for i in range(0, len(data), 16):
        chunk = data[i:min(i + 16, len(data))]
        chunk = struct.pack('>BHB', len(chunk), addr & 0xffff, 0) + chunk
        checksum = (-(sum(bytearray(chunk)))) & 0xff
        hexline = ':%s%02X' % (strfunc(binascii.hexlify(chunk)).upper(),
                               checksum)
        output.append(hexline)
        addr += 16
    return '\n'.join(output)


def main(argv=None):
    """
    Entry point for the command line tool 'compile'.
    It will ensure the required first argument ends in ".py" (the source
    Python script).

    An optional second argument is used to distinguish between micro:bit V1 and V2.

    An optional third argument is used to reference the path to the place where we want to store the generated hex file.

    Exceptions are caught and printed for the user.
    """
    if not argv:
        argv = sys.argv[1:]

    parser = argparse.ArgumentParser(description="something")
    parser.add_argument('source', nargs='?', default=None)
    parser.add_argument('runtimeHex', nargs='?', default=None)
    parser.add_argument('robot', nargs='?', default="microbit-v2")
    parser.add_argument('target', nargs='?', default=None)
    args = parser.parse_args(argv)
    try:
        compile(python_script=args.source, runtime_hex_path=args.runtimeHex, robot_type=args.robot, path_to_store=args.target)
    except Exception as ex:
        error_message = (
            "Error compiling: {ex}"
        )
        sys.exit(1)

if __name__ == '__main__':
    main(sys.argv[1:])