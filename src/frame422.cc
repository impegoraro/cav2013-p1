#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <time.h>

#include "frame.h"
#include "frame422.h"

Frame422::Frame422(unsigned int nRows, unsigned int nCols) : Frame(nRows, nCols, nRows, (nCols / 2))
{
	//TODO: Remove randomness
	srand(time(NULL));
	for(int i = 0; i < m_uvRows * m_uvCols; ++i) {
		(*m_u)[i] = rand() % 255;
		(*m_v)[i] = rand() % 255;
	}
}

Frame Frame422::convert()
{
	Frame f(m_uvRows, (m_uvCols * 2));
	int l = 0;

	f.y() = *m_y;
		
	for(int i = 0; i < (m_uvRows * (m_uvCols * 2)); i++) {
		if(i >= (m_uvRows * m_uvCols)) {
			f.u()[i] = f.u()[i - 2];
			f.v()[i] = f.v()[i - 2];
			++l;
		} else{
			f.u()[i] = (*m_u)[i];
			f.v()[i] = (*m_v)[i];
		}
	}
	return f;
}