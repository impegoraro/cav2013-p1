/*
 * frame.h
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

#ifndef CAV_FRAME_H
#define CAV_FRAME_H

#include <string>

#include "block.h"
#include "video-format.h"

/**
 * The frame class stores the different kinds of Blocks, and has the general methods
 * required to manipulate/access the Frame.
 * This is the base class for the following formats YUV444, YUV422, YUV420 and RGB, 
 * so all the implemented methods work in the format that the frame was created.
 */
class Frame
{
public:
	/**
	 * Frame constructor
	 * Protected constructor, creates an uninitialized frame. 
	 * This is used internally by the class hierachy.
	 */
	Frame();
	/**
	 * Frame constructor
	 * Constructs a frame with the specified rows and columns and of type YUV444 
	 * (all buffers of the same size). The buffer is uninitialized.
	 * @param nRows - Number of Rows
	 * @param nCols - Number of Columns
	 */
	Frame(uint nRows, uint nCols);
	/**
	 * Copy Constructor.
	 * Makes a copy of each blocks y, u and v.
	 * @param f - a constant reference to the Frame to copy
	 */
	Frame(const Frame& f);
	/** Move Constructor. 
	 *	Moves the buffer instead of doing an unnecessary copy.
	 *  @param f - The frame to move
	 */
	Frame(Frame&& f);
	/**
	 * Frame detructor, frees all memory used by the frame.
	 */
	virtual ~Frame();
	/**
	 * Gets the current VideoFormat.
	 */
	VideoFormat getFormat() const;

	/**
	 * Changes the frames components.
	 * @param y - Y Component to copy 
	 * @param u - U Component to copy
	 * @param v - V Component to copy
	 */
	void setBlock(const Block &y, const Block &u, const Block &v);
	/**
	 * Makes a copy of the internal buffers.
	 * @param y - Y Component to copy 
	 * @param u - U Component to copy
	 * @param v - V Component to copy
	 */
	void getBlock(Block& y, Block& u, Block& v);
	
	/**
	 * Changes the value of a pixel.
	 * @param row - row to change
	 * @param col - column to change
	 * @param y - Y value
	 * @param u - U value
	 * @param v - V value
	 */
	void setPixel(uint row, uint col, int y, int u, int v);
	/**
	 * Gets the value of a pixel.
	 * @param row - row numer
	 * @param col - column number
	 * @param y - Y value
	 * @param u - U value
	 * @param v - V value
	 */
	void getPixel(uint row, uint col, int& y, int& u, int& v);
	/**
	 * Gets the value of a pixel in linear mode.
	 * @param pos - position within the blocks.
	 * @param y - Y value
	 * @param u - U value
	 * @param v - V value
	 */
	void getPixel(uint pos, int& y, int& u, int& v);
	/**
	 * Displays the current frame in a window.
	 */
	void display(bool wait = true, std::string name = "");

	/**
	 * Converts the frame to black and white.
	 */
	void setBlackWhite();
	/**
	 * Inverts the colors of the current frame.
	 */
	void setInvertColors();
	/**
	 * Change the luminance of the frame.
	 */
	void changeLuminance(float factor);

	/**
	 * Gets the number of rows of the defined frame.
	 * @return uint - Number of rows
	 */
	uint rows() const;
	/**
	 * Gets the number of columns of the defined frame.
	 * @return uint - Number of columns
	 */
	uint cols() const;

	/**
	 * Returns the size of the frame.
	 * @return uint - size of the frame
	 */
	uint size() const;

	/**
	 * Makes a copies the frame into the internal buffers.
	 * @param rhs - The source frame.
	 */
	virtual Frame& operator=(const Frame& rhs);
	/**
	 * Moves the buffers of rhs into the internal buffers.
	 * @param rhs - The source frame.
	 */
	virtual Frame& operator=(Frame&& rhs);

	/**
	 * Gets the block defined by the component Y.
	 * Since it returns a reference to the block all changes made to the 
	 * block are apply directly in the frame.
	 * @return Block& - A reference to the block Y.
	 */
	Block& y();

	/**
	 * Gets the block defined by the component Y.
	 * @return const Block& - A constant reference to the block Y.
	 */
	const Block& y() const;
	
	/**
	 * Gets the block defined by the component U.
	 * Since it returns a reference to the block all changes made to the 
	 * block are apply directly in the frame.
	 * @return Block& - A reference to the block U.
	 */
	Block& u();
	
	/**
	 * Gets the block defined by the component U.
	 * @return const Block& - A constant reference to the block U.
	 */
	const Block& u() const;

	/**
	 * Gets the block defined by the component V.
	 * Since it returns a reference to the block all changes made to the 
	 * block are apply directly in the frame.
	 * @return Block& - A reference to the block V.
	 */
	Block& v();

	/**
	 * Gets the block defined by the component V.
	 * @return const Block& - A constant reference to the block V
	 */
	const Block& v() const;

	/**
	 * Compares two the current frame with the one passed as a parameter.
	 * @param rhs Frame to compare
	 * @param y - Result for the Y component 
	 * @param u - Result for the U component
	 * @param v - Result for the V component
	 */
	void psnr(const Frame& rhs, float& y, float& u, float& v) const;

	/**
	 * Creates a buffer of the YUV buffers in packed mode, the size of the resulting 
	 * buffer is returned by the first parameter. Note that the buffer is dynamically allocated
	 * therefore must be freed by the caller.
	 * @param size - size of the newly created buffer.
	 * @return unsigned char* - buffer in packed mode.
	 */
	virtual unsigned char* packedMode(uint& size) const;

	/**
	 * Writes a frame to the mass storage device.
	 * @param path - The file path where the frame is to be saved on.
	 */
	virtual void write(const std::string& path);

	/**
	 * Converts a Frame to format specified by the format.
	 * Since this method is virtual the base class is allow to override is behaviour. Note 
	 * that this is done by the FrameRGB, Frame444, Frame422 and Frame420 classes. 
	 * @param format - format of the new frame.
	 */
	virtual Frame convert(VideoFormat format);
	/**
	 * Static method that creates a frame from a file.
	 * Note that the method dynamically allocates the frame so the resposibility of 
	 * removing the frame lies in the caller.
	 * @param path - File from where to read the frame.
	 * @return Frame* - dyanmically allcated frame.
	 */
	static Frame* create_from_file(const std::string& path);

	/**
	 * Creates a new frame in the specified format.
	 * @param nRows - Number of Rows
	 * @param nCols - Number of Columns
	 * @param format - Frame format
	 */
	static Frame* create(uint nRows, uint nCols, VideoFormat format);

	/**
	 *
	 */
	//Block findBestBlock(const Frame& previous, const Block& b, uint radius, int& dr, int& dc, BlockType type);
	const Block findBestBlock(const Frame& previous, const Block& b, uint radius, uint actualRow, uint actualCol, int& dr, int& dc, BlockType type) const;
protected:
	/**
	 * Number of rows
	 */
	uint m_rows;
	/**
	 * Number of columns
	 */
	uint m_cols;
	/**
	 * Number of rows of the U and V component.
	 */
	uint m_uvRows;
	/**
	 *Number of columns of the U and V component.
	 */
	uint m_uvCols;

	/**
	 * Buffer for the Y component.
	 */
	Block* m_y;
	/**
	 * Buffer for the U component,
	 */
	Block* m_u;
	/**
	 * Buffer for the V component.
	 */
	Block* m_v;

	/**
	 * Videos format
	 */
	VideoFormat m_format;
	// Dumb constructor that doesnt initializes anything. 
	// Its purpose is to help the parse function without using the heap.
	/**
	 * Frame constructor
	 * A Protected constructor used by the hierarchy of Frames (RGB, 422 and 420) to 
	 * initialize the internal structure of the frame.
	 * @param rows - rows of the Y component
	 * @param cols - cols of the Y component
	 * @param uvRows - uvRows of U and V component
	 * @param uvCols - uvCols of U and V component
	 * @param format - Videos format!
	 */
	Frame(uint rows, uint cols, uint uvRows, uint uvCols, VideoFormat format);
};

#endif