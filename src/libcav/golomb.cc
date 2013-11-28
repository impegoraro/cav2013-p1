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

#include "cav-header.h"
#include "video-format.h"
#include "predictor.h"
#include "linear-predictor.h"
#include "nonlinear-predictor.h"
#include "golomb.h"
#include "bitstream.h"

static bool isPowerOf2(uint x)
{
	return (x != 0) && ((x & (x-1)) == 0);
}

Golomb::Golomb(Predictor& pred, BitStream& bs, uint m)
	: Coder(pred, bs), m_m(m)
{

}

void Golomb::encode()
{
	uint q, r, m(pow(m_m, 2));
	uint tmp; // temporary holding for error using the even-odd strategy 
	auto errors = m_pred.errors();

	for(auto e : errors) {
		// handles positives as even numbers and negatives as odd numbers
		tmp = (e >= 0) ? 2 * e : 2 * abs(e) - 1; 
		
		q = tmp / m;
		r = tmp % m;
		m_bs.writeNBits(r, m_m);
		for(uint i = 0; i < q; i++)
			m_bs.writeBit(1);
		m_bs.writeBit(0);
	}
}

Predictor Golomb::decode(const std::string& fpath)
{
	VideoFormat vformat;
	uint q, r, size, tmp, i(0);
	int bit;
	CAVHeader tmpHeader;
	BitStream bs(fpath.c_str(), (char*)"rb", &tmpHeader);
	GolombCAVHeader *header{(GolombCAVHeader*) &tmpHeader};
	assert(header->magic == GOLOMB_MAGIC && isPowerOf2(header->m));
	uint m(pow(header->m, 2));
	
	switch(header->format) {
		case 422: 
			vformat = YUV_422; 
			size = header->nCols * header->nRows + ((header->nCols / 2) * header->nRows) * 2; 
			break;
		case 420: 
			vformat = YUV_420; 
			size = header->nCols * header->nRows + ((header->nCols / 2)  * (header->nRows / 2)) * 2; 
			break;
		case 444:
		default: vformat = YUV_444; size = header->nCols * header->nRows * 3; break;

	}
	
	std::vector<int> errors(size);
	while(i < size) {
		q = 0;
		r = bs.readNBits(header->m);
		if(r == EOF) break;
		do {
			bit = bs.readBit();
			if(bit == EOF || !bit) break;
			++q;

		} while(bit);
		tmp = q * m + r;
		errors[i] = (tmp % 2 == 0) ? tmp / 2 : -1 * ((tmp+1) / 2); 
		i++;
	}

	switch(header->predictor) {
		case LINEAR_PREDICTOR: {
			LinearPredictor lp(header->index, header->nRows, header->nCols, vformat, errors);
			return lp;
		break;
		} case NONLINEAR_PREDICTOR: {
			// Non linear
			NonLinearPredictor nlp(header->nRows, header->nCols, vformat, errors);
			return nlp;
		break;
		}default:

			fprintf(stderr, "Invalid predictor in header\n");
			abort();
	}
}

Predictor Golomb::decode(BitStream& bs)
{
	VideoFormat vformat;
	uint q, r, size, tmp, i(0);
	int bit;
	GolombCAVHeader *header{(GolombCAVHeader*) bs.getHeader()};
	assert(header->magic == GOLOMB_MAGIC && isPowerOf2(header->m));
	uint m(pow(header->m, 2));
	
	switch(header->format) {
		case 422: 
			vformat = YUV_422; 
			size = header->nCols * header->nRows + ((header->nCols / 2) * header->nRows) * 2; 
			break;
		case 420: 
			vformat = YUV_420; 
			size = header->nCols * header->nRows + ((header->nCols / 2)  * (header->nRows / 2)) * 2; 
			break;
		case 444:
		default: vformat = YUV_444; size = header->nCols * header->nRows * 3; break;

	}
	
	std::vector<int> errors(size);
	while(i < size) {
		q = 0;
		r = bs.readNBits(header->m);
		if(r == EOF) break;
		do {
			bit = bs.readBit();
			if(bit == EOF || !bit) break;
			++q;

		} while(bit);
		tmp = q * m + r;
		errors[i] = (tmp % 2 == 0) ? tmp / 2 : -1 * ((tmp+1) / 2); 
		i++;
	}

	switch(header->predictor) {
		case LINEAR_PREDICTOR: {
			LinearPredictor lp(header->index, header->nRows, header->nCols, vformat, errors);
			return lp;
		break;
		} case NONLINEAR_PREDICTOR: {
			// Non linear
			NonLinearPredictor nlp(header->nRows, header->nCols, vformat, errors);
			return nlp;
		break;
		}default:

			fprintf(stderr, "Invalid predictor in header\n");
			abort();
	}
}