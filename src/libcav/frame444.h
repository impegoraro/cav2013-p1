/*
 * frame444.h
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

#ifndef FRAME444_H_
#define FRAME444_H_

#include "frame.h"
#include "video-format.h"

/**
 * Intialiazes frame for YUV444 format. Same size for components Y, U and V.
 */
class Frame444 : public Frame
{
public:
	/**
	 * Constructs a frame in the YUV444 format.
	 * @param nRows - Number of rows.
	 * @param nCols - Number of columns.
	 */
	Frame444(unsigned int nRows, unsigned int nCols);

	/**
	 * Move constructor.
	 * Moves a frame (base class) of type YUV444 to a real Frame444 object
	 * @param f Reference of the previous object.
	 */
	Frame444(Frame&& f);

	/**
	 * Move constructor.
	 * Moves a frame of type YUV444
	 * @param f Reference of the previous object.
	 */
	Frame444(Frame444&& f);

	/**
	 * Overrided method from the frame base class. This methods converts a frame in YUV444 format to the new format.
	 * @param dest - VideoFormat for the new Frame.
 	 */
	virtual Frame convert(VideoFormat dest);
};

#endif