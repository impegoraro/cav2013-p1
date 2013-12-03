/*
 * golomb-interframe.cc
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

#include <iostream>
#include <cmath>

#include "util.h"
#include "golomb-interframe.h"

void GolombInterframe::encode()
{
	uint bSize{m_bWidth * m_bHeight};
	uint dr{0}, dc{0};
	Block b2, b1, be;

	// Y
	for(uint r = 0; r < m_nFrame->rows(); r += bSize) {
		for(uint c = 0; c < m_nFrame->cols(); c += bSize) {
			b2 = m_nFrame->y().getSubBlock(r, c, bSize);
			//(const Frame& previous, const Block& b, uint radius, uint& dr, uint& dc, BlockType type)
			b1 = m_pFrame->findBestBlock(*m_pFrame, b2, bSize, dr, dc, BlockType::Y);
			be = b2 - b1;
			this->encode((int)dr);
			this->encode((int)dc);
			this->encode(be);
		}
	}

	for(uint r = 0; r < m_nFrame->rows(); r += bSize) {
		for(uint c = 0; c < m_nFrame->cols(); c += bSize) {
			b2 = m_nFrame->u().getSubBlock(r, c, bSize);
			b1 = m_pFrame->findBestBlock(*m_pFrame, b2, bSize, dr, dc, BlockType::U);
			be = b2 - b1;
			this->encode((int)dr);
			this->encode((int)dc);
			this->encode(be);
		}
	}

	for(uint r = 0; r < m_nFrame->rows(); r += bSize) {
		for(uint c = 0; c < m_nFrame->cols(); c += bSize) {
			b2 = m_nFrame->v().getSubBlock(r, c, bSize);
			b1 = m_pFrame->findBestBlock(*m_pFrame, b2, bSize, dr, dc, BlockType::V);
			be = b2 - b1;
			this->encode((int)dr);
			this->encode(dc);
			this->encode(be);
		}
	}
}

void GolombInterframe::encode(int val)
{
	uint q, r, m(pow(m_m, 2));
	uint tmp; // temporary holding for error using the even-odd strategy 

	// handles positives as even numbers and negatives as odd numbers
	tmp = (val >= 0) ? 2 * val : 2 * abs(val) - 1; 
	
	q = tmp / m;
	r = tmp % m;
	m_bs.writeNBits(r, m_m);
	for(uint i = 0; i < q; i++)
		m_bs.writeBit(1);
	m_bs.writeBit(0);
}

void GolombInterframe::encode(const Block& blk)
{
	for(uint i = 0; i < blk.size(); i++)
		encode(blk[i]);
}