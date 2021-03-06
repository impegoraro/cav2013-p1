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
	GolombInterframe(BitStream& bs, Frame* pf, Frame* nf, unsigned long long m, uint bWidth, uint bHeight, uint radius = 0, int quantY = 1, int quantU = 1, int quantV = 1)
		: m_bs(bs), m_pFrame(pf), m_nFrame(nf), m_m(m), m_bWidth(bWidth), m_bHeight(bHeight), m_radius{radius},
		m_quantY(quantY), m_quantU(quantU), m_quantV(quantV), m_elapsed{0}
	{
		assert(isPowerOf2(m));
	}
	void encode();

	void set_frame(Frame* f)
	{
		m_pFrame = m_nFrame;
		m_nFrame = f;
	}
	
	Frame* decode(unsigned long long m_m);


	double getEncodeTime() const
	{
		return m_elapsed;
	}

protected:
	BitStream& m_bs;
	Frame* m_pFrame;
	Frame* m_nFrame;
	unsigned long long m_m;
	uint m_bWidth;
	uint m_bHeight;
	uint m_radius;
	int m_quantY;
	int m_quantU;
	int m_quantV;
	double m_elapsed;

	void encode(int val);
	void encode(const Block& blk);

	int decode2(unsigned long long m_m);
	void decode(Block& blk, unsigned long long m_m);
};

#endif
