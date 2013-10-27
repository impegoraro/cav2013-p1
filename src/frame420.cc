#include <algorithm>

#include "frame.h"
#include "frame444.h"
#include "frame422.h"
#include "frame420.h"
#include "video-format.h"


Frame420::Frame420(unsigned int nRows, unsigned int nCols) : Frame(nRows, nCols, (nRows / 2), (nCols / 2), YUV_420)
{

}

Frame Frame420::convert(VideoFormat dest)
{
	switch(dest) {
		case RGB: {
			Frame444* f = new Frame444(std::move(convert(YUV_444)));
			Frame fdest = std::move(f->convert(dest));
			delete f;
			return fdest;
		}
		break;
		case YUV_444: {
			Frame444 f((m_uvRows * 2), (m_uvCols * 2));
			
			f.y() = y();

			for(uint r = 0; r < m_uvRows; r++) {
				for(uint c = 0; c < m_uvCols; c++) {
					f.u()[(r * 2) * f.cols() + c * 2] = u()[r * m_uvCols + c];
					f.u()[(r * 2) * f.cols() + c * 2+1] = u()[r * m_uvCols + c];
					f.u()[(r * 2 + 1) * f.cols() + c * 2] = u()[r * m_uvCols + c];
					f.u()[(r * 2 + 1) * f.cols() + c * 2+1] = u()[r * m_uvCols + c];

					f.v()[(r * 2) * f.cols() + c * 2] = v()[r * m_uvCols + c];
					f.v()[(r * 2) * f.cols() + c * 2 + 1] = v()[r * m_uvCols + c];
					f.v()[(r * 2 + 1) * f.cols() + c * 2] = v()[r * m_uvCols + c];
					f.v()[(r * 2 + 1) * f.cols() + c * 2 + 1] = v()[r * m_uvCols + c];
					
				}
			}
			return f;
		}
		break;
		case YUV_422: {
			Frame444* f = new Frame444(std::move(convert(YUV_444)));
			Frame f422 = std::move(f->convert(dest));
			delete f;
			return f422;
		}
		break;
		case YUV_420: {
			return Frame420(*this);
		}
		break;
	}

	return Frame(m_uvRows, m_uvCols);
}