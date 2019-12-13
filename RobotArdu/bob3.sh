#!/bin/bash

COMP_RESOURCES=$1
PROG_FULL_PATH=$2
TARGET_FULL_PATH=$3
avr-gcc -I${COMP_RESOURCES}/libraries/Bob3/include -I${COMP_RESOURCES}/libraries/RobertaFunctions \
        -D_BOB3_ -g -Os -fno-exceptions -fdata-sections -fno-threadsafe-statics -MMD -ffunction-sections -DAVR \
        -Wall -Wextra -Wpointer-arith -Wcast-qual -Wmissing-include-dirs -Wno-unused-parameter -Wuninitialized \
        -mmcu=atmega88 -DF_CPU=8000000 -finput-charset=UTF-8 -c ${PROG_FULL_PATH}.cpp -o ${TARGET_FULL_PATH}.o
avr-gcc -L${COMP_RESOURCES}/libraries/Bob3/lib -mmcu=atmega88 -Os -Wl,--gc-sections -o ${TARGET_FULL_PATH}.elf ${TARGET_FULL_PATH}.o  -lbob3-smd
avr-objcopy -j .text -j .data -O ihex ${TARGET_FULL_PATH}.elf ${TARGET_FULL_PATH}.hex
rm ${TARGET_FULL_PATH}.elf ${TARGET_FULL_PATH}.o ${TARGET_FULL_PATH}.d
