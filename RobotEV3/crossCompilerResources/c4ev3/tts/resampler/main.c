#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>

#include "speex_resampler.h"

#define RIFF_HEADER_SIZE    44
#define RIFF_RIFF_SIG       0x52494646
#define RIFF_WAVE_SIG       0x57415645
#define RIFF_FMT_SIG        0x666d7420
#define RIFF_FMT_PCM        0x0100
#define RIFF_FMT_1CHAN      0x0100
#define RIFF_FMT_8BITS      0x0800
#define RIFF_FMT_16BITS     0x1000
#define RIFF_DATA_SIG       0x64617461

#define INPUT_SAMPLE_RATE   22050
#define INPUT_BITS          RIFF_FMT_16BITS
#define OUTPUT_SAMPLE_RATE  8000
#define OUTPUT_BITS         RIFF_FMT_16BITS
#define RESAMPLING_QUALITY  0 // from 0 to 10

int readAndCopy(int input, int output, void * buffer, int length);
int readAndCopyInt(int input, int output, bool lsb);
int readInt(int input, bool lsb);
void writeInt(int output, int value, bool lsb);
short readAndCopyShort(int input, int output, bool lsb);
short readShort(int fileHandle, bool lsb);
void writeShort(int output, short value, bool lsb);

int main (int argc, char * argv[]) {
    if (argc != 2) {
        printf("Usage: resampler <output file>\n");
        return -1;
    }

	int err;
	SpeexResamplerState * state = speex_resampler_init(1, INPUT_SAMPLE_RATE, OUTPUT_SAMPLE_RATE, RESAMPLING_QUALITY, &err);

	int input = 0; // stdin
	if (input == -1) {
		perror("can't open input file:");
		return -1;
	}

	int output = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (output == -1) {
		perror("can't open output file:");
		return -1;
	}

	if (readAndCopyInt(input, output, false) != RIFF_RIFF_SIG) {
		printf("file check failed\n");
		return -1;
	}
	// Skip chunk size
	readAndCopyInt(input, output, true);
	if (readAndCopyInt(input, output, false) != RIFF_WAVE_SIG) {
		printf("file check failed\n");
		return -1;
	}
	if (readAndCopyInt(input, output, false) != RIFF_FMT_SIG) {
		printf("file check failed\n");
		return -1;
	}

	int size = readAndCopyInt(input, output, true);
	if (readAndCopyShort(input, output, false) != RIFF_FMT_PCM) {
		printf("file check failed\n");
		return -1;
	}
	if (readAndCopyShort(input, output, false) != RIFF_FMT_1CHAN) {
		printf("file check failed\n");
		return -1;
	}

	int sampleRate = readInt(input, true);
	if (sampleRate != 22050) {
		printf("sample rate: %d != 22050\n", sampleRate);
		return -1;
	}
	writeInt(output, 8000, true);

	readAndCopyInt(input, output, true);
	readAndCopyShort(input, output, true);
	int bits = readShort(input, false);
	if (bits != RIFF_FMT_16BITS) {
		printf("not 16 bit\n");
		return -1;
	}
	writeShort(output, RIFF_FMT_8BITS, false);

	// Skip any data in this chunk after the 16 bytes above
	size -= 16;
	uint8_t tmp;
	while (size-- > 0) {
		readAndCopy(input, output, &tmp, 1);
	}

	// Skip optional chunks until we find a data sig (or we hit eof!)
	int dataLength;
	for(;;) {
		int chunk = readAndCopyInt(input, output, false);
		dataLength = readAndCopyInt(input, output, true);
		if (chunk == RIFF_DATA_SIG) {
			break;
		}

		// Skip to the start of the next chunk
		while(dataLength-- > 0) {
			readAndCopy(input, output, &tmp, 1);
		}
	}


	while (dataLength > 0) {
		uint8_t byteBuffer[4096];
		int readBytes = read(input, byteBuffer, 4096);
		dataLength -= readBytes;

		if (readBytes == 0) {
			break;
		}

		// TODO: Handle odd reads

		spx_uint32_t inLength = readBytes / 2;
		spx_int16_t in[2048];

		int i;
		for (i = 0; i < inLength; i++) {
			uint8_t b1 = byteBuffer[i * 2];
			uint8_t b2 = byteBuffer[(i * 2) + 1];
			in[i] = (b1 & 0xff) | ((b2 & 0xff) << 8); // from 2 bytes to short
		}

		spx_uint32_t outLength = 1024;
		spx_int16_t out[1024];

		int res = speex_resampler_process_int(state,
                                 0,
                                 in,
                                 &inLength,
                                 out,
                                 &outLength);

		for (i = 0; i < outLength; i++) {
			short tmp = out[i];
			// 16bit -> 8bit
			byteBuffer[i] = ((tmp + 32767) >> 8) & 0xff;

		}
		write(output, byteBuffer, i);
	}

	close(input);
	close(output);
	return 0;
}

int readAndCopy(int input, int output, void * buffer, int length) {
	int readBytes = read(input, buffer, length);
	write(output, buffer, readBytes);
	return readBytes;
}

int readAndCopyInt(int input, int output, bool lsb){
	uint8_t b1, b2, b3, b4;
	if (lsb) {
		readAndCopy(input, output, &b1, 1);
		readAndCopy(input, output, &b2, 1);
		readAndCopy(input, output, &b3, 1);
		readAndCopy(input, output, &b4, 1);
	} else {
		readAndCopy(input, output, &b4, 1);
		readAndCopy(input, output, &b3, 1);
		readAndCopy(input, output, &b2, 1);
		readAndCopy(input, output, &b1, 1);
	}
	int val = b1 & 0xff;
	val |= (b2 & 0xff) << 8;
	val |= (b3 & 0xff) << 16;
	val |= (b4 & 0xff) << 24;
	return val;
}

int readInt(int input, bool lsb){
	uint8_t b1, b2, b3, b4;
	if (lsb) {
		read(input, &b1, 1);
		read(input, &b2, 1);
		read(input, &b3, 1);
		read(input, &b4, 1);
	} else {
		read(input, &b4, 1);
		read(input, &b3, 1);
		read(input, &b2, 1);
		read(input, &b1, 1);
	}
	int val = b1 & 0xff;
	val |= (b2 & 0xff) << 8;
	val |= (b3 & 0xff) << 16;
	val |= (b4 & 0xff) << 24;
	return val;
}

void writeInt(int output, int value, bool lsb) {
	uint8_t b1, b2, b3, b4;
	b1 = value & 0xff;
	b2 = (value >> 8) & 0xff;
	b3 = (value >> 16) & 0xff;
	b4 = (value >> 24) & 0xff;
	if (lsb) {
		write(output, &b1, 1);
		write(output, &b2, 1);
		write(output, &b3, 1);
		write(output, &b4, 1);
	} else {
		write(output, &b4, 1);
		write(output, &b3, 1);
		write(output, &b2, 1);
		write(output, &b1, 1);
	}
}

short readAndCopyShort(int input, int output, bool lsb) {
	uint8_t b1, b2;
	if (lsb) {
		readAndCopy(input, output, &b1, 1);
		readAndCopy(input, output, &b2, 1);
	} else {
		readAndCopy(input, output, &b2, 1);
		readAndCopy(input, output, &b1, 1);
	}
	short val = b1 & 0xff;
	val |= (b2 & 0xff) << 8;
	return val;
}

short readShort(int fileHandle, bool lsb) {
	uint8_t b1, b2;
	if (lsb) {
		read(fileHandle, &b1, 1);
		read(fileHandle, &b2, 1);
	} else {
		read(fileHandle, &b2, 1);
		read(fileHandle, &b1, 1);
	}
	short val = b1 & 0xff;
	val |= (b2 & 0xff) << 8;
	return val;
}

void writeShort(int output, short value, bool lsb) {
	uint8_t b1, b2;
	b1 = value & 0xff;
	b2 =  (value >> 8) & 0xff;
	if (lsb) {
		write(output, &b1, 1);
		write(output, &b2, 1);
	} else {
		write(output, &b2, 1);
		write(output, &b1, 1);
	}
}
