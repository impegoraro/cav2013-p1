#include <iostream>
#include <algorithm>
#include <assert.h>
#include <cstring>

#include "block.h"

Block::Block()
	: shouldClean(false)
{
}

/**
 * Block Constructor.
 * Initializes the block with the size rows * cols.
 * /param unsigned int - Number of rows
 * /param unsigned int - Number of columns
 */
Block::Block(unsigned int rows, unsigned cols)
	: shouldClean(true), m_nRows(rows), m_nCols(cols)
{
	m_buffer = new int[m_nRows * m_nCols];
}

/**
 * Block Copy Constructor.
 * Initializes the block of the same size of b and copies its content.
 * /param Block& - a reference to a block
 */
Block::Block(const Block& b)
	: m_nRows(b.m_nRows), m_nCols(b.m_nCols), shouldClean(true)
{
	this->m_buffer = new int[m_nRows * m_nCols];
	// Aparently the memmove does not copies the buffer correctly
	//std::memmove(this->m_buffer, b.m_buffer, (m_nRows * m_nCols));

	//Falling back to copying byte by byte
	for(int i = 0; i < m_nRows * m_nCols; i++)
		m_buffer[i] = b.m_buffer[i];
	assert(*this == b);
}

/**
 * Block Copy Constructor.
 * Initializes the block of the same size of b and copies its content.
 * /param Block& - a reference to a block
 */
Block::Block(Block&& b)
	: m_nRows(b.m_nRows), m_nCols(b.m_nCols), shouldClean(true)
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
	if(m_buffer != NULL && shouldClean)
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
 * Gets the size of the block
 * /returns unsigned int - size of the block
 */
unsigned int Block::size(void)
{
	return m_nRows * m_nCols;
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

/**
 * Returns a sub block. Note that the sub block is not a copy that means that any change to any of its elements will be made to the original block.
 * /return Block - A subblock 
 */
Block Block::getSubBlock(unsigned int begin, unsigned int end)
{
	assert(begin > end && end <= m_nRows * m_nCols);
	Block b;
	b.m_nRows = begin;
	b.m_nCols = end;
	b.m_buffer = &m_buffer[begin];

	return b;
}

/**
 * Copies the values of the a sub block to the internal data.
 * 
 */
 /*TODO: implement const methods to let the 3rd parameter to be a const Block& */
void Block::setSubBlock(unsigned int begin, unsigned int end, Block& b)
{
	assert(begin > end && end <= m_nRows * m_nCols);
	for(int i = begin; i < end; i++)
		m_buffer[i] = b.m_buffer[i / begin];
}

/**
 * Operator== Overloaded
 * Checks whether the blocks are equal.
 * /param const Block& - Constant reference to a block
 * /returns bool - true if the blocks are equal, otherwise false.
 */
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
 * Operator== Overloaded
 * Checks whether the block's buffer is equal to a chunk of memory of the same size.
 * /param const const char* - array of bytes.
 * /returns bool - true if the blocks are equal, otherwise false.
 */
bool Block::operator==(const char* rhs)
{
	bool res = true;
	for(int i = 0; i < (m_nRows * m_nCols); i++)
		if(m_buffer[i] != rhs[i])
			res = false;
	return res;
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
	if(m_nRows != rhs.m_nRows || m_nCols != rhs.m_nCols) {
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
		m_buffer[i] = rhs[i];
	assert (*this == rhs);
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