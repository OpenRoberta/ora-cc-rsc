#!/bin/sh

LMS_ASSEMBLER=~/Workspaces/lms2012/lmssrc/adk/lmsasm

LMS_SOURCE_NAME=launcher # without extension
LMS_SOURCE_FOLDER="`pwd`"

cd $LMS_ASSEMBLER
java -jar assembler.jar "$LMS_SOURCE_FOLDER/$LMS_SOURCE_NAME"
