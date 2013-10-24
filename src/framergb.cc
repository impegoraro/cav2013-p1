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


FrameRGB::FrameRGB(unsigned int nRows, unsigned int nCols) : Frame(nRows, nCols, nRows, nCols, RGB)
{
}

Frame FrameRGB::convert(VideoFormat dest)
{
	assert(m_uvRows > 0 && m_uvCols > 0);
	
	switch(dest) {
	case RGB:
		return Frame(*this);
		break;
	case YUV_444: {
		Frame444 f(this->m_uvRows, this->m_uvRows);
		int r, g, b, y, u, v;
		for(uint i = 0; i < this->m_uvRows * this->m_uvCols; i++) {
			r = this->y()[i];
			g = this->u()[i];
			b = this->v()[i];

			y = r *  .299 + g *  .587 + b *  .114 ;
			u = r * -.169 + g * -.332 + b *  .500  + 128.;
			v = r *  .500 + g * -.419 + b * -.0813 + 128.;

			/* clipping to [0 ... 255] */
			if(y < 0) r = 0;
			if(u < 0) g = 0;
			if(v < 0) b = 0;
			if(y > 255) r = 255;
			if(u > 255) g = 255;
			if(v > 255) b = 255;


			f.y()[i] = y;
			f.u()[i] = u;
			f.v()[i] = v;

			return f;
		}
		break;
	} case YUV_422: {
		Frame f = std::move(convert(YUV_444));
		return f.convert(dest);
		break;
	}
	case YUV_420: {
		Frame f = std::move(convert(YUV_444));
		return f.convert(dest);
		break;
	}
	}
	return Frame(m_uvRows, m_uvCols);
}