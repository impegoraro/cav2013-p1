#include <string.h>
#include <cstdlib>
#include <error.h>
#include <errno.h>

#include "bitstream.h"
/* 
 *@class BitStream to read and write bits on a file.
 *@author Antonio J. R. Neves (an@ua.pt)
 */
 
BitStream::BitStream(const char *fileName, char *m)
{
	fp = fopen(fileName, m);
	pos = 0;
	buffer = 0;
	if(fp == NULL) {
		printf("%s\n", strerror(errno));
		abort();
	}
	if(strcmp(m, "wb") == 0)
	{
		mode = 1;
	}
	else
	{
		mode = 0;
	}
}

BitStream::~BitStream()
{
	if(mode == 1)
	{
		putc(buffer, fp);
	}
	fclose(fp);
}

void BitStream::writeHeader(const std::string& header)
{
	if(mode) {
		fprintf(fp, "%s\n", header.c_str());
	}
}

void BitStream::readHeader(uint& nCols, uint& nRows, PredictorType& predictor, int& index)
{
	if(!mode) {
		int pred;
		fscanf(fp, (char*) "%u %u %d %d\n", &nCols, &nRows, &pred, &index);

		switch(pred) {
			case 1: predictor = LINEAR_PREDICTOR; break;
			case 2: predictor = NONLINEAR_PREDICTOR; break;
			default:
				fprintf(stderr, "Invalid predictor in header\n");
				abort();
		}
	}
}

void BitStream::writeBit(int value)
{
	if(pos == 0)
		pos = 1;

	if(pos == 256)
	{
		putc(buffer, fp);
		pos = 1;
		buffer = 0;
	}

	buffer = buffer | (value * pos);
	pos = pos << 1;
}

int BitStream::readBit()
{
	if(pos == 256 || pos == 0)
	{
		if((buffer = fgetc(fp)) == EOF)
			return EOF;
		
		pos = 1;
	}

	int tmp = (buffer & pos) != 0 ? 1 : 0;
	pos = pos << 1;
	return tmp;
}

void BitStream::writeNBits(int value, int nb)
{ 
	while(nb--)
		writeBit((value >> nb) & 1);
}

int BitStream::readNBits(int nb)
{
	int bits = 0;
	while(nb--)
	{
		int bit = readBit();
		if(bit == EOF)
			return EOF;

		bits = (bits << 1) | bit;
	}

	return bits;
}
