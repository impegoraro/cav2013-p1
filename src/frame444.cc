#include <iostream>
#include <algorithm>
#include <string>
#include <assert.h>

#include "video-format.h"
#include "frame.h"
#include "frame444.h"



Frame444::Frame444(unsigned int nRows, unsigned int nCols) : Frame(nRows, nCols, nRows, nCols, YUV_444)
{
}

/**
 * Converts a Frame to YUV 444
 */
Frame Frame444::convert(VideoFormat dest)
{
	assert(m_uvRows > 0 && m_uvCols > 0);
	
	switch(dest) {
	case YUV_444:
		return Frame(*this);
		break;
	case YUV_422:

		break;
	case YUV_420:

		break;
	}
	return Frame(m_uvRows, m_uvCols);
}