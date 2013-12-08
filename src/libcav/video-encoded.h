/*
 * video-encoded.h
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

#ifndef CAV_VIDEO_ENCODED_H
#define CAV_VIDEO_ENCODED_H

#include <string>
#include <fstream>
#include <opencv2/opencv.hpp>
#include "frame.h"
#include "video-interface.h"
#include "bitstream.h"
#include "cav-header.h"

/**
 * Class that has all the information about the Video and manages accessing/displaying the Frames.
 */

class VideoEncoded : public VideoInterface
{
public:
	VideoEncoded(const std::string& path);
	/**
	 * Construct a Video object for writing.	
	 * @param fpath  - File path of the file
	 * @param rows - Number of rows
	 * @param cols - Number of columns.
	 * @param fps - Frames per second.
	 * @param format - VideoFormat.
	 */
	VideoEncoded(const std::string& fpath, uint rows, uint cols, uint fps, VideoFormat format);
	/**
	 * Video Destructor.
	 * Cleans the internal state of the class, closes the stream.
	 */
	virtual ~VideoEncoded()
	{

	}

	/**
	 * Returns the videos next frame. Note that the frame is dynamically allocated, so the resposability of deleting it is delegated to the caller.
	 * @throws VideoEndedException
	 * @return Frame* - Dynamically allocated frame.
	 */
	virtual Frame* getFrame();

	/**
	 * Sets the posistion of the file to the begining.
	 */
	virtual void reset();

	/**
	 * Gets the videos number of frames.
	 * @return total number of frames
	 */
	virtual uint getTotalFrames();

protected:
	/**
	 * File stream to read of write the video (YUV444, YUV422 and YUV422 formats).
	 */
	BitStream m_bs;

	GolombCAVHeader m_header;
	VideoCAVHeader *m_vh;
};

#endif