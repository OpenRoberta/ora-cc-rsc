# TTS with C4EV3

## Installation

Copy the NEPO-TTS folder in the root of a sd card and insert it in the robot.

## Implementation

Easpeak is used to generate a WAV file with a sample rate of 22050Hz 16bit and then c4ev3 will
encode from 16bit to 8bit while playing the file.

## Compile espeak-ng

Download the CodeSourcery GCC compiler [from here](https://sourcery.mentor.com/public/gnu_toolchain/arm-none-linux-gnueabi/arm-2009q3-67-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2).
Note that it has to be the 2009-q3 version (or similar) because:
- the compiler suggested from c4ev3 is too old and it doesn't support some utility functions to convert
 numbers with different endianess (ex: le16toh);
- newer compilers use libc > 2.8 (the version supported by the EV3 firmware);

Clone [espeak-ng](https://github.com/espeak-ng/espeak-ng) and then edit the file `src/libespeak-ng/compiledata.c` replacing the two occurences of `22050` with `8000`.

Then:
```bash

# compile for your platform, sice espeak will be used to compile the sound data
./configure
make

# now crosscompile espeak
# (replace 'arm-2009q3' with the folder where you extracted the compiler)
export CC=arm-2009q3/bin/arm-none-linux-gnueabi-gcc
export CCX=arm-2009q3/bin/arm-none-linux-gnueabi-g++
./configure --host=arm-linux-gnueabi --build=x86_64-linux-gnu --with-pcaudiolib=no --with-sonic=no --prefix=/media/card/NEPO-TTS
make -B src/espeak-ng src/speak-ng
```
