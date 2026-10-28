"""
Checks the Calliope mini V3 hex produced by compile.py (issue OpenRoberta/openroberta-lab#1745).

Run from this directory with: python3 -m unittest test_compile_calliope_v3
"""
import re
import struct
import unittest
import zlib
from pathlib import Path

import compile as compiler

RUNTIME_DIR = Path(__file__).parent / "runtimeHex"
RUNTIME_HEX = RUNTIME_DIR / "calliope-v3-runtime.hex"
MARKER = re.compile("FE307F59.{16}9DD7B1C1")


def records(hex_text):
    for line in hex_text.splitlines():
        if line.startswith(":"):
            raw = bytes.fromhex(line[1:])
            yield raw[3], (raw[1] << 8) | raw[2], raw[4:4 + raw[0]], line


def memory_image(hex_text, linear_only=False):
    """Address -> byte map. With linear_only it behaves like a partial flashing client that ignores type 02."""
    image, base = {}, 0
    for rtype, offset, data, _ in records(hex_text):
        if rtype == 0x04:
            base = int.from_bytes(data, "big") << 16
        elif rtype == 0x02 and not linear_only:
            base = int.from_bytes(data, "big") << 4
        elif rtype in (0x00, 0x0D):
            for i, byte in enumerate(data):
                image[base + offset + i] = byte
    return image


def partial_flashing_info(hex_text):
    """Replays the MicroPython detection of the Calliope apps: region table, DAL hash, filesystem region."""
    image, header = {}, None
    base = 0
    for rtype, offset, data, line in records(hex_text):
        if rtype == 0x04:
            base = int.from_bytes(data, "big") << 16
        elif rtype in (0x00, 0x0D):
            for i, byte in enumerate(data):
                image[base + offset + i] = byte
            if header is None and MARKER.search(line[9:]):
                header = base + offset
    version, table_len, regions, page_log2 = struct.unpack("<HHHH", bytes(image[header + 4 + i] for i in range(8)))
    assert (version, table_len, regions) == (1, 48, 3)
    info = {}
    for n in range(regions):
        entry = bytes(image[header - table_len + 16 * n + i] for i in range(16))
        region_id, hash_type, page, length, pointer = entry[0], entry[1], *struct.unpack("<HII", entry[2:12])
        if region_id == 2:
            assert hash_type == 2
            text, address = bytearray(), pointer
            while image.get(address):
                text.append(image[address])
                address += 1
            info["dal_string"], info["dal_hash"] = bytes(text), zlib.crc32(bytes(text))
        elif region_id == 3:
            info["fs_start"], info["fs_length"] = page << page_log2, length
    info["fs_lines"] = sum(
        1 for rtype, offset, data, line in records(hex_text)
        if rtype in (0x00, 0x0D) and any(b != 0xFF for b in data)
    )
    return info, image


class CalliopeV3HexTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        runtime = RUNTIME_HEX.read_text()
        cls.runtime_image = memory_image(runtime)
        cls.runtime_linear_view = memory_image(runtime, linear_only=True)
        program = "from calliopemini import *\nprint('hello')\n"
        script = compiler.import_external_libraries(RUNTIME_DIR / "pythonLibraries", program)
        cls.hex = compiler.embed_fs_uhex(compiler.normalize_to_linear_addressing(runtime), compiler.bytefunc(script))
        cls.linear_view = memory_image(cls.hex, linear_only=True)

    def test_original_runtime_is_misread_by_linear_only_clients(self):
        # documents the cause of #1745: without normalisation the DAL version string is not at its address
        self.assertNotEqual(self.runtime_linear_view.get(0x5FE60), self.runtime_image[0x5FE60])

    def test_no_segment_address_records(self):
        self.assertNotIn(0x02, {rtype for rtype, _, _, _ in records(self.hex)})

    def test_runtime_memory_image_is_unchanged(self):
        runtime_part = {a: b for a, b in self.linear_view.items() if not 0x6D000 <= a <= 0x72000}
        self.assertEqual(runtime_part, self.runtime_image)

    def test_partial_flashing_info(self):
        info, _ = partial_flashing_info(self.hex)
        self.assertTrue(info["dal_string"].startswith(b"micro:bit v"))
        self.assertEqual(info["dal_hash"], zlib.crc32(info["dal_string"]))
        self.assertEqual((info["fs_start"], info["fs_length"]), (0x6D000, 0x6000))
        self.assertGreater(info["fs_lines"], 0)

    def test_program_is_embedded_in_filesystem(self):
        fs = bytes(self.linear_view.get(a, 0xFF) for a in range(0x6D000, 0x6D000 + 256))
        self.assertIn(b"main.py", fs)
        self.assertIn(b"print('hello')", fs)


if __name__ == "__main__":
    unittest.main()
