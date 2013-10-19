#ifndef FRAME422_H_
#define FRAME422_H_

#include "frame.h"
#include "video-format.h"


class Frame422 : public Frame
{
public:
	Frame422(unsigned int nRows, unsigned int nCols);

protected:
	virtual Frame convert(VideoFormat dest);
};

#endif