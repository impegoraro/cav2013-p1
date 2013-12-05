/*
 * frame.cc
 * Copyright (C) 2013  Ilan Pegoraro and Luís Neves
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <algorithm>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <assert.h>
#include <climits>
#include <opencv2/opencv.hpp>

#include "video-format.h"
#include "frame.h"
#include "frame444.h"
#include "frame422.h"
#include "frame420.h"
#include "framergb.h"

Frame::Frame()
	: m_rows(0), m_cols(0), m_uvRows(0), m_uvCols(0), m_y(nullptr), m_u(nullptr), m_v(nullptr), m_format(YUV_444)
{
}

Frame::Frame(uint nRows, uint nCols)
	: m_rows(nRows), m_cols(nCols), m_uvRows(nRows), m_uvCols(nCols), m_format(YUV_444)
{
	assert(m_rows > 0 && m_cols > 0 && m_uvRows > 0 && m_uvCols > 0);

	m_y = new Block(m_rows, m_cols);
	m_u = new Block(m_uvRows, m_uvCols);
	m_v = new Block(m_uvRows, m_uvCols);
}

Frame::Frame(uint rows, uint cols, uint uvRows, uint uvCols, VideoFormat format)
	: m_rows(rows), m_cols(cols), m_uvRows(uvRows), m_uvCols(uvCols), m_format(format)
{
	assert(m_rows > 0 && m_cols > 0 && m_uvRows > 0 && m_uvCols > 0);

	m_y = new Block(m_rows, m_cols);
	m_u = new Block(m_uvRows, m_uvCols);
	m_v = new Block(m_uvRows, m_uvCols);
}

Frame::Frame(const Frame &f)
	: m_rows(f.m_rows), m_cols(f.m_cols), m_uvRows(f.m_uvRows), m_uvCols(f.m_uvCols), m_format(f.m_format)
{
	assert(m_rows > 0 && m_cols > 0 && m_uvRows > 0 && m_uvCols > 0);
	
	this->m_y = f.m_y->dup();
	this->m_u = f.m_u->dup();
	this->m_v = f.m_v->dup();
}

Frame::Frame(Frame &&f)
	: m_rows(f.m_rows), m_cols(f.m_cols),m_uvRows(f.m_uvRows), m_uvCols(f.m_uvCols), 
	m_y(std::move(f.m_y)), m_u(std::move(f.m_u)), m_v(std::move(f.m_v)), m_format(f.m_format)
{
	assert(m_rows > 0 && m_cols > 0 && m_uvRows > 0 && m_uvCols > 0);

	f.m_y = nullptr;
	f.m_u = nullptr;
	f.m_v = nullptr;
}

Frame::~Frame()
{
	if(m_y != nullptr)
		delete m_y;
	if(m_u != nullptr)
		delete m_u;
	if(m_v != nullptr)
		delete m_v;	
}

Frame& Frame::operator=(const Frame& rhs)
{
	assert(m_rows == rhs.m_rows && m_cols == rhs.m_cols && m_uvRows == rhs.m_uvRows && m_uvCols == rhs.m_uvCols && m_format == rhs.m_format);

	if(m_y != nullptr)
		delete m_y;
	if(m_u != nullptr)
		delete m_u;
	if(m_v != nullptr)
		delete m_v;

	(*m_y) = *rhs.m_y;
	(*m_u) = *rhs.m_u;
	(*m_v) = *rhs.m_v;

	return *this;
}

Frame& Frame::operator=(Frame&& rhs)
{
	m_rows = rhs.m_rows;
	m_cols = rhs.m_cols;
	m_uvRows = rhs.m_uvRows;
	m_uvCols = rhs.m_uvCols;
	m_format = rhs.m_format;

	if(m_y != nullptr)
		delete m_y;
	if(m_u != nullptr)
		delete m_u;
	if(m_v != nullptr)
		delete m_v;

	(m_y) = rhs.m_y;
	rhs.m_y = nullptr;
	(m_u) = rhs.m_u;
	rhs.m_u = nullptr;
	(m_v) = rhs.m_v;
	rhs.m_v = nullptr;

	return *this;
}

void Frame::setBlock(const Block &y, const Block &u, const Block &v)
{
	assert(m_rows > 0 && m_cols > 0 && m_uvRows > 0 && m_uvCols > 0);

	(*m_y) = y;
	(*m_u) = u;
	(*m_v) = v;
}

void Frame::getBlock(Block& y, Block& u, Block& v)
{
	assert(m_rows > 0 && m_cols > 0 && m_uvRows > 0 && m_uvCols > 0);

	y = *m_y;
	u = *m_u;
	v = *m_v;
}

void Frame::setPixel(uint row, uint col, int y, int u, int v)
{
	assert(m_rows > 0 && m_cols > 0 && m_uvRows > 0 && m_uvCols > 0);

	m_y->setPoint(row, col, y);
	m_u->setPoint(row, col, u);	
	m_v->setPoint(row, col, v);
}

void Frame::getPixel(uint row, uint col, int& y, int& u, int& v)
{
	assert(m_rows > 0 && m_cols > 0 && m_uvRows > 0 && m_uvCols > 0);

	y = m_y->getPoint(row, col);
	u = m_u->getPoint(row, col);	
	v = m_v->getPoint(row, col);
}

void Frame::getPixel(uint pos, int& y, int& u, int& v)
{
	assert(m_rows > 0 && m_cols > 0 && m_uvRows > 0 && m_uvCols > 0 && pos < (m_uvRows * m_uvCols));
	y = (*m_y)[pos];
	u = (*m_u)[pos];
	v = (*m_v)[pos];
}

VideoFormat Frame::getFormat() const
{
	return m_format;
}

void Frame::display(bool wait, std::string name)
{
	assert(m_rows > 0 && m_cols > 0 && m_uvRows > 0 && m_uvCols > 0);
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
	std::string winName;
	if(name.size() == 0) {
		std::stringstream ss;

		ss<< "rgb-"<<rand() % 10000;
		winName = std::move(ss.str());
	} else 
		winName = name;
	imshow(winName.c_str(), img);
	if(wait) cv::waitKey(0); // giving control back to opencv 
}

void Frame::setBlackWhite()
{
	assert(m_rows > 0 && m_cols > 0 && m_uvRows > 0 && m_uvCols > 0);

	for(uint i = 0; i < m_uvRows * m_uvCols; i++) {
		u()[i] = 127;
		v()[i] = 127;
	}
}

void Frame::setInvertColors()
{
	assert(m_rows > 0 && m_cols > 0 && m_uvRows > 0 && m_uvCols > 0);

	for(uint i = 0; i < y().rows() * y().cols(); i++)
		y()[i] = 255 - y()[i];

	for(uint i = 0; i < m_uvRows * m_uvCols; i++) {
		u()[i] = 255 - u()[i];
		v()[i] = 255 - v()[i];
	}
}

void Frame::changeLuminance(float factor)
{
	assert(m_rows > 0 && m_cols > 0 && m_uvRows > 0 && m_uvCols > 0);

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

uint Frame::rows() const
{
	return m_rows;
}

uint Frame::cols() const
{
	return m_cols;
}

uint Frame::size() const
{
	return m_rows * m_cols;
}

Block& Frame::y()
{
	return *m_y;
}

const Block& Frame::y() const
{
	return *m_y;
}

Block& Frame::u()
{
	return *m_u;
}

const Block& Frame::u() const
{
	return *m_u;
}

Block& Frame::v()
{
	return *m_v;
}

const Block& Frame::v() const
{
	return *m_v;
}

Frame Frame::convert(VideoFormat format)
{
	assert(false);
	assert(m_rows > 0 && m_cols > 0 && m_uvRows > 0 && m_uvCols > 0);

	Frame f;

	switch(format) {
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

unsigned char* Frame::packedMode(uint& size) const
{
	size = m_rows * m_cols + m_uvRows * m_uvCols * 2;
	unsigned char *buffer = new unsigned char[size];
	
	for(uint i = 0 ; i < m_rows * m_cols * 3 ; i += 3)
	{
		/* Accessing to planar infor */
		buffer[i / 3] = (*m_y)[i / 3];
		if(m_format == RGB || m_format == YUV_444 || ((i/3) < m_u->size())) {
			buffer[(i / 3) + (m_rows * m_cols)] = (*m_u)[i / 3];
			buffer[(i / 3) + (m_rows * m_cols + m_uvRows * m_uvCols)] = (*m_v)[i / 3];
		}
	}
	return buffer;	
}

void Frame::write(const std::string& path)
{
	assert(m_rows > 0 && m_cols > 0 && m_uvRows > 0 && m_uvCols > 0);
	uint size(0);
	unsigned char *b; 
	std::ofstream stream(path, std::ios::trunc | std::ios::out);

	if(!stream.good())
		throw FileNotFoundException();

	b = packedMode(size);
	
	stream<< m_cols<< " "<< m_rows<< " "<<m_format <<std::endl;
	stream.write((char*)b, size);
	delete b;
	stream.close();
}

void Frame::psnr(const Frame& rhs, float& y, float& u, float& v) const
{
	assert(m_y != nullptr && m_u != nullptr && m_v != nullptr);
    float sumY(0), sumU(0), sumV(0);
    float eSqY(0), eSqU(0), eSqV(0);

    eSqY = 1.0 / m_y->size();
    eSqU = 1.0 / m_u->size();
    eSqV = 1.0 / m_v->size();

    for(uint i = 0; i < m_y->size(); i++) {
        sumY += ((*m_y)[i] - rhs.y()[i]) * ((*m_y)[i] - rhs.y()[i]);
    }

    for(uint i = 0; i < m_u->size(); i++) {
        sumU += ((*m_u)[i] - rhs.u()[i]) * ((*m_u)[i] - rhs.u()[i]);
        sumV += ((*m_v)[i] - rhs.v()[i]) * ((*m_v)[i] - rhs.v()[i]);
    }

	y = 10.0 * log10f(255.0 * 255.0 / (eSqY * sumY));
	u = 10.0 * log10f(255.0 * 255.0 / (eSqU * sumU));
	v = 10.0 * log10f(255.0 * 255.0 / (eSqV * sumV));
}

/************************************
 *          Static Methods          * 
 ***********************************/

Frame* Frame::create_from_file(const std::string& path)
{
	std::ifstream stream(path);
	uint cols, rows, type;
	char c;
	Frame *f(nullptr);

	if(!stream.good())
		throw FileNotFoundException();

	stream>>cols>>rows>>type>>c;
	unsigned char buffer[cols * rows * 3];

	switch(type) {
	case 444: {
		f = new Frame444(rows, cols);
		break;
	} case 422: {
		f = new Frame422(rows, cols);
		break;
	} case 420: {
		f = new Frame420(rows, cols);
		break;
	} default:
		throw InvalidVideoTypeException();
	}
	
	stream.read((char*)buffer, f->size() * 3);
	int y, u, v;
	for(uint i = 0 ; i < f->size() * 3; i += 3)
	{ 
		/* Accessing to planar infor */
		y = buffer[i / 3]; 
		f->y()[i / 3] = y;
		
		if(type == YUV_444 || ((i/3) < f->u().size())) {
			u = buffer[(i / 3) + f->y().size()]; 
			v = buffer[(i / 3) + f->y().size() + f->u().size()];
			f->u()[i / 3] = u;
			f->v()[i / 3] = v;
		}
	}
	stream.close();
	return f;
}

// Block Frame::findBestBlock(const Frame& previous, const Block& b, uint radius, int& dr, int& dc, BlockType type)
// {
// 	std::cout<< "should remove"<<std::endl;

// 	assert(false);
// 	std::cout<< "FindBestBlock"<<std::endl;
// 	int bestMatch{INT_MAX}, tmpDiff{0};
// 	Block *inB{nullptr}, *pinB{nullptr};
// 	Block tmpBlock(1,1), bestBlock(1,1);
// 	if(type == BlockType::Y) {inB = m_y; pinB = previous.m_y;}
// 	else if(type == BlockType::U) {inB = m_u; pinB = previous.m_u;}
// 	else if(type == BlockType::V) {inB = m_v; pinB = previous.m_v;}
	
// 	assert(pinB != nullptr);
// 	uint ir{(b.rows() - radius < 0) ? 0 : (b.rows() - radius)};
// 	uint ic{(b.cols() - radius < 0) ? 0 : (b.cols() - radius)};
// 	uint fr{(b.rows() + radius > inB->rows()) ? inB->rows() : (b.rows() + radius)};
// 	uint fc{(b.cols() + radius > inB->cols()) ? inB->cols() : (b.cols() + radius)};

// 	for(uint r = ir; r < fr; r++){
// 		for(uint c = ic; c < fc; c++){
// 			tmpBlock = pinB->getSubBlock(r * previous.cols() + c, b.rows(), b.cols());
// 			tmpDiff = b.compareTo(tmpBlock);
// 			if(tmpDiff < bestMatch) {
// 				bestMatch = tmpDiff;
// 				bestBlock = tmpBlock;
// 				dr = r;
// 				dc = c;
// 			}
// 		}
// 	}	
// 	return bestBlock;
// }

const Block Frame::findBestBlock(const Frame& previous, const Block& b, uint radius, uint actualRow, uint actualCol, int& dr, int& dc, BlockType type) const
{
	int bestMatch{INT_MAX}, tmpDiff{0};
	Block *inB{nullptr}, *pinB{nullptr};
	Block tmpBlock(1,1), bestBlock(1,1);
	if(type == BlockType::Y) {inB = m_y; pinB = previous.m_y;}
	else if(type == BlockType::U) {inB = m_u; pinB = previous.m_u;}
	else if(type == BlockType::V) {inB = m_v; pinB = previous.m_v;}
	
	assert(pinB != nullptr);
	//uint ir{(b.rows() < 0) ? 0 : (actualRow - radius)};
	//uint ic{(b.cols() < 0) ? 0 : (actualCol - radius)};
	//uint fr{(b.rows() + radius >= inB->rows()) ? actualRow + b.rows() : (actualRow  + b.rows() + radius)};
	//uint fc{(b.cols() + radius >= inB->cols()) ? actualCol + b.cols() : (actualCol  + b.cols() + radius)};

	//uint ir{(b.rows() < 0) ? 0 : (actualRow - radius)};
	//uint ic{(b.cols() < 0) ? 0 : (actualCol - radius)};
	//uint fr{(b.rows() + radius > inB->rows()) ? actualRow + b.rows() : (actualRow  + b.rows() + radius)};
	//uint fc{(b.cols() + radius > inB->cols()) ? actualCol + b.cols() : (actualCol  + b.cols() + radius)};
 
 	uint ir{(actualRow < 0) ? 0 : (actualRow - radius)}; 
 	uint ic{(actualCol < 0) ? 0 : (actualCol - radius)}; 
 	uint fr{(actualRow + b.rows() + radius > inB->rows()) ? inB->rows() : (actualRow + b.rows() + radius)}; 
 	uint fc{(actualCol + b.cols() + radius > inB->cols()) ? inB->cols() : (actualCol + b.cols() + radius)};
	
	for(uint r = ir; r + b.rows() < fr + 1; r++){
		for(uint c = ic; c + b.cols() < fc + 1; c++){
			tmpBlock = pinB->getSubBlock(r * pinB->cols() + c, b.rows(), b.cols());
			tmpDiff = b.compareTo(tmpBlock);
			if(tmpDiff < bestMatch) {
				bestMatch = tmpDiff;
				bestBlock = tmpBlock;
				dr = r;
				dc = c;
			}
		}
	}	
	return bestBlock;
}

Frame* Frame::create(uint nRows, uint nCols, VideoFormat format)
{
	switch (format)
	{
		case YUV_444: return new Frame444(nRows, nCols);
		case YUV_422: return new Frame422(nRows, nCols);
		case YUV_420: return new Frame420(nRows, nCols);
		case RGB: return new FrameRGB(nRows, nCols);
	}
	assert(false);
}