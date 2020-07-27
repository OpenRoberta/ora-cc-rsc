#!/bin/bash

if [[ "$#" -ne 3 ]]
then
    echo "Wrong number of arguments"
    echo "copy_c4ev3.sh <c4ev3 path> <gcc glibc prefix> <gcc uclibc prefix>"
    echo "example: ./copy_c4ev3.sh ../EV3-API arm-linux-gnueabi- arm-c4ev3-linux-uclibceabi-"
    echo "Read the README file in the folder containing this script for more information."
    exit
fi

C4EV3_PATH="$1"
CROSS_COMPILE_GLIBC="$2"
CROSS_COMPILE_UCLIBC="$3"

SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )"


function clear {
	rm -rf "$SCRIPTPATH/include/c4ev3"
	mkdir -p "$SCRIPTPATH/include/c4ev3" "$SCRIPTPATH/include/c4ev3/ev3_sensors"
}

function safe_cp {
	cp "$@"
	result=$?
	if [[ $result -ne 0 ]]; then
		echo "Error while copying $1 to $2"
		exit $result
	fi
}

function copy_headers {
	safe_cp $C4EV3_PATH/include/*.h "$SCRIPTPATH/include/c4ev3"
	safe_cp $C4EV3_PATH/include/ev3_sensors/*.h "$SCRIPTPATH/include/c4ev3/ev3_sensors"
	# we need the 'private' definition of the 'SensorHandler' struct (included in NEPOLog.h)
	safe_cp $C4EV3_PATH/API/sensors/ev3_sensors.h "$SCRIPTPATH/include/c4ev3/ev3_sensors/ev3_sensors.private.h"
}

function build_static_library {
	cd "$C4EV3_PATH"
	make clean > /dev/null
	CROSS_COMPILE="$1" make
	result=$?
	if [[ $result -ne 0 ]]; then
		echo "Error while compiling c4ev3 using the cross compiler prefix $1. Exit code: $result"
		exit $result
	fi
	cd "$SCRIPTPATH"
}

function copy_static_library {
	CROSS_COMPILE="$1"
	LIB_FOLDER="$2"
	build_static_library "$CROSS_COMPILE"
	mkdir -p "$SCRIPTPATH/$LIB_FOLDER"
	safe_cp "$C4EV3_PATH/libev3api.a" "$SCRIPTPATH/$LIB_FOLDER/libev3api.a"
}

clear
copy_headers
printf "headers copied\n\n"

printf "building glibc c4ev3 static library\n\n"
copy_static_library "$CROSS_COMPILE_GLIBC" "lib"
printf "glibc c4ev3 static library built\n\n"

printf "building uclibc c4ev3 static library (for raspberry)\n\n"
copy_static_library "$CROSS_COMPILE_UCLIBC" "lib-uclibc"
printf "uclibc c4ev3 static library (for raspberry) built\n\n"

echo "c4ev3 is now updated in ora-cc-rsc!"
