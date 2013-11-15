/*
 * linear-predictor.cc
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

#include "linear-predictor.h"

static const std::function<int (int, int, int) > m_defFunctors [] = {
	{[](int a, int b, int c) -> int { return a; }},
	{[](int a, int b, int c) -> int { return b; }},
	{[](int a, int b, int c) -> int { return c; }},
	{[](int a, int b, int c) -> int { return a + b - c; }},
	{[](int a, int b, int c) -> int { return a + (b - c) / 2; }},
	{[](int a, int b, int c) -> int { return b + (a - c) / 2; }},
	{[](int a, int b, int c) -> int { return (a + b) / 2; }}
};

LinearPredictor::LinearPredictor(const Frame& f)
	: LinearPredictor(f, 0)
{
}

LinearPredictor::LinearPredictor(const Frame& f, uint type)
	: Predictor(f), m_functor(m_defFunctors[type])
{
	assert(type <= 6);
}
	 
LinearPredictor::LinearPredictor(const Frame& f, std::function< int(int, int, int) >& functor)
	: Predictor(f), m_functor(functor)
{
}

std::vector<int> LinearPredictor::predict()
{
	uint i(0), uSize = (m_f.u().rows() * m_f.u().cols());
	std::vector<int> errors(m_f.size() + uSize * 2);
	int x, xv;
	int a, b, c2, av, bv, cv;

	// Predicting the Y component
	for(uint r = 0; r < m_f.rows(); r++) {
		for(uint c = 0; c < m_f.cols(); c++){
			x = m_f.y()[r * m_f.cols() + c];
			if(c == 0 && r == 0){
				// First pixel at F[0][0], all values are considered to be 0
				errors[i] = x;
			} else if (c != 0 && r == 0) {
				// First row b and c are considered to be 0 (no previous value)
				a = m_f.y()[r * m_f.cols() + c - 1];
				// b and c2 are zero 
				errors[i] = x - m_functor(a, 0, 0);
			} else if(c == 0 && r != 0) {
				// First column a and b are considered to be 0
				// a and c  are zeros 

				b = m_f.y()[(r - 1) * m_f.cols() + c];
				errors[i] = x - m_functor(0, b, 0);
			} else { 
				a = m_f.y()[r * m_f.cols() + c - 1];
				b = m_f.y()[(r - 1) * m_f.cols() + c];
				c2 = m_f.y()[(r - 1) * m_f.cols() + c - 1];
				errors[i] = x - m_functor(a, b, c2);
			}
			i++;
		}
	}

	// Predicting the U and V components
	i = 0;
	for(uint r = 0; r < m_f.u().rows(); r++) {
		for(uint c = 0; c < m_f.u().cols(); c++) {
			x = m_f.u()[r * m_f.u().cols() + c];
			xv = m_f.v()[r * m_f.v().cols() + c];
			

			if(c == 0 && r == 0){
				// First pixel at F[0][0], all values are considered to be 0
				errors[m_f.size() + i] = x;
				errors[m_f.size() + uSize + i] = xv;
			} else if (c != 0 && r == 0) {
				// First row b and c are considered to be 0 (no previous value)
				a = m_f.u()[r * m_f.u().cols() + c - 1];
				av = m_f.v()[r * m_f.v().cols() + c - 1];
				
				errors[m_f.size() + i] = x - m_functor(a, 0, 0);
				errors[m_f.size() + uSize + i] = xv - m_functor(av, 0, 0);
			} else if(c == 0 && r != 0) {
				// First column a and b are considered to be 0
				// a and c  are zeros 

				b = m_f.u()[(r - 1) * m_f.u().cols() + c];
				errors[m_f.size() + i] = x - m_functor(0, b, 0);

				bv = m_f.v()[(r - 1) * m_f.v().cols() + c];
				errors[m_f.size() + uSize + i] = xv - m_functor(0, bv, 0);
			} else { 
				a = m_f.u()[r * m_f.u().cols() + c - 1];
				b = m_f.u()[(r - 1) * m_f.u().cols() + c];
				c2 = m_f.u()[(r - 1) * m_f.u().cols() + c - 1];
				errors[m_f.size() + i] = x - m_functor(a, b, c2);

				av = m_f.v()[r * m_f.v().cols() + c - 1];
				bv = m_f.v()[(r - 1) * m_f.v().cols() + c];
				cv = m_f.v()[(r - 1) * m_f.v().cols() + c - 1];
				errors[m_f.size() + uSize + i] = xv - m_functor(av, bv, cv);
			}

			i++;
		}
	}

	return errors;
}