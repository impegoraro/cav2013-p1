#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <assert.h>

#include "frame.h"
#include "framergb.h"
#include "frame444.h"
#include "frame422.h"
#include "frame420.h"

#include "video-format.h"

Frame422::Frame422(unsigned int nRows, unsigned int nCols) : Frame(nRows, nCols, nRows, (nCols / 2), YUV_422)
{
}

Frame Frame422::convert(VideoFormat dest)
{
	Frame f(m_uvRows, (m_uvCols * 2));
	int l = 0;

	f.y() = *m_y; // copies the Y buffer as is
	int cols = f.cols();

	for (uint i = 0; i < f.cols() * f.rows(); i+=2) { 
		f.u()[i + 1] = f.u()[i] = u()[i / 2]; 
		f.v()[i + 1] = f.v()[i] = v()[i / 2];
	} 
	
	return f;
}