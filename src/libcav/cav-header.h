#ifndef CAV_HEADERS_H_
#define CAV_HEADERS_H_

#include <cstring>

constexpr uint VIDEO_MAGIC = 0x45444956;

struct CAVHeader
{
	CAVHeader()
	{
		std::memset(this, 0, sizeof(struct CAVHeader));
	}
	CAVHeader(uint magic, uint nCols, uint nRows, int format)
	{
		this->magic = magic;
		this->nCols = nCols;
		this->nRows = nRows;
		this->format = format;
		std::memset(&undefined, 0, 30);
	}
	uint magic;
	uint nCols;
	uint nRows;
	int format;
	char undefined[30];
};

struct GolombCAVHeader
{
	GolombCAVHeader()
	{
		std::memset(this, 0, sizeof(struct GolombCAVHeader));
	}
	GolombCAVHeader(uint magic, uint nCols, uint nRows, int format, uint m, int predictor, float quantFactor, int index, bool block = false)
	{
		this->m = m;
		this->predictor = predictor;
		this->quantFactor = quantFactor;
		this->index = index;
		this->magic = magic;
		this->nCols = nCols;
		this->nRows = nRows;
		this->format = format;
		this->block = block;
		std::memset(&undefined, 0, 8);
	}
	uint magic;
	uint nCols;
	uint nRows;
	int format;
	uint m;
	int predictor;
	float quantFactor; 
	int index;
	bool block;
	char undefined[8];
};

struct VideoCAVHeader
{
	uint magic;
	ushort nFrames;
	ushort fps;
};

#endif