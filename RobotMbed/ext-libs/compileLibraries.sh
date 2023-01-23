#!/bin/bash

for d in */ ; do
    ./../compileLibrary.sh "" "${d%/}" "${d%/}" "../libs2017"
    cp -r ${d%/} ../libs2017/
done

cp NEPODefs.h ../libs2017/microbit/inc/
