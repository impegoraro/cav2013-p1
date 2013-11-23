/*
 * framergb.h
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

#ifndef CAV_FRAMERGB_H_
#define CAV_FRAMERGB_H_

#include "frame.h"


/**
 * Intialiazes frame for RGB format. Same size for components R, G and B.
 */
class FrameRGB : public Frame
{
public:
	/**
	 * Constructs a frame in the RGB format.
	 * @param nRows - Number of rows.
	 * @param nCols - Number of columns.
	 */
	FrameRGB(unsigned int nRows, unsigned int nCols);
	/**
	 * Move constructor
	 * @param f - Frame
	 */
	FrameRGB(FrameRGB&& f) : Frame(f)
	{
	}
	/**
	 * Overrided method from the frame base class. This methods converts a frame in RGB format to the new format.
	 * @param format - VideoFormat for the new Frame.
	 */
	virtual Frame convert(VideoFormat format);
};


#endif