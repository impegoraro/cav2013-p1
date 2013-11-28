#ifndef CAV_HEADERS_H_
#define CAV_HEADERS_H_

constexpr uint VIDEO_MAGIC = 0x56494445;

struct CAVHeader
{
	uint magic;
	uint nCols;
	uint nRows;
	int format;
	char undefined[20];
};

struct GolombCAVHeader
{
	uint magic;
	uint nCols;
	uint nRows;
	int format;
	uint m;
	int predictor;
	int index;
	char undefined[8];
};

struct VideoCAVHeader
{
	uint magic;
	ushort nFrames;
	ushort fps;
};

#endif