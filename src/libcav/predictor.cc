/*
 * predictor.cc
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
#include <vector>
#include <string>
#include <assert.h>


#include "predictor.h"
#include "frame.h"
#include "frame444.h"
#include "frame422.h"
#include "frame420.h"

std::vector<int> Predictor::predict(const Frame& f) const 
{
	uint uSize = (f.u().rows() * f.u().cols()), i(0);
	std::vector<int> errors(f.size() + uSize * 2);
	int x, xv;
	int a, b, c2, av, bv, cv;
	Frame tmp(f);

	// Predicting the Y component
	for(uint r = 0; r < tmp.rows(); r++) {
		for(uint c = 0; c < tmp.cols(); c++) {
			x = tmp.y()[r * tmp.cols() + c];
			if(c == 0 && r == 0){
				// First pixel at F[0][0], all values are considered to be 0
				errors[i] = x;
			} else if (r == 0) {
				// First row b and c are considered to be 0 (no previous value)
				a = tmp.y()[c - 1];
				// b and c2 are zero 
				errors[i] = (x - m_functor(a, 0, 0)) / m_quantFactorY;
				tmp.y()[r * tmp.y().cols() + c] = (errors[i]  * m_quantFactorY) + m_functor(a, 0, 0);
			} else if(c == 0) {
				// First column a and b are considered to be 0
				// a and c  are zeros 

				b = tmp.y()[(r - 1) * tmp.cols() + c];
				errors[i] = (x - m_functor(0, b, 0)) / m_quantFactorY;
				tmp.y()[r * tmp.y().cols() + c] = (errors[i]  * m_quantFactorY) + m_functor(0, b, 0);
			} else { 
				a = tmp.y()[r * tmp.cols() + c - 1];
				b = tmp.y()[(r - 1) * tmp.cols() + c];
				c2 = tmp.y()[(r - 1) * tmp.cols() + c - 1];
				errors[i] = (x - m_functor(a, b, c2)) / m_quantFactorY;
				tmp.y()[r * tmp.y().cols() + c] = (errors[i]  * m_quantFactorY) + m_functor(a, b, c2);
			}
			i++;
		}
	}

	i = 0;
	// Predicting the U and V components
	for(uint r = 0; r < tmp.u().rows(); r++) {
		for(uint c = 0; c < tmp.u().cols(); c++) {
			x = tmp.u()[r * tmp.u().cols() + c];
			xv = tmp.v()[r * tmp.v().cols() + c];
			

			if(c == 0 && r == 0) {
				// First pixel at F[0][0], all values are considered to be 0
				errors[tmp.size() + i] = x;
				errors[tmp.size() + uSize + i] = xv;
			} else if (r == 0) {
				// First row b and c are considered to be 0 (no previous value)
				a = tmp.u()[c - 1];
				av = tmp.v()[c - 1];
				
				errors[tmp.size() + i] = (x - m_functor(a, 0, 0)) / m_quantFactorU;
				errors[tmp.size() + uSize + i] = (xv - m_functor(av, 0, 0)) / m_quantFactorV;

				tmp.u()[r * tmp.u().cols() + c] = (errors[tmp.size() + i]  * m_quantFactorU) + m_functor(a, 0, 0);
				tmp.v()[r * tmp.u().cols() + c] = (errors[tmp.size() + uSize + i]  * m_quantFactorV) + m_functor(av, 0, 0);
			} else if(c == 0) {
				// First column a and b are considered to be 0
				// a and c  are zeros 

				b = tmp.u()[(r - 1) * tmp.u().cols()];
				errors[tmp.size() + i] = (x - m_functor(0, b, 0)) / m_quantFactorU;

				bv = tmp.v()[(r - 1) * tmp.v().cols()];
				errors[tmp.size() + uSize + i] = (xv - m_functor(0, bv, 0)) / m_quantFactorV;

				tmp.u()[r * tmp.u().cols() + c] = (errors[tmp.size() + i]  * m_quantFactorU) + m_functor(0, b, 0);
				tmp.v()[r * tmp.u().cols() + c] = (errors[tmp.size() + uSize + i]  * m_quantFactorV) + m_functor(0, bv, 0);
			} else { 
				a = tmp.u()[r * tmp.u().cols() + c - 1];
				b = tmp.u()[(r - 1) * tmp.u().cols() + c];
				c2 = tmp.u()[(r - 1) * tmp.u().cols() + c - 1];
				errors[tmp.size() + i] = (x - m_functor(a, b, c2)) / m_quantFactorU;

				av = tmp.v()[r * tmp.v().cols() + c - 1];
				bv = tmp.v()[(r - 1) * tmp.v().cols() + c];
				cv = tmp.v()[(r - 1) * tmp.v().cols() + c - 1];
				errors[tmp.size() + uSize + i] = (xv - m_functor(av, bv, cv)) / m_quantFactorV;

				tmp.u()[r * tmp.u().cols() + c] = (errors[tmp.size() + i]  * m_quantFactorU) + m_functor(a, b, c2);
				tmp.v()[r * tmp.u().cols() + c] = (errors[tmp.size() + uSize + i]  * m_quantFactorV) + m_functor(av, bv, cv);
			}

			i++;
		}
	}

	return errors;
}

/* Predictor */

Frame* Predictor::guess() const 
{
	Frame *f = Frame::create(m_nRows, m_nCols, m_format);
	uint uSize = (f->u().rows() * f->u().cols()), i(0);
	int e, ev;
	int a, b, c2, av, bv, cv;

	assert(m_errors.size() == m_nRows * m_nCols + uSize * 2);
	// Guessing the Y component
	for(uint r = 0; r < f->rows(); r++) {
		for(uint c = 0; c < f->cols(); c++) {
			e = m_errors[i];
			
			if(c == 0 && r == 0) {
				// First pixel at F[0][0], all values are considered to be 0
				
				f->y()[r * f->cols() + c] = e;
			} else if (r == 0) {
				// First row b and c are considered to be 0 (no previous value)

				a = f->y()[c - 1];
				//std::cout<< "a = "<< a<< " e = "<<e <<std::endl;
				// b and c2 are zero 
				f->y()[r + c] = (e  * m_quantFactorY) + m_functor(a, 0, 0);
			} else if(c == 0) {
				// First column a and b are considered to be 0
				// a and c  are zeros 

				b = f->y()[(r - 1) * f->cols()];
				f->y()[r * f->cols() + c] = (e  * m_quantFactorY) + m_functor(0, b, 0);
			} else { 
				a = f->y()[r * f->cols() + c - 1];
				b = f->y()[(r - 1) * f->cols() + c];
				c2 = f->y()[(r - 1) * f->cols() + c - 1];
				f->y()[r * f->cols() + c] = (e  * m_quantFactorY) + m_functor(a, b, c2);
			}
			i++;
		}
	}

	i = 0;
	// Guessing the U and V components
	for(uint r = 0; r < f->u().rows(); r++) {
		for(uint c = 0; c < f->u().cols(); c++) {
			e = m_errors[f->size() + i];
			ev = m_errors[f->size()  + uSize + i];

			if(c == 0 && r == 0) {
				// First pixel at F[0][0], all values are considered to be 0
				f->u()[0] = e;
				f->v()[0] = ev;
			} else if (r == 0) {
				// First row b and c are considered to be 0 (no previous value)
				a = f->u()[c - 1];
				av = f->v()[c - 1];
				
				f->u()[i] = (e  * m_quantFactorU) + m_functor(a, 0, 0);
				f->v()[i] = (ev  * m_quantFactorV) + m_functor(av, 0, 0);
			} else if(c == 0) {
				// First column a and b are considered to be 0
				// a and c  are zeros 

				b = f->u()[(r - 1) * f->u().cols()];
				f->u()[i] = (e  * m_quantFactorU) + m_functor(0, b, 0);

				bv = f->v()[(r - 1) * f->v().cols()];
				f->v()[i] = (ev  * m_quantFactorV) + m_functor(0, bv, 0);

			} else { 
				a = f->u()[r * f->u().cols() + c - 1];
				b = f->u()[(r - 1) * f->u().cols() + c];
				c2 = f->u()[(r - 1) * f->u().cols() + c - 1];
				f->u()[i] = (e  * m_quantFactorU) + m_functor(a, b, c2);

				av = f->v()[r * f->v().cols() + c - 1];
				bv = f->v()[(r - 1) * f->v().cols() + c];
				cv = f->v()[(r - 1) * f->v().cols() + c - 1];
				f->v()[i] = (ev  * m_quantFactorV) + m_functor(av, bv, cv);
			}
			i++;
		}
	}



	return f;
}