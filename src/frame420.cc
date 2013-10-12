#include <algorithm>

#include "frame.h"
#include "frame420.h"

Frame420::Frame420(unsigned int nRows, unsigned int nCols) : Frame((nRows / 2), (nCols / 2))
{

}

Frame Frame420::convert()
{
	Frame f((m_uvRows * 2), (m_uvCols * 2));
	
	return(f);
}
