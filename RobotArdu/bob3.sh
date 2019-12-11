#!/bin/env bash
echo $PWD
avr-gcc -I../ora-cc-rsc/RobotArdu/libraries/Bob3/include -I../ora-cc-rsc/RobotArdu/libraries/RobertaFunctions -D_BOB3_ -g -Os -fno-exceptions -fdata-sections -fno-threadsafe-statics -MMD -ffunction-sections -DAVR -Wall -Wextra -Wpointer-arith -Wcast-qual -Wmissing-include-dirs -Wno-unused-parameter -Wuninitialized -mmcu=atmega88 -DF_CPU=8000000 -finput-charset=UTF-8 -c $1.cpp -o $2.o
avr-gcc -L../ora-cc-rsc/RobotArdu/libraries/Bob3/lib -mmcu=atmega88 -Os -Wl,--gc-sections -o $2.elf $2.o  -lbob3-smd
avr-objcopy -j .text -j .data -O ihex $2.elf $2.hex
rm $2.elf $2.o $2.d
