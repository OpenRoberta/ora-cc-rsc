import asyncio
import subprocess
import sys
import tempfile
import pathlib
from typing import List, Optional, Tuple

def mpy_cross_compile(
        file_name: str,
        file_contents: str,
        optimization_level: Optional[int] = None,
        small_number_bits: Optional[int] = None,
        heap_size: Optional[int] = None,
        extra_args: Optional[List[str]] = None,
) -> Tuple[subprocess.CompletedProcess, Optional[bytes]]:
    """
    Compiles a file using mpy-cross.

    Args:
        file_name: The file name.
        file_contents: The MicroPython source code to compile.
        optimization_level: The optimization level 0 to 3.
        small_number_bits: The number of bits in a MP_SMALL_INT.
        heap_size: The heap size.
        extra_args: Additional args to pass directly.

    Returns:
        The completed process and the raw mpy data if compiling succeeded,
        otherwise ``None``.

    Example::

        proc, mpy = mpy_cross_compile("example.py", "print('hello mpy')")

        # be sure to check the return code for failure
        try:
            proc.check_returncode()
        except subprocess.CalledProcessError:
            # stderr should contain more information about the failure
            print(proc.stderr)
            ...

        ...

    """
    with tempfile.TemporaryDirectory() as tmp_dir:
        tmp_dir = pathlib.Path(tmp_dir)

        with open(tmp_dir / "tmp.py", "w") as in_file:
            in_file.write(file_contents)

        args = ["mpy-cross-v6", in_file.name, "-s", file_name]

        if optimization_level is not None:
            if optimization_level not in range(4):
                raise ValueError("optimization_level must be between 0 and 3")

            args.append(f"-O{optimization_level}")

        if small_number_bits is not None:
            args.append(f"-msmall-int-bits={small_number_bits}")

        if heap_size is not None:
            args += ["-X", f"heapsize={heap_size}"]

        if extra_args:
            args += extra_args

        process = subprocess.run(args, capture_output=True)

        try:
            with open(tmp_dir / "tmp.mpy", "rb") as out_file:
                data = out_file.read()
        except OSError:
            data = None

        return process, data

async def compile_program(program: str):
    """Compiles a Python file with ``mpy-cross``.

    Arguments:
        program:
            script that is to be compiled.

    Returns:
        The compiled script in MPY format.

    """

    proc, mpy = mpy_cross_compile("__main__.py", program)

    if proc.returncode == 0:
        return "success", mpy
    else:
        return "error", proc.stderr.decode("utf-8")



if __name__ == "__main__":
    args = sys.argv[1:]
    #broken program for testing
    program = (
    "from pybricks.hubs import PrimeHub\n" +
    "hub = PrimeHub()\n"
    #uncomment this to check for error message
    #+ "while True"
    )

    if len(args) >= 1:
        program = str(args[0])

    try:
        status, payload = asyncio.run(compile_program(program))
        print(status)
        if status == "success":
            print(*list(payload), sep=",")
        else:
            print(payload)
    except:
        print("error")
        print("mpy-cross internal error")
        pass
