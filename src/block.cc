#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <assert.h>
#include <cstring>

#include "block.h"

//Block::Block()
//{
//
//}

Block::Block(unsigned int rows, unsigned cols)
{
	this->m_nRows = rows;
	this->m_nCols = cols;

	m_buffer = new int[m_nRows * m_nCols];
}

Block::Block(const Block& b)
	: m_nRows(b.m_nRows), m_nCols(b.m_nCols)
{
	this->m_buffer = new int[m_nRows * m_nCols];
	std::memmove(this->m_buffer, b.m_buffer, (m_nRows * m_nCols));
}

Block::Block(Block&& b)
	: m_nRows(b.m_nRows), m_nCols(b.m_nCols)
{
	m_buffer = b.m_buffer;
	b.m_buffer = NULL;
}

Block::~Block()
{
	if(m_buffer != NULL)
		delete []m_buffer;
}

void Block::setPoint(unsigned int row, unsigned int col, int value)
{
	if(row > m_nRows || col > m_nCols)
		throw IndexOutOfBoundsException();

	m_buffer[row * m_nCols + col] = value;
}

int Block::getPoint(unsigned int row, unsigned int col)
{
	if(row > m_nRows || col > m_nCols)
		throw IndexOutOfBoundsException();

	return m_buffer[row * m_nCols + col];
}

Block* Block::dup()
{
	Block* b = new Block(m_nRows, m_nCols);
	*b = *this; // Copying buffers
	
	return b;
}

int& Block::operator[](unsigned int index)
{
	assert(index < m_nRows * m_nCols);
	
	return m_buffer[index];
}

const int& Block::operator[](unsigned int index) const
{
	assert(index < m_nRows * m_nCols);
	
	return m_buffer[index];
}

Block& Block::operator=(const Block& rhs)
{
	assert (m_nRows == rhs.m_nRows && m_nCols == rhs.m_nCols);

	// TODO: Check if smaller blocks could be used
	//if(m_nRows > rhs.m_nRows && m_nCols > rhs.m_nCols)
	//	throw InvalidDimensionException();

	//std::memmove(this->m_buffer, rhs.m_buffer, (m_nRows * m_nCols));

	for(int i = 0; i < (m_nRows * m_nCols); i++)
		m_buffer[i] = rhs.m_buffer[i];
	return *this;
}

Block& Block::operator=(Block&& rhs)
{
	// TODO: Check if smaller blocks could be used
	if(m_nRows != rhs.m_nRows && m_nCols != rhs.m_nCols)
		throw InvalidDimensionException();
	this->m_buffer = rhs.m_buffer;
	rhs.m_buffer = NULL;
	return *this;
}


void Block::print()
{
	for(int i = 0; i < m_nRows; i++) {
		for(int j = 0; j < m_nCols; j++) 
			std::cout<< m_buffer[i * m_nCols + j] << " ";
		std::cout<< std::endl;
	}
}