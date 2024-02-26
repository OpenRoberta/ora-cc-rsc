# SPDX-License-Identifier: MIT
# Copyright (c) 2019-2023 The Pybricks Authors

import asyncio
import logging
import os
import sys
from typing import List, Optional

import mpy_cross_v5
import mpy_cross_v6


logger = logging.getLogger(__name__)


async def compile_program(program: str, abi: int, compile_args: Optional[List[str]] = None):
    """Compiles a Python file with ``mpy-cross``.

    Arguments:
        path:
            Path to script that is to be compiled.
        abi:
            Expected MPY ABI version.
        compile_args:
            Extra arguments for ``mpy-cross``.

    Returns:
        The compiled script in MPY format.

    Raises:
        RuntimeError: if there is not a running event loop.
        ValueError if MPY ABI version is not 5 or 6.
        subprocess.CalledProcessError: if executing the ``mpy-cross` tool failed.
    """

    if abi == 5:
        proc, mpy = mpy_cross_v5.mpy_cross_compile("__main__.py", program, no_unicode=True)
    elif abi == 6:
        proc, mpy = mpy_cross_v6.mpy_cross_compile("__main__.py", program)
    else:
        raise ValueError("mpy_version must be 5")
    proc.check_returncode()
    return mpy


if __name__ == "__main__":
    args = sys.argv[1:]
    #broken program for testing
    program = (
    "from pybricks.hubs import PrimeHub\n" +
        "hub = PrimeHub()\n")
    version = 6

    if len(args) >= 1:
        program = str(args[0])

    if len(args) >= 2:
        version = int(args[1])

    try:
        program_mpy = asyncio.run(compile_program(program, 6, None))
        print(*list(program_mpy), sep=",")
    except:
    	pass
