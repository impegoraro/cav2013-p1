#ifndef FRAME420_H_
#define FRAME420_H_

#include "frame.h"

class Frame420 : public Frame
{
public:
	Frame420(unsigned int nRows, unsigned int nCols);

protected:
	virtual Frame convert();
};

#endif