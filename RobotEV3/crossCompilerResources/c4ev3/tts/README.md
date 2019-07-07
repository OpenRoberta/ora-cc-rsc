# TTS with C4EV3

## Installation

Copy the NEPO-TTS folder in the root of a sd card and insert it in the robot.

## Implementation

The generated NEPO code will call the `synthetize.sh` scripts, which executes espeak and a program to downsample the generated wav file.
Espeak generates a Wav file with a sample rate of 22050Hz 16bit and the resampler downsamples it to 8000Hz 8bit (supported by c4ev3).

## Build
Run `build.sh` to compile the downsampler.
