/*
 * video-interface.h
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

#ifndef CAV_VIDEO_INTERFACE_H
#define CAV_VIDEO_INTERFACE_H

#include <string>
#include <fstream>
#include <opencv2/opencv.hpp>
#include "frame.h"

/**
 * Class that has all the information about the Video and manages accessing/displaying the Frames.
 */

class VideoInterface
{
public:
	/**
	 * Constructs a video.
	 */
	VideoInterface(uint rows, uint cols, uint fps, VideoFormat format)
		: m_rows(rows), m_cols(cols), m_type(format)
	{
	}
	
	/**
	 * Video Destructor.
	 * Cleans the internal state of the class, closes the stream.
	 */
	virtual ~VideoInterface()
	{ }

	/**
	 * Gets the rows that each frame represents
	 * @return uint - Number of rows
	 */
	virtual uint rows()
	{
		return m_rows;	
	}
	/**
	 * Gets the columns that each frame represents
	 * @return uint - Number of columns
	 */
	virtual uint cols()
	{
		return m_cols;	
	}
	/**
	 * Gets the frames per second
	 * @return uint - Number of frames per second
	 */
	virtual uint fps()
	{
		return m_fps;	
	}

	/**
	 * Displays the video in a window.
	 * @param playing - if true the video runs with its FPS, otherwise a key must be pressed to step through the frames. (default: true)
	 */
	virtual void display(bool playing = true);

	/**
	 * Gets the video's format.
	 * @return VideoFormat - Current VideoFormat.
	 */
	VideoFormat format()
	{
		return m_type;
	}

	/**
	 * Returns the videos next frame. Note that the frame is dynamically allocated, so the resposability of deleting it is delegated to the caller.
	 * @throws VideoEndedException
	 * @return Frame* - Dynamically allocated frame.
	 */
	virtual Frame* getFrame() = 0;

	/**
	 * Sets the posistion of the file to the begining.
	 */
	virtual void reset() = 0;

	/**
	 * Gets the videos number of frames.
	 * @return total number of frames
	 */
	virtual uint getTotalFrames() = 0;

protected:
	inline uint getFrameSize()
	{
		switch(m_type)
		{
			case RGB:
			case YUV_444: return m_rows * m_cols * 3;
			break;
			case YUV_422: return m_rows * m_cols + (m_rows * m_cols / 2) * 2; 
			break;
			case YUV_420: return m_rows * m_cols + (m_rows / 2 * m_cols / 2) * 2;
			break;
		}
		return 0;
	}
	/**
	 * Number of rows of the videos
	 */
	uint m_rows;
	/**
	 * Number of columns of the videos
	 */
	uint m_cols;
	/**
	 * Frame rate (Frames per second)
	 */
	uint m_fps;
	/**
	 * Video's format.
	 */
	VideoFormat m_type;
	/** Header's size. */
	uint m_headerSize;
};

#endif