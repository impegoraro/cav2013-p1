#ifndef FRAME420_H_
#define FRAME420_H_

#include "frame.h"
#include "video-format.h"

class Frame420 : public Frame
{
public:
	Frame420(unsigned int nRows, unsigned int nCols);

	virtual Frame convert(VideoFormat dest);
protected:
};

#endif