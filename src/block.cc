#include <iostream>
#include <stdio.h>

#include "block.h"

Block::Block()
{

}

Block::Block(unsigned int rows, unsigned cols)
{
	this->m_nRows = rows;
	this->m_nCols = cols;

	m_buffer = new int[m_nRows * m_nCols];
}

Block::~Block()
{
	delete []m_buffer;
}

void Block::setPoint(unsigned int row, unsigned int col, int value)
{
	if(row > m_nRows || col > m_nCols)
		//throw Exception();
		return;

	m_buffer[row * m_nCols + col] = value;
}

Block& operator=(Block& rhs)
{
	// TODO: Check if smaller blocks could be used
	// Also add exception
	if(m_nRows != rhs.m_nRows && m_nCols != rhs.m_nCols)
		return *this;
	
	memcpy(this->buffer, rhs.m_buffer, (m_nRows * m_nCols));
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

//Block::Block& Block::Block(Block&& b)
//{
//	
//}