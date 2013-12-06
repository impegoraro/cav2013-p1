/*
 * block.h
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

#ifndef CAV_BLOCK_H
#define CAV_BLOCK_H

#include "exceptions/cav-exceptions.h"


enum class BlockType
{
	Y,
	U,
	V
};

/**
 * The class with all the Block data and methods.
 * The class allows operations in both individual values or using sub blocks.
 */
class Block
{
public:
	/**
	 * Creates an empty block with no associated buffer.
	 */
	Block();
	/**
	 * Initializes the block with the size rows * cols.
	 * @param rows - Number of rows
	 * @param cols - Number of columns
	 */
	Block(uint rows, uint cols);
	/**
	 * Block Move Constructor.
	 * Initializes the block of the same size of b and moves its content to the new block (without copying).
	 * @param b - a reference to a block
	 */
 	Block(Block&& b);
	/**
	 * Block Copy Constructor.
	 * Initializes the block of the same size of b and copies its content.
	 * @param b - a reference to a block
	 */
	Block(const Block& b);

	/**
	 * Removes heap allocated objects.
	 */
	virtual ~Block();

	/**
	 * Sets the value in the buffer int a given row and column
	 * @param row - Row number.
	 * @param col - Column number.
	 * @param value  - The value.
	 */
	void setPoint(uint row, uint col, int value);
	/**
	 * Gets a copy of the value in a given position within the block.
	 * @param row - Row number
	 * @param col - Column number
	 * @return uint - Number of rows
	 */
	int getPoint(uint row, uint col);

	/**
	 * Duplicates the block.
	 * The row, column and the block's content is duplicated. Note that the deletion of the block is delegated to the caller.
	 * @return Block* - A pointer to the newly heap allocated block.
	 */
	Block* dup();

	/**
	 * Gets the number of rows of the defined block.
	 * @return uint - Number of rows
	 */
	uint rows(void) const;
	/**
	 * Gets the number of columns of the defined block.
	 * @return uint - Number of columns
	 */
	uint cols(void) const;
	/**
	 * Gets the size of the block
	 * @return uint - size of the block
	 */
	uint size(void) const;

	/**
	 * Returns the internal buffer of the block.
	 * @return const int* - A pointer to the start of array of ints.
	 */
	int const* buffer() const;

	/**
	 * Copy operator assignment
	 * Copies the block contents, if the block is the same size the buffer is reused otherwise the block is recreated freeing the previously allocated buffer.
	 * @param rhs - A reference to the block.
	 * @return Block& - A reference to the to object itself.
	 */
	Block& operator=(const Block& rhs);
	/**
	 * Copies the buffer rhs into the internal data. Note that the size of the array must be equal to the internal size of the Block, if not the result is unknown.
	 * @param rhs - array of bytes.
	 * @return Block& - a reference to the to object itself.
	 */
	Block& operator=(const char *rhs);
	/**
	 * Move operator assigment,
	 * Moves the buffer from the parameter
	 * @param rhs a rvalue reference of the object to move
	 */
	Block& operator=(Block&& rhs);
	/**
	 * Operator== Overloaded
	 * Checks whether the blocks are equal.
	 * @param rhs - Constant reference to a block
	 * @return bool - true if the blocks are equal, false otherwise.
	 */
	bool operator==(const Block& rhs);
	/**
	 * Operator== Overloaded
	 * Checks whether the block's buffer is equal to a chunk of memory of the same size.
	 * @param rhs - array of bytes.
	 * @return bool - true if the blocks are equal, false otherwise.
	 */
	bool operator==(const char* rhs);
	/**
	 * Overloaded operator[] allows positioning within the block to get or set the value in that position. Note that the return value is a reference to an int.
 	 * @param index - buffer's index to get the value from.
	 * @return uint& - a reference to the value.
	 */ 
	int& operator[](uint index);
	/**
	 * Overloaded operator[] for const Blocks, allows positioning within the block to get the value in that position.
 	 * @param index - buffer's index to get the value from.
	 * @return uint - The value.
	 */ 
	int operator[](uint index) const;

	/**
	 * Return a sub block. Note that the sub block is not a copy, that means that any change to any of its elements will be made to the original block as well.
	 * Also since its not a copy, the block cannot be deleted. Doing so make the sub block useless and any future use of it will result in an unpredictible result.
	 * @param begin - initial position of the sub block.
	 * @param rows - Number of rows of the sub block
	 * @param cols - Number of columns of the sub block.
	 * @return Block - A sub block 
	 */
	Block getSubBlock(uint begin, uint rows, uint cols);

	/**
	 * Return a sub block. Note that the sub block is not a copy, that means that any change to any of its elements will be made to the original block as well.
	 * Also since its not a copy, the block cannot be deleted. Doing so make the sub block useless and any future use of it will result in an unpredictible result.
	 * @param begin - initial position of the sub block.
	 * @param rows - Number of rows of the sub block
	 * @param cols - Number of columns of the sub block.
	 * @return Block - A sub block 
	 */
	const Block getSubBlock(uint begin, uint rows, uint cols) const;
	/**
	 * Copies the values of the a sub block to the internal data.
	 * @param begin - Initial position from where to copy b.
	 * @param b - Sub block to get the values from.
	 */
	void setSubBlock(uint begin, Block& b);

	
	/**
	 * Prints the content of the block.
	 */
	void print();

	Block getSubBlock(uint oRow, uint oCol, uint rows, uint cols);
	const Block getSubBlock(uint oRow, uint oCol, uint rows, uint cols) const;

	uint compareTo(const Block& rhs) const;
	Block operator+(const Block& rhs) const;
	Block operator-(const Block& rhs) const;
protected:
	/**
	 * Number of rows.
	 */
	uint m_nRows;
	/**
	 * Number of columns
	 */
	uint m_nCols;
	/**
	 * Whether destructor should clean the memory (buffer).
	 */
	 bool m_shouldClean;

	 /**
	 * The buffer where that values are stored.
	 */
	int *m_buffer;

};

#endif
