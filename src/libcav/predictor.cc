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

	// Predicting the Y component
	for(uint r = 0; r < f.rows(); r++) {
		for(uint c = 0; c < f.cols(); c++) {
			x = f.y()[r * f.cols() + c];
			if(c == 0 && r == 0){
				// First pixel at F[0][0], all values are considered to be 0
				errors[i] = x;
			} else if (r == 0) {
				// First row b and c are considered to be 0 (no previous value)
				a = f.y()[c - 1];
				// b and c2 are zero 
				errors[i] = x - m_functor(a, 0, 0, 0);
			} else if(c == 0) {
				// First column a and b are considered to be 0
				// a and c  are zeros 

				b = f.y()[(r - 1) * f.cols() + c];
				errors[i] = x - m_functor(0, b, 0, 0);
			} else { 
				a = f.y()[r * f.cols() + c - 1];
				b = f.y()[(r - 1) * f.cols() + c];
				c2 = f.y()[(r - 1) * f.cols() + c - 1];
				errors[i] = x - m_functor(a, b, c2, 0);
			}

			i++;
		}
	}

	i = 0;
	// Predicting the U and V components
	for(uint r = 0; r < f.u().rows(); r++) {
		for(uint c = 0; c < f.u().cols(); c++) {
			x = f.u()[r * f.u().cols() + c];
			xv = f.v()[r * f.v().cols() + c];
			

			if(c == 0 && r == 0) {
				// First pixel at F[0][0], all values are considered to be 0
				errors[f.size() + i] = x;
				errors[f.size() + uSize + i] = xv;
			} else if (r == 0) {
				// First row b and c are considered to be 0 (no previous value)
				a = f.u()[c - 1];
				av = f.v()[c - 1];
				
				errors[f.size() + i] = x - m_functor(a, 0, 0, 0);
				errors[f.size() + uSize + i] = xv - m_functor(av, 0, 0, 0);
			} else if(c == 0) {
				// First column a and b are considered to be 0
				// a and c  are zeros 

				b = f.u()[(r - 1) * f.u().cols()];
				errors[f.size() + i] = x - m_functor(0, b, 0, 0);

				bv = f.v()[(r - 1) * f.v().cols()];
				errors[f.size() + uSize + i] = xv - m_functor(0, bv, 0, 0);
			} else { 
				a = f.u()[r * f.u().cols() + c - 1];
				b = f.u()[(r - 1) * f.u().cols() + c];
				c2 = f.u()[(r - 1) * f.u().cols() + c - 1];
				errors[f.size() + i] = x - m_functor(a, b, c2, 0);

				av = f.v()[r * f.v().cols() + c - 1];
				bv = f.v()[(r - 1) * f.v().cols() + c];
				cv = f.v()[(r - 1) * f.v().cols() + c - 1];
				errors[f.size() + uSize + i] = xv - m_functor(av, bv, cv, 0);
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
				f->y()[r + c] = e + m_functor(a, 0, 0, 0);
			} else if(c == 0) {
				// First column a and b are considered to be 0
				// a and c  are zeros 

				b = f->y()[(r - 1) * f->cols()];
				f->y()[r * f->cols() + c] = e + m_functor(0, b, 0, 0);
			} else { 
				a = f->y()[r * f->cols() + c - 1];
				b = f->y()[(r - 1) * f->cols() + c];
				c2 = f->y()[(r - 1) * f->cols() + c - 1];
				f->y()[r * f->cols() + c] = e + m_functor(a, b, c2, 0);
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
				
				f->u()[i] = e + m_functor(a, 0, 0, 0);
				f->v()[i] = ev + m_functor(av, 0, 0, 0);
			} else if(c == 0) {
				// First column a and b are considered to be 0
				// a and c  are zeros 

				b = f->u()[(r - 1) * f->u().cols()];
				f->u()[i] = e + m_functor(0, b, 0, 0);

				bv = f->v()[(r - 1) * f->v().cols()];
				f->v()[i] = ev + m_functor(0, bv, 0, 0);

			} else { 
				a = f->u()[r * f->u().cols() + c - 1];
				b = f->u()[(r - 1) * f->u().cols() + c];
				c2 = f->u()[(r - 1) * f->u().cols() + c - 1];
				f->u()[i] = e + m_functor(a, b, c2, 0);

				av = f->v()[r * f->v().cols() + c - 1];
				bv = f->v()[(r - 1) * f->v().cols() + c];
				cv = f->v()[(r - 1) * f->v().cols() + c - 1];
				f->v()[i] = ev + m_functor(av, bv, cv, 0);
			}
			i++;
		}
	}



	return f;
}