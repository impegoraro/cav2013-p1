#ifndef CAV_FRAMERGB_H_
#define CAV_FRAMERGB_H_

#include "frame.h"


class FrameRGB : public Frame
{

public:
	FrameRGB(unsigned int nRows, unsigned int nCols);

	virtual Frame convert(VideoFormat format);
};


#endif