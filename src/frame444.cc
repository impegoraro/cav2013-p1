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
		return f;
		break;
	} case YUV_444:
		return Frame(*this);
		break;
	case YUV_422: {
		Frame422 f(m_uvRows, m_uvCols);

		f.y() = y();

		for(int i = 0; i< m_uvRows * m_uvCols / 2; i++) {
			f.u()[i] = u()[i * 2];
			f.v()[i] = v()[i * 2];
		}

		return f;
		break;
	}
	case YUV_420: {
		Frame420 f(m_uvRows, m_uvCols);

		f.y() = y();
		for(int r = 0; r < m_uvRows; r += 2)
			for(int c = 0; c < m_uvCols; c += 2)
			{
				f.u()[(r / 2) * f.u().cols() + c / 2] = u()[m_uvCols + c];
				f.v()[(r / 2) * f.v().cols() + c / 2] = v()[m_uvCols + c];
			}
		

		/*for (unsigned int l = 0, lr = 0; lr < m_uvRows / 2; l += 2, lr++) {
	        for (unsigned int c = 0, cr = 0; cr < m_uvCols / 2; c += 2, cr++) {
	                unsigned int pos = c + (l * m_uvCols);
	                unsigned int posRaw = cr + (lr * (m_uvCols / 2));

	                f.u()[posRaw] = u()[pos];
	                f.v()[posRaw] = v()[pos];
	        }
        }*/
		return f;
		}
		break;
	}
	return Frame(m_uvRows, m_uvCols);
}