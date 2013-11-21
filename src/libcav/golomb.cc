/*
 * golomb.cc
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
#include <assert.h>
#include <algorithm>
#include <sstream>
#include <cmath>

#include "predictor.h"
#include "golomb.h"
#include "bitstream.h"

Golomb::Golomb(Predictor& pred, const std::string& fpath, uint m)
	: Coder(pred, fpath), m_m(m)
{
	assert(m_m > 0 && m_fpath.size() > 0);
}

void Golomb::encode()
{
	BitStream bs(m_fpath.c_str(), (char*)"wb");
	uint q, r, m(pow(m_m, 2));
	uint tmp; // temporary holding for error using the even-odd strategy 
	auto errors = m_pred.predict();
	std::stringstream ss;

	ss<< m_pred.frame().cols()<< " "
	  << m_pred.frame().rows()<< " "
	  << 1 << " " // get predictors 
	  << m_pred.index();
	bs.writeHeader(ss.str());

	for(auto e : errors) {
		// handles positives as even numbers and negatives as odd numbers
		tmp = (e >= 0) ? 2 * e : 2 * abs(e) - 1; 
		
		q = tmp / m;
		r = tmp % m;
		bs.writeNBits(r, m_m);
		for(uint i = 0; i < q; i++)
			bs.writeBit(1);
		bs.writeBit(0);
	}
}

std::vector<int> Golomb::decode()
{
	BitStream bs(m_fpath.c_str(), (char*)"rb");
	//TODO: using hardcoded values, this should go with the file.
	uint m(pow(m_m, 2));
	uint q, r;
	int bit, index;
	PredictorType pred;
	uint cols(0), rows(0), tmp, i(0);

	bs.readHeader(cols, rows, pred, index);

	std::vector<int> errors(cols * rows * 3);
	while(i < (cols * rows * 3)) {
		q = 0;
		r = bs.readNBits(m_m);
		if(r == EOF) break;
		do {
			bit = bs.readBit();
			if(bit == EOF || !bit) break;
			q = (q << 1) | bit;

		} while(bit);
		tmp = q * m + r;
		errors[i] = (tmp % 2 == 0) ? tmp / 2 : -1 * ((tmp+1) / 2); 
		i++;
	}
	
	return errors;
}