#include <iostream>
#include <algorithm>
#include <assert.h>
#include <cstring>

#include "block.h"

//Block::Block()
//{
//
//}

/**
 * Block Constructor.
 * Initializes the block with the size rows * cols.
 * /param unsigned int - Number of rows
 * /param unsigned int - Number of columns
 */
Block::Block(unsigned int rows, unsigned cols)
{
	this->m_nRows = rows;
	this->m_nCols = cols;

	m_buffer = new int[m_nRows * m_nCols];
}

/**
 * Block Copy Constructor.
 * Initializes the block of the same size of b and copies its content.
 * /param Block& - a reference to a block
 */
Block::Block(const Block& b)
	: m_nRows(b.m_nRows), m_nCols(b.m_nCols)
{
	this->m_buffer = new int[m_nRows * m_nCols];
	std::memmove(this->m_buffer, b.m_buffer, (m_nRows * m_nCols));
	assert(*this == b);
}

/**
 * Block Copy Constructor.
 * Initializes the block of the same size of b and copies its content.
 * /param Block& - a reference to a block
 */
Block::Block(Block&& b)
	: m_nRows(b.m_nRows), m_nCols(b.m_nCols)
{
	m_buffer = b.m_buffer;
	b.m_buffer = NULL;
}

/**
 * Block destructores.
 * Removes heap allocated objects.
 */
Block::~Block()
{
	if(m_buffer != NULL)
		delete []m_buffer;
}

/**
 * Sets the value in the buffer int a given row and column
 * /param unsigned int - Row number 
 * /param unsigned int - Column number
 * /param int - the value
 */
void Block::setPoint(unsigned int row, unsigned int col, int value)
{
	assert(row > m_nRows || col > m_nCols);

	m_buffer[row * m_nCols + col] = value;
}

/**
 * Gets the value of a given position in the block.
 * /param unsigned int - Row number
 * /param unsigned int - Column number
 * /returns unsigned int - Number of rows
 */
int Block::getPoint(unsigned int row, unsigned int col)
{
	assert(row > m_nRows || col > m_nCols);

	return m_buffer[row * m_nCols + col];
}

/**
 * Gets the number of rows of the defined block.
 * /returns unsigned int - Number of rows
 */
unsigned int Block::rows(void)
{
	return m_nRows;
}

/**
 * Gets the number of columns of the defined block.
 * /returns unsigned int - Number of columns
 */
unsigned int Block::cols(void)
{
	return m_nCols;
}

/**
 * Duplicates the block.
 * The row, column and the block's content is duplicated. Note that the deletion of the block is delegated to the caller.
 * /returns Block* - a pointer to the newly heap allocated block.
 */
Block* Block::dup()
{
	Block* b = new Block(m_nRows, m_nCols);
	*b = *this; // Copying buffers
	
	assert (*b == *this);
	return b;
}

bool Block::operator==(const Block& rhs)
{
	if(this->m_nRows != rhs.m_nRows || this->m_nCols != rhs.m_nCols)
		return false;

	for(int i = 0; i < m_nRows * m_nCols; i++) {
		if(m_buffer[i] != rhs.m_buffer[i])
			return false;
	}
	return true;
}

/**
 * Overloaded operator[] allows positioning within the block to get or set the value in that position.
 * /returns int& - a reference to the value.
 */ 
int& Block::operator[](unsigned int index)
{
	assert(index < m_nRows * m_nCols);
	
	return m_buffer[index];
}

/**
 * Overloaded operator[] allows positioning within the block to get the value in that position.
 * /returns int - a reference to the value.
 */ 
int Block::operator[](unsigned int index) const
{
	assert(index < m_nRows * m_nCols);
	
	return m_buffer[index];
}

/**
 * Copy operator assignment
 * Copies the block contents, if the block is the same size the buffer is reused otherwise the block is recreated freeing the previously allocated buffer.
 * /param const Block& - a reference to the block 
 * /returns Block& - a reference to the to object itself.
 */
Block& Block::operator=(const Block& rhs)
{
	if(m_nRows != rhs.m_nRows && m_nCols != rhs.m_nCols) {
		delete []m_buffer;
		m_nRows = rhs.m_nRows;
		m_nCols = rhs.m_nCols;
	}

	for(int i = 0; i < (m_nRows * m_nCols); i++)
		m_buffer[i] = rhs.m_buffer[i];

	assert (*this == rhs);
	return *this;
}

/**
 * Copies the buffer rhs into the internal data. Note that the size of the array must be equal to the internal size of the Block, if not the result is unknown.
 * /param const char * - array of bytes.
 * /returns Block& - a reference to the to object itself.
 */
Block& Block::operator=(const char *rhs)
{
	
	for(int i = 0; i < m_nRows * m_nCols; i++)
		(*this)[i] = rhs[i]; 
}

/**
 * Move operator assigment,
 * Moves the buffer from the parameter
 * /param Block&& a rvalue reference of the object to move
 */
Block& Block::operator=(Block&& rhs)
{
	// TODO: Check if smaller blocks could be used
	if(m_nRows != rhs.m_nRows && m_nCols != rhs.m_nCols)
		throw InvalidDimensionException();
	
	if(this->m_buffer != NULL)
		delete []this->m_buffer;

	this->m_buffer = rhs.m_buffer;
	rhs.m_buffer = NULL;
	return *this;
}

/**
 * Prints the content of the block.
 */
void Block::print()
{
	for(int i = 0; i < m_nRows; i++) {
		for(int j = 0; j < m_nCols; j++) 
			std::cout<< m_buffer[i * m_nCols + j] << " ";
		std::cout<< std::endl;
	}
}