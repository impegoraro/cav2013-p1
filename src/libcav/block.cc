/*
 * block.cc
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
#include <cmath>
#include <algorithm>
#include <assert.h>
#include <cstring>

#include "block.h"


Block::Block()
	: m_nRows(0), m_nCols(0), m_shouldClean(false), m_buffer(NULL)
{
}

Block::Block(uint rows, uint cols)
	: m_nRows(rows), m_nCols(cols), m_shouldClean(true)
{
	assert(m_nRows > 0 && m_nCols > 0);
	m_buffer = new int[m_nRows * m_nCols];

	//std::memset(m_buffer, 0, sizeof(int) * size());
}

Block::Block(const Block& b)
	: m_nRows(b.m_nRows), m_nCols(b.m_nCols), m_shouldClean(true)
{
	this->m_buffer = new int[m_nRows * m_nCols];
	std::memcpy(this->m_buffer, b.m_buffer, sizeof(int) * m_nRows * m_nCols);

	////Falling back to copying byte by byte
	//for(uint i = 0; i < m_nRows * m_nCols; i++)
	//	m_buffer[i] = b.m_buffer[i];
	assert(*this == b);
}

Block::Block(Block&& b)
	: m_nRows(b.m_nRows), m_nCols(b.m_nCols), m_shouldClean(b.m_shouldClean), m_buffer(b.m_buffer)
{
	b.m_buffer = NULL;
}

Block::~Block()
{
	if(m_buffer != NULL && m_shouldClean)
		delete []m_buffer;
}

void Block::setPoint(uint row, uint col, int value)
{
	assert(row > m_nRows || col > m_nCols);

	m_buffer[row * m_nCols + col] = value;
}

int Block::getPoint(uint row, uint col)
{
	assert(row > m_nRows || col > m_nCols);

	return m_buffer[row * m_nCols + col];
}

uint Block::rows(void) const
{
	return m_nRows;
}

uint Block::cols(void) const
{
	return m_nCols;
}

uint Block::size(void) const
{
	return m_nRows * m_nCols;
}

int const* Block::buffer() const
{
	return m_buffer;
}

Block* Block::dup()
{
	Block* b = new Block(m_nRows, m_nCols);
	*b = *this; // Copying buffers
	
	assert (*b == *this);
	return b;
}

Block Block::getSubBlock(uint begin, uint rows, uint cols)
{
	assert(begin + rows * cols <= m_nRows * m_nCols);
	Block b;
	b.m_nRows = rows;
	b.m_nCols = cols;
	b.m_buffer = &m_buffer[begin];

	return b;
}

 /*TODO: implement const methods to let the 3rd parameter to be a const Block& */
void Block::setSubBlock(uint begin, Block& b)
{
	assert(begin + b.rows() * b.cols() <= m_nRows * m_nCols);
	
	for(uint i = begin, c = 0; c < b.rows() * b.cols(); i++, c++)
		m_buffer[i] = b.m_buffer[c];
}

bool Block::operator==(const Block& rhs)
{
	bool res(false);

	if(this->m_nRows == rhs.m_nRows && this->m_nCols == rhs.m_nCols) {
		//res = true; // assume the blocks are equals
		//for(uint i = 0; i < (m_nRows * m_nCols); i++) {
		//	if(m_buffer[i] != rhs.m_buffer[i]) {
		//		res = false; // first element different, return false
		//		break;
		//	}
		//}
		return (std::memcmp(m_buffer, rhs.m_buffer, sizeof(int) * m_nRows * m_nCols) == 0);
	}
	return res;
}

bool Block::operator==(const char* rhs)
{
	return std::memcmp(m_buffer, rhs, sizeof(int) * m_nRows * m_nCols);
	//bool res = true;
	//for(uint i = 0; i < (m_nRows * m_nCols); i++)
	//	if(m_buffer[i] != rhs[i]) {
	//		res = false;  // first element different, return false
	//		break;
	//	}
	//return res;
}

int& Block::operator[](uint index)
{
	assert(index < m_nRows * m_nCols);
	
	return m_buffer[index];
}

int Block::operator[](uint index) const
{
	assert(index < m_nRows * m_nCols);
	
	return m_buffer[index];
}

Block& Block::operator=(const Block& rhs)
{
	if(m_nRows != rhs.m_nRows || m_nCols != rhs.m_nCols) {
		if(m_buffer != NULL && m_shouldClean)
			delete []m_buffer;
		m_nRows = rhs.m_nRows;
		m_nCols = rhs.m_nCols;
		m_buffer = new int[m_nRows * m_nCols];
		m_shouldClean = true;
	}
	std::memcpy(m_buffer, rhs.m_buffer, sizeof(int) * m_nRows * m_nCols);
	//Fallback method if errors occurs
	//for(uint i = 0; i < (m_nRows * m_nCols); i++)
	//	m_buffer[i] = rhs.m_buffer[i];

	assert (*this == rhs);
	return *this;
}

Block& Block::operator=(const char *rhs)
{
	//for(uint i = 0; i < m_nRows * m_nCols; i++)
	//	m_buffer[i] = rhs[i];

	std::memcpy(this->m_buffer, rhs, sizeof(int) * m_nRows * m_nCols);
	assert (*this == rhs);
	return *this;
}

Block& Block::operator=(Block&& rhs)
{
	if(this->m_buffer != NULL && m_shouldClean)
		delete []this->m_buffer;
	
	this->m_nRows = rhs.m_nRows;
	this->m_nCols = rhs.m_nCols;
	this->m_shouldClean = rhs.m_shouldClean;
	this->m_buffer = rhs.m_buffer;
	rhs.m_buffer = NULL;
	return *this;
}

uint Block::compareTo(const Block& rhs) const
{
	uint val{0};
	assert(this->m_nRows == rhs.m_nRows && this->m_nCols > rhs.m_nCols);
	for (uint i = 0; i < rhs.size(); i++)
		val += abs((*this)[i] - rhs[i]);

	return val;
}

Block Block::operator+(const Block& rhs) const
{
	assert(this->m_nRows == rhs.m_nRows && this->m_nCols > rhs.m_nCols);
	Block b(m_nRows, m_nCols);
	for (uint i = 0; i < rhs.size(); i++)
		b[i] = (*this)[i] + rhs[i];

	return b;
}

Block Block::operator-(const Block& rhs) const
{
	assert(this->m_nRows == rhs.m_nRows && this->m_nCols > rhs.m_nCols);
	Block b(m_nRows, m_nCols);
	for (uint i = 0; i < rhs.size(); i++)
		b[i] = (*this)[i] - rhs[i];

	return b;
}

void Block::print()
{
	for(uint i = 0; i < m_nRows; i++) {
		for(uint j = 0; j < m_nCols; j++) 
			std::cout<< m_buffer[i * m_nCols + j] << " ";
		std::cout<< std::endl;
	}
}