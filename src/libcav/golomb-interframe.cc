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
	uint radius{0};
	int dr{0}, dc{0};
	Block b2, b1, be;

	// Y
	for(uint r = 0; r < m_nFrame->rows(); r += m_bWidth) {
		for(uint c = 0; c < m_nFrame->cols(); c += m_bHeight) {
			b2 = m_nFrame->y().getSubBlock(r * m_nFrame->cols() + c, m_bWidth, m_bHeight);
			//b1 = m_pFrame->y().getSubBlock(r * m_nFrame->cols() + c, m_bWidth, m_bHeight);
			b1 = m_pFrame->findBestBlock(*m_pFrame, b2, radius, r, c, dr, dc, BlockType::Y);
			be = b2 - b1;
			int tdr = dr - r;
			int tdc = dc - c;
			//int tdr = 0;
			//int tdc = 0;
			this->encode(tdr);
			this->encode(tdc);
			this->encode(be);
		
		}
	}
	//std::cout<< "GolombInterframe Y Completed"<<std::endl;

	for(uint r = 0; r < m_nFrame->u().rows(); r += m_bWidth) {
		for(uint c = 0; c < m_nFrame->u().cols(); c += m_bHeight) {
			b2 = m_nFrame->u().getSubBlock(r * m_nFrame->u().cols() + c, m_bWidth, m_bHeight);
			//b1 = m_pFrame->u().getSubBlock(r * m_nFrame->u().cols() + c, m_bWidth, m_bHeight);
			b1 = m_pFrame->findBestBlock(*m_pFrame, b2, radius, r, c, dr, dc, BlockType::U);
			be = b2 - b1;
			int tdr = dr - r;
			int tdc = dc - c;
			//int tdr = 0;
			//int tdc = 0;
			this->encode(tdc);
			this->encode(tdr);
			this->encode(be);
		}
	}
	//std::cout<< "GolombInterframe U Completed"<<std::endl;

	for(uint r = 0; r < m_nFrame->u().rows(); r += m_bWidth) {
		for(uint c = 0; c < m_nFrame->u().cols(); c += m_bHeight) {
			b2 = m_nFrame->v().getSubBlock(r * m_nFrame->u().cols() + c, m_bWidth, m_bHeight);
			//b1 = m_pFrame->v().getSubBlock(r * m_nFrame->u().cols() + c, m_bWidth, m_bHeight);
			b1 = m_pFrame->findBestBlock(*m_pFrame, b2, radius, r, c, dr, dc, BlockType::V);
			be = b2 - b1;
			int tdr = dr - r;
			int tdc = dc - c;
			//int tdr = 0;
			//int tdc = 0;
			this->encode(tdr);
			this->encode(tdc);
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

void GolombInterframe::encode(const Block& blk)
{
	for(uint i = 0; i < blk.size(); i++)
		encode(blk[i]);
}


Frame* GolombInterframe::decode(uint m_m)
{
	assert(isPowerOf2(m_m));
	//std::cout<< "GolombInterframe encode"<<std::endl;
	int dr{0}, dc{0};
	Block b2, b1, be(m_bWidth, m_bHeight);
	Frame *actual = Frame::create(m_pFrame->rows(), m_pFrame->cols(), m_pFrame->getFormat());
	int tdc, tdr;

	// Y
	for(uint r = 0; r < m_pFrame->rows(); r += m_bWidth) {
		for(uint c = 0; c < m_pFrame->cols(); c += m_bHeight) {
			tdr = this->decode2(m_m);
			tdc = this->decode2(m_m);
			this->decode(be, m_m);

			dr = tdr + r;
			dc = tdc + c;
			b2 = m_pFrame->y().getSubBlock(dr * m_pFrame->cols() + dc, m_bWidth, m_bHeight);
			//b1 = m_pFrame->findBestBlock(*m_pFrame, b2, radius, dr, dc, BlockType::Y);
			b1 = b2 + be;
			actual->y().setSubBlock(r * actual->cols() + c, b1);
		
		}
	}

	for(uint r = 0; r < m_pFrame->u().rows(); r += m_bWidth) {
		for(uint c = 0; c < m_pFrame->u().cols(); c += m_bHeight) {
			tdr = this->decode2(m_m);
			tdc = this->decode2(m_m);
			this->decode(be, m_m);
			dr = tdr + r;
			dc = tdc + c;
			
			b2 = m_pFrame->u().getSubBlock(dr * m_pFrame->u().cols() +dc, m_bWidth, m_bHeight);
			//b1 = m_pFrame->findBestBlock(*m_pFrame, b2, radius, dr, dc, BlockType::U);
			b1 = b2 + be;
			actual->u().setSubBlock(r * actual->u().cols() + c, b1);
		}
	}

	for(uint r = 0; r < m_pFrame->u().rows(); r += m_bWidth) {
		for(uint c = 0; c < m_pFrame->u().cols(); c += m_bHeight) {
			tdr = this->decode2(m_m);
			tdc = this->decode2(m_m);
			this->decode(be, m_m);
			dr = tdr + r;
			dc = tdc + c;

			b2 = m_pFrame->v().getSubBlock(dr * m_pFrame->u().cols() + dc, m_bWidth, m_bHeight);
			//b1 = m_pFrame->findBestBlock(*m_pFrame, b2, radius, dr, dc, BlockType::V);
			b1 = b2 + be;
			actual->v().setSubBlock(r * actual->u().cols() + c, b1);
		}
	}
	return actual;
}


int GolombInterframe::decode2(uint m_m)
{
	assert(isPowerOf2(m_m));
	uint r, q = 0, tmp,  m(pow(2, m_m));
	int bit;

	r = m_bs.readNBits(m_m);
	if(r == EOF) throw int(-1);
	do {
		bit = m_bs.readBit();
		if(bit == EOF || !bit) break;
		++q;

	} while(bit);
	tmp = q * m + r;
	int e = (tmp % 2 == 0) ? tmp / 2 : -1 * ((tmp+1) / 2); 

	return e;
}

void GolombInterframe::decode(Block& blk, uint m_m)
{
	assert(isPowerOf2(m_m));
	for(uint i = 0; i < blk.size(); i++)
		blk[i] = decode2(m_m);
}