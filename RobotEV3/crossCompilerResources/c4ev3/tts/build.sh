CC=arm-linux-gnueabi-gcc

cd resampler
$CC -D OUTSIDE_SPEEX -D RANDOM_PREFIX=speex_outside -DFLOATING_POINT main.c speex_resampler.c -Os -lm -o ../NEPO-TTS/resampler
