/*
 * golomb-interframe.h
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

#ifndef GOLOMB_INTERFRAME_H
#define GOLOMB_INTERFRAME_H

#include <cmath>
#include <assert.h>

#include "coder.h"
#include "util.h"
#include "block.h"
#include "bitstream.h"
#include "frame.h"


class GolombInterframe
{
public:
	GolombInterframe(BitStream& bs, const Frame* pf, const Frame* nf, unsigned long long m, uint bWidth, uint bHeight, uint radius = 0)
		: m_bs(bs), m_pFrame(pf), m_nFrame(nf), m_m(m), m_bWidth(bWidth), m_bHeight(bHeight), m_radius{radius}
	{
		assert(isPowerOf2(m));
	}
	void encode();

	void set_frame(const Frame* f)
	{
		m_pFrame = m_nFrame;
		m_nFrame = f;
	}
	
	Frame* decode(unsigned long long m_m);
protected:
	BitStream& m_bs;
	const Frame* m_pFrame;
	const Frame* m_nFrame;
	unsigned long long m_m;
	uint m_bWidth;
	uint m_bHeight;
	uint m_radius;

	void encode(int val);
	void encode(const Block& blk);

	int decode2(unsigned long long m_m);
	void decode(Block& blk, unsigned long long m_m);
};

#endif
