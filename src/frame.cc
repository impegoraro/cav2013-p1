#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <assert.h>
#include <opencv2/opencv.hpp>

#include "video-format.h"
#include "frame.h"
#include "frame444.h"
#include "frame422.h"
#include "frame420.h"

/**
 * Frame constructor
 * Protected constructor, creates an uninitialized frame. This is used internally by the class hierachy.
 */
Frame::Frame()
	: m_uvRows(0), m_uvCols(0), m_y(NULL), m_u(NULL), m_v(NULL), m_format(YUV_444)
{
}

/**
 * Frame constructor
 * Constructs an frame with the specified rows and columns with all buffers of the same size. The data of the buffer is uninitialized.
 * /param uint - Number of Rows
 * /param uint - Number of Columns
 */
Frame::Frame(uint nRows, uint nCols)
	: m_uvRows(nRows), m_uvCols(nCols), m_format(YUV_444)
{
	assert(m_uvRows > 0 && m_uvCols > 0);

	m_y = new Block(nRows, nCols);
	m_u = new Block(m_uvRows, m_uvCols);
	m_v = new Block(m_uvRows, m_uvCols);
}

/**
 * Frame constructor
 * A Protected constructor used by the hierarchy of Frames (422 and 420) to initialize the internal structure of the frame.
 * /param uint - rows of the Y component
 * /param uint - cols of the Y component
 * /param uint - uvRows of U and V component
 * /param uint - uvCols of U and V component
 */
Frame::Frame(uint rows, uint cols, uint uvRows, uint uvCols, VideoFormat format)
	: m_uvRows(uvRows), m_uvCols(uvCols), m_format(format)
{
	assert(rows > 0 && cols > 0 && m_uvRows > 0 && m_uvCols > 0);

	m_y = new Block(rows, cols);
	m_u = new Block(m_uvRows, m_uvCols);
	m_v = new Block(m_uvRows, m_uvCols);
}

/**
 * Copy Constructor.
 * Makes a copy of each blocks y, u and v.
 * /param Frame - a constant reference to the Frame to copy
 */
Frame::Frame(const Frame &f)
	: m_uvRows(f.m_uvRows), m_uvCols(f.m_uvCols), m_format(f.m_format)
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
	: m_uvRows(f.m_uvRows), m_uvCols(f.m_uvCols), m_y(std::move(f.m_y)), m_u(std::move(f.m_u)), m_v(std::move(f.m_v)), m_format(f.m_format)
{
	assert(m_uvRows > 0 && m_uvCols > 0);

	f.m_y = NULL;
	f.m_u = NULL;
	f.m_v = NULL;
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
	assert(m_uvRows <= rhs.m_uvRows && m_uvCols <= rhs.m_uvCols && m_format == rhs.m_format);

	if(m_y != NULL)
		delete m_y;
	if(m_u != NULL)
		delete m_u;
	if(m_v != NULL)
		delete m_v;

	(*m_y) = *rhs.m_y;
	(*m_u) = *rhs.m_u;
	(*m_v) = *rhs.m_v;

	return *this;
}

Frame& Frame::operator=(Frame&& rhs)
{
	m_uvRows = rhs.rows();
	m_uvCols = rhs.cols();
	m_format = rhs.m_format;

	if(m_y != NULL)
		delete m_y;
	if(m_u != NULL)
		delete m_u;
	if(m_v != NULL)
		delete m_v;

	(m_y) = rhs.m_y;
	rhs.m_y = NULL;
	(m_u) = rhs.m_u;
	rhs.m_u = NULL;
	(m_v) = rhs.m_v;
	rhs.m_v = NULL;

	return *this;
}

void Frame::setBlock(const Block &y, const Block &u, const Block &v)
{
	assert(m_uvRows > 0 && m_uvCols > 0);
	(*m_y) = y;
	(*m_u) = u;
	(*m_v) = v;
}

void Frame::getBlock(Block& y, Block& u, Block& v)
{
	assert(m_uvRows > 0 && m_uvCols > 0);
	y = *m_y;
	u = *m_u;
	v = *m_v;
}

void Frame::setPixel(uint row, uint col, int y, int u, int v)
{
	assert(m_uvRows > 0 && m_uvCols > 0);
	m_y->setPoint(row, col, y);
	m_u->setPoint(row, col, u);	
	m_v->setPoint(row, col, v);
}

void Frame::getPixel(uint row, uint col, int& y, int& u, int& v)
{
	assert(m_uvRows > 0 && m_uvCols > 0);
	y = m_y->getPoint(row, col);
	u = m_u->getPoint(row, col);	
	v = m_v->getPoint(row, col);
}

void Frame::getPixel(uint pos, int& y, int& u, int& v)
{
	assert(m_uvRows > 0 && m_uvCols > 0 && pos < (m_uvRows * m_uvCols));
	y = (*m_y)[pos];
	u = (*m_u)[pos];
	v = (*m_v)[pos];
}

VideoFormat Frame::getFormat()
{
	return m_format;
}

void Frame::display()
{
	assert(m_uvRows > 0 && m_uvCols > 0);
	int r, g, b;
	int y, u, v;
	Frame f;
	uint yRows, yCols;
	unsigned char *buffer;
	
	// Move the converted frame instead of just copying it.
	f = std::move(convert(YUV_444));
	
	yRows = f.rows();
	yCols = f.cols();
	cv::Mat img = cv::Mat(cv::Size(yCols, yRows), CV_8UC3);

	buffer = (uchar*)img.ptr();
	for(uint i = 0; i < f.rows() * f.cols() * 3; i += 3) {
		
		y = f.y()[i / 3];
		u = f.u()[(i / 3)];
		v = f.v()[(i / 3)];

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

/**
 * Converts the frame to black and white.
 */
void Frame::setBlackWhite()
{
	for(uint i = 0; i < m_uvRows * m_uvCols; i++) {
		u()[i] = 127;
		v()[i] = 127;
	}
}

/**
 * Inverts the colors of the current frame.
 */
void Frame::setInvertColors()
{
	for(uint i = 0; i < y().rows() * y().cols(); i++)
		y()[i] = 255 - y()[i];

	for(uint i = 0; i < m_uvRows * m_uvCols; i++) {
		u()[i] = 255 - u()[i];
		v()[i] = 255 - v()[i];
	}
}

/**
 * Change the luminance of the frame.
 */
void Frame::changeLuminance(float factor)
{
	int y_;
	for(uint i = 0; i < y().rows() * y().cols(); i++) {
		y_ = y()[i] * factor;

		if(y_ < 0)
			y_ = 0;
		else if(y_ > 255)
			y_ = 255;
		y()[i] = y_;
	}
}

/**
 * Gets the number of rows of the defined frame.
 * /returns uint - Number of rows
 */
uint Frame::rows()
{
	return m_uvRows;
}

/**
 * Gets the number of columns of the defined frame.
 * /returns uint - Number of columns
 */
uint Frame::cols()
{
	return m_uvCols;
}

/**
 * Gets the block defined by the component Y.
 * /returns Block& - A reference to the block Y.
 */
Block& Frame::y()
{
	return *m_y;
}

/**
 * Gets the block defined by the component U.
 * /returns Block& - A reference to the block U.
 */
Block& Frame::u()
{
	return *m_u;
}

/**
 * Gets the block defined by the component V
 * /returns Block& - A reference to the block V.
 */
Block& Frame::v()
{
	return *m_v;
}

/**
 * Converts a Frame to YUV 444
 */
Frame Frame::convert(VideoFormat dest)
{
	assert(m_uvRows > 0 && m_uvCols > 0);

	Frame f;
	
	switch(dest) {
	case RGB:
	break;
	case YUV_444:
		f = std::move(Frame(*this));
		break;
	case YUV_422:

		break;
	case YUV_420:

		break;
	}
	return f;
}

/**
 * Test function to write a frame to a file specified by path.
 * /param const  std::string& - The file path.
 */
void Frame::write(const std::string& path)
{
	std::ofstream stream(path, std::ios::trunc | std::ios::out);

	if(!stream.good())
		throw FileNotFoundException();

	stream<<m_uvCols<< " "<<m_uvRows<< " "<<m_format<<std::endl;
	unsigned char buffer[m_uvRows * m_uvCols * 3];

	for(uint i = 0; i < m_uvRows * m_uvCols; i++) {
		buffer[i * 3] = (*m_y)[i];
		buffer[i * 3 + m_uvRows * m_uvCols] = (*m_u)[i];
		buffer[i * 3 + m_uvRows * m_uvCols * 2] = (*m_v)[i];
	}
	stream.write((char*)buffer, (m_uvRows * m_uvCols * 3));
	stream.close();
}

/************************************
 *          Static Methods          * 
 ***********************************/


/**
 * Creates a frame from the a filename
 */
Frame* Frame::create_from_file(const std::string& path)
{
	std::ifstream stream(path);
	uint cols, rows, type;
	uint uvCols(0), uvRows(0), size(0);
	char c;
	Frame *f(NULL);

	if(!stream.good())
		throw FileNotFoundException();

	stream>>cols>>rows>>type>>c;
	unsigned char buffer[cols * rows * 3];

	switch(type) {
	case 444: {
		uvRows = rows;
		uvCols = cols;
		size = rows * cols * 3;
		f = new Frame444(rows, cols);
		break;
	} case 422: {
		uvRows = rows;
		uvCols = cols / 2;
		size = rows * cols + uvRows * uvCols * 2;
		f = new Frame422(rows, cols);
		cols /= 2;
		break;
	} case 420: {
		rows = rows / 2;
		cols = cols / 2;
		size = rows * cols + uvRows * uvCols * 2;

		f = new Frame420(rows, cols);
		break;
	} default:
		throw InvalidVideoTypeException();
	}
	
	stream.read((char*)buffer, size);
	int y,u,v;
	for(uint i = 0 ; i < rows * cols * 3 ; i += 3)
	{ 
		/* Accessing to planar infor */
		y = buffer[i / 3]; 
		f->y()[i / 3] = y;
		
		if(type == YUV_444 || ((i/3) < f->u().size())) {
			u = buffer[(i / 3) + (rows * cols)]; 
			v = buffer[(i / 3) + (rows * cols + uvRows * uvCols)];
			f->u()[i / 3] = u;
			f->v()[i / 3] = v;
		}
	}
	stream.close();
	return f;
}
