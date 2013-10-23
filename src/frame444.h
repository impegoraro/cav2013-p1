#ifndef FRAME444_H_
#define FRAME444_H_

#include "frame.h"
#include "video-format.h"

class Frame444 : public Frame
{
public:
	Frame444(unsigned int nRows, unsigned int nCols);

	virtual Frame convert(VideoFormat dest);
};

#endif