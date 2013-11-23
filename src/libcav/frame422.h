/*
 * frame422.h
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

#ifndef FRAME422_H_
#define FRAME422_H_

#include "frame.h"
#include "video-format.h"


/**
 * Intialiazes frame for 422. The components U and V have half the columns of Y.
 */
class Frame422 : public Frame
{
public:
	/**
	 * Constructs a frame with half the columns for the U and V components (YUV422 format).
	 * @param nRows - Number of rows.
	 * @param nCols - Number of columns.
	 */
	Frame422(unsigned int nRows, unsigned int nCols);

	/**
	 * Move constructor
	 * @param f - Frame
	 */
	Frame422(Frame422&& f) : Frame(f)
	{
	}

	/**
	 * Overrided method from the frame base class. This methods converts a frame in YUV422 format to the new format.
	 * @param dest - VideoFormat for the new Frame.
	 */
	virtual Frame convert(VideoFormat dest);
protected:
};

#endif