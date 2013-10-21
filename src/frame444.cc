#include <iostream>
#include <algorithm>
#include <string>
#include <assert.h>

#include "video-format.h"
#include "frame.h"
#include "framergb.h"
#include "frame444.h"
#include "frame422.h"
#include "frame420.h"



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
	case RGB: {
		FrameRGB f(m_uvRows, m_uvRows);
		int r, g, b, y, u, v;
		
		for(int i = 0; i < this->m_uvRows * this->m_uvCols; i++) {
			y = this->y()[i];
			u = this->u()[i];
			v = this->v()[i];

			b = (int)(1.164*(y - 16) + 2.018*(u-128));
			g = (int)(1.164*(y - 16) - 0.813*(u-128) - 0.391*(v-128));
			r = (int)(1.164*(y - 16) + 1.596*(v-128));

			/* clipping to [0 ... 255] */
			if(r < 0) r = 0;
			if(g < 0) g = 0;
			if(b < 0) b = 0;
			if(r > 255) r = 255;
			if(g > 255) g = 255;
			if(b > 255) b = 255;

			f.y()[i] = r;
			f.u()[i] = g;
			f.v()[i] = b;
		}
		break;
	} case YUV_444:
		return Frame(*this);
		break;
	case YUV_422:

		break;
	case YUV_420:

		break;
	}
	return Frame(m_uvRows, m_uvCols);
}