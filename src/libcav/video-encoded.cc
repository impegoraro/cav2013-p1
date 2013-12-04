/*
 * video-encoded.cc
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

#include <string>
#include <fstream>
#include <opencv2/opencv.hpp>
#include "predictor.h"
#include "golomb.h"
#include "frame.h"
#include "video-encoded.h"
#include "video-interface.h"
#include "cav-header.h"

VideoEncoded::VideoEncoded(const std::string& path)
	: VideoInterface(0, 0, 0, YUV_444), m_bs(path.c_str(), (char *)"rb", (CAVHeader*)&m_header), 
	m_vh((VideoCAVHeader*) ((GolombCAVHeader*) &m_header)->undefined)
{
	m_rows = m_header.nRows;
	m_cols = m_header.nCols;
	m_fps = m_vh->fps;
	if(m_header.format == 444) m_type = YUV_444;
	else if(m_header.format == 422) m_type = YUV_422;
	else if(m_header.format == 420) m_type = YUV_420;
}

Frame* VideoEncoded::getFrame()
{
	Predictor pred = Golomb::decode(m_bs);
	return pred.guess();
}

void VideoEncoded::reset()
{
}

uint VideoEncoded::getTotalFrames()
{
	return 10;
}