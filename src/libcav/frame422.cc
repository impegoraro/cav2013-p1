/*
 * frame422.cc
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

#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <assert.h>

#include "frame.h"
#include "framergb.h"
#include "frame444.h"
#include "frame422.h"
#include "frame420.h"

#include "video-format.h"

Frame422::Frame422(uint nRows, uint nCols) : Frame(nRows, nCols, nRows, (nCols / 2), YUV_422)
{
}

Frame Frame422::convert(VideoFormat dest)
{
	assert(m_rows > 0 && m_cols > 0 && m_uvRows > 0 && m_uvCols > 0);

	switch(dest) {
		case RGB: {
			Frame444* f = new Frame444(std::move(convert(YUV_444)));
			Frame fdest = std::move(f->convert(dest));
			delete f;
			return fdest;
		}
		break;
		case YUV_444: {
			Frame444 f(m_uvRows, (m_uvCols * 2));
			
			f.y() = *m_y; // copies the Y buffer as is
			for (uint i = 0; i < f.cols() * f.rows(); i+=2) { 
				f.u()[i + 1] = f.u()[i] = u()[i / 2]; 
				f.v()[i + 1] = f.v()[i] = v()[i / 2];
			}
			return f;
		}
		break;
		case YUV_422: {
			return Frame(*this);
		}
		break;
		case YUV_420: {
			Frame444* f = new Frame444(std::move(convert(YUV_444)));
			Frame f420 = std::move(f->convert(dest));
			delete f;
			return f420;
		}
		break;
	}
 
	return Frame(m_uvRows, m_uvCols);
}