#include <algorithm>

#include "frame.h"
#include "frame420.h"
#include "video-format.h"

Frame420::Frame420(unsigned int nRows, unsigned int nCols) : Frame(nRows, nCols, (nRows / 2), (nCols / 2), YUV_420)
{

}

Frame Frame420::convert(VideoFormat dest)
{
	Frame f((m_uvRows * 2), (m_uvCols * 2));
	
	f.y() = y();

	for(int r = 0; r < m_uvRows; r++) {
		for(int c = 0; c < m_uvCols; c++) {
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