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
	//std::cout<< "GolombInterframe encode"<<std::endl;
	uint radius{2};
	uint dr{0}, dc{0};
	Block b2, b1, be;

	// Y
	for(uint r = 0; r < m_nFrame->rows() - m_bWidth; r += m_bWidth) {
		for(uint c = 0; c < m_nFrame->cols() - m_bWidth; c += m_bHeight) {
			b2 = m_nFrame->y().getSubBlock(r * m_nFrame->cols() + c, m_bWidth, m_bHeight);
			//(const Frame& previous, const Block& b, uint radius, uint& dr, uint& dc, BlockType type)
			b1 = m_pFrame->findBestBlock(*m_pFrame, b2, radius, dr, dc, BlockType::Y);
			be = b2 - b1;
			this->encode(dr);
			this->encode(dc);
			this->encode(be);
		}
	}
	//std::cout<< "GolombInterframe Y Completed"<<std::endl;

	for(uint r = 0; r < m_nFrame->u().rows() - m_bWidth; r += m_bWidth) {
		for(uint c = 0; c < m_nFrame->u().cols() - m_bHeight; c += m_bHeight) {
			b2 = m_nFrame->u().getSubBlock(r * m_nFrame->u().cols() + c, m_bWidth, m_bHeight);
			b1 = m_pFrame->findBestBlock(*m_pFrame, b2, radius, dr, dc, BlockType::U);
			be = b2 - b1;
			this->encode(dr);
			this->encode(dc);
			this->encode(be);
		}
	}
	//std::cout<< "GolombInterframe U Completed"<<std::endl;

	for(uint r = 0; r < m_nFrame->u().rows() - m_bWidth; r += m_bWidth) {
		for(uint c = 0; c < m_nFrame->u().cols() - m_bHeight; c += m_bHeight) {
			b2 = m_nFrame->v().getSubBlock(r * m_nFrame->u().cols() + c, m_bWidth, m_bHeight);
			b1 = m_pFrame->findBestBlock(*m_pFrame, b2, radius, dr, dc, BlockType::V);
			be = b2 - b1;
			this->encode(dr);
			this->encode(dc);
			this->encode(be);
		}
	}
	//std::cout<< "GolombInterframe V Completed"<<std::endl;

}

void GolombInterframe::encode(int val)
{
	uint q, r, m(pow(2, m_m));
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

// int GolombInterframe::decode(uint m_m)
// {
// 	int r, q = 0;
// 	uint m{pow(m_m, 2)};
// 	r = m_bs.readNBits(m_m);
// 	if(r == EOF) break;
// 	do {
// 		bit = m_bs.readBit();
// 		if(bit == EOF || !bit) break;
// 		++q;

// 	} while(bit);
// 	tmp = q * m + r;
// 	int e = (tmp % 2 == 0) ? tmp / 2 : -1 * ((tmp+1) / 2); 
// 	i++;
// }


void GolombInterframe::encode(const Block& blk)
{
	for(uint i = 0; i < blk.size(); i++)
		encode(blk[i]);
}