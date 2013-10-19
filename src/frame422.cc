#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <assert.h>

#include "frame.h"
#include "frame422.h"
#include "video-format.h"

Frame422::Frame422(unsigned int nRows, unsigned int nCols) : Frame(nRows, nCols, nRows, (nCols / 2), YUV_422)
{
}

Frame Frame422::convert()
{
	Frame f(m_uvRows, (m_uvCols * 2));
	int l = 0;

	f.y() = *m_y; // copies the Y buffer as is
	int cols = f.cols();
	for (int r = 0; r < m_uvRows; r++)
		for (int c = 0; c < m_uvCols; c++) {
			f.u()[r * cols + c * 2] = (*m_u)[r * m_uvCols + c];
			f.u()[r * cols + c * 2 + 1] = (*m_u)[r * m_uvCols + c];
			
			f.v()[r * cols + c * 2] = (*m_v)[r * m_uvCols + c];
			f.v()[r * cols + c * 2 + 1] = (*m_v)[r * m_uvCols + c];
		}

	return f;
}