#include <iostream>
#include <stdio.h>

#include "video-format.h"
#include "frame.h"

Frame::Frame(unsigned int nRows, unsigned int nCols, VideoFormat format) 
	: m_format(format)
{
	switch(format) {
		case YUV_444:
			m_uvRows = nRows;
			m_uvCols = nCols;
			break;
		case YUV_422:
			m_uvRows = nRows;
			m_uvCols = nCols / 2;
			break;
		case YUV_420:
			m_uvRows = nRows / 2;
			m_uvCols = nCols / 2;
			break;
	}
	m_y = new Block(nRows, nCols);
	m_u = new Block(m_uvRows, m_uvCols);
	m_v = new Block(m_uvRows, m_uvCols);
}

Frame::~Frame()
{
	delete m_y;
	delete m_u;
	delete m_v;	
}