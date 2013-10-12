#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <stdio.h>
#include <cstdlib>
#include <time.h>
#include <opencv2/opencv.hpp>

#include "video-format.h"
#include "frame.h"


Frame::Frame(unsigned int nRows, unsigned int nCols)
	: m_uvRows(nRows), m_uvCols(nCols)
{
	assert(m_uvRows > 0 && m_uvCols > 0);

	m_y = new Block(nRows, nCols);
	m_u = new Block(m_uvRows, m_uvCols);
	m_v = new Block(m_uvRows, m_uvCols);
}

Frame::Frame(unsigned int rows, unsigned int cols, unsigned int uvRows, unsigned int uvCols)
	: m_uvRows(uvRows), m_uvCols(uvCols)
{
	assert(rows > 0 && cols > 0 && m_uvRows > 0 && m_uvCols > 0);

	m_y = new Block(rows, cols);
	m_u = new Block(m_uvRows, m_uvCols);
	m_v = new Block(m_uvRows, m_uvCols);
}

Frame::Frame(const Frame &f)
	: m_uvRows(f.m_uvRows), m_uvCols(f.m_uvCols)
{
	assert(m_uvRows > 0 && m_uvCols > 0);

	this->m_y = f.m_y->dup();
	this->m_u = f.m_u->dup();
	this->m_v = f.m_v->dup();
}

/** Move Constructor. 
 *	Moves the buffer instead of doing an unnecessary copy.
 *  /param The frame to move
 */

Frame::Frame(Frame &&f)
	: m_uvRows(f.m_uvRows), m_uvCols(f.m_uvCols), m_y(std::move(f.m_y)), m_u(std::move(f.m_u)), m_v(std::move(f.m_v))
{
	assert(m_uvRows > 0 && m_uvCols > 0);
}

Frame::~Frame()
{
	if(m_y != NULL)
		delete m_y;
	if(m_u != NULL)
		delete m_u;
	if(m_v != NULL)
		delete m_v;	
}

Frame& Frame::operator=(const Frame& rhs)
{
	//assert(m_uvRows <= rhs.m_uvRows && m_uvCols <= rhs.m_uvCols);
	(*m_y) = *rhs.m_y;
	(*m_u) = *rhs.m_u;
	(*m_v) = *rhs.m_v;
}

void Frame::setBlock(const Block &y, const Block &u, const Block &v)
{
	(*m_y) = y;
	(*m_u) = u;
	(*m_v) = v;
}

void Frame::getBlock(Block& y, Block& u, Block& v)
{
	y = *m_y;
	u = *m_u;
	v = *m_v;
}

void Frame::setPixel(int row, int col, int y, int u, int v)
{
	m_y->setPoint(row, col, y);
	m_u->setPoint(row, col, u);	
	m_v->setPoint(row, col, v);
}

void Frame::getPixel(int row, int col, int& y, int& u, int& v)
{
	y = m_y->getPoint(row, col);
	u = m_u->getPoint(row, col);	
	v = m_v->getPoint(row, col);
}

void Frame::getPixel(int pos, int& y, int& u, int& v)
{
	y = (*m_y)[pos];
	u = (*m_u)[pos];	
	v = (*m_v)[pos];
}

Frame Frame::convert()
{
	Frame f(*this);
	
	return f;
}

void Frame::display()
{
//	std::cout<< "printing: m_y"<< std::endl;
//	m_y->print();
//	std::cout<< "printing: m_u"<< std::endl;
//	m_u->print();
//	std::cout<< "printing: m_v"<< std::endl;
//	m_v->print();

	Frame f = convert();
	int r, g, b;
	int y, u, v;
	int yRows(f.rows()), yCols(f.cols());
	cv::Mat img = cv::Mat(cv::Size(yCols, yRows), CV_8UC3);
	unsigned char *buffer;

	buffer = (uchar*)img.ptr();
	for(int i = 0; i < f.rows() * f.cols() * 3; i += 3) {
		y = f.y()[i / 3];
		u = f.u()[(i / 3) + (yRows * yCols)];
		v = f.v()[(i / 3) + (yRows * yCols) * 2];

		/* convert to RGB */
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

		buffer[i] = b;
		buffer[i + 1] = g;
		buffer[i + 2] = r;
	}

	imshow("rgb", img);
}

int Frame::rows()
{
	return m_uvRows;
}

int Frame::cols()
{
	return m_uvCols;
}

Block& Frame::y()
{
	return *m_y;
}
Block& Frame::u()
{
	return *m_u;
}
Block& Frame::v()
{
	return *m_v;
}