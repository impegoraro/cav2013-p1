#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <time.h>

#include "frame.h"
#include "frame422.h"
#include "video-format.h"

Frame422::Frame422(unsigned int nRows, unsigned int nCols) : Frame(nRows, nCols, nRows, (nCols / 2), YUV_422)
{
}

Frame Frame422::convert()
{
	Frame f(m_uvRows, (m_uvCols * 2));
	f.y() = *m_y;

	//for(int i = 0; i < (m_uvRows * (m_uvCols * 2)); i++) {
	//	if(i >= (m_uvRows * m_uvCols)) {
	//		//f.u()[i] = f.u()[i-1] ;
	//		//f.v()[i] = f.v()[i-1] ;
	//	} else{
	//		f.u()[i] = (*m_u)[i];
	//		f.v()[i] = (*m_v)[i];
	//	}
	//}

	for(int r = 0; r < m_uvRows - 1; r++)
		for(int c = 0; c < m_uvCols; c++) {
			f.u()[r * f.cols() + c] = (*m_u)[m_uvCols + c];
			f.u()[(r+1) * f.cols() + c * 2] = (*m_u)[m_uvCols + c];
	
			f.v()[r * f.cols() + c] = (*m_v)[m_uvCols + c];
			f.v()[(r+1) * f.cols() + c * 2] = (*m_v)[m_uvCols + c];
		}

	return f;
}