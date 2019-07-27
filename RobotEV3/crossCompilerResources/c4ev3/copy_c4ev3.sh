#!/bin/bash

C4EV3_PATH=$1
SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )"


rm -rf $SCRIPTPATH/include/c4ev3
mkdir -p $SCRIPTPATH/include/c4ev3 $SCRIPTPATH/include/c4ev3/ev3_sensors $SCRIPTPATH/include/c4ev3/ev3_inputs

cp $C4EV3_PATH/src/*.h $SCRIPTPATH/include/c4ev3
cp $C4EV3_PATH/src/ev3_sensors/*.h $SCRIPTPATH/include/c4ev3/ev3_sensors
cp $C4EV3_PATH/src/ev3_inputs/*.h $SCRIPTPATH/include/c4ev3/ev3_inputs
cp $C4EV3_PATH/libev3api.a $SCRIPTPATH/lib/libev3api.a