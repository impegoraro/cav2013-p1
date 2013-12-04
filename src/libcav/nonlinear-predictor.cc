/*
 * nonlinear-predictor.cc
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

#include "frame.h"
#include "frame444.h"
#include "frame422.h"
#include "frame420.h"

#include "predictor.h"
#include "nonlinear-predictor.h"

constexpr int N_FUNCTORS = 1;

/* One more argument for the nonlinear JPEG LS predictors  */
static std::function<int (int, int, int) > m_defFunctors[N_FUNCTORS]{
	{[] (int a, int b, int c) {
		int res;
		if(c >= std::max(a, b))
			res = std::min(a, b);
		else if(c <= std::min(a, b))
			res = std::max(a, b);
		else
			res = a + b -c;

		return res;
	}}
};

NonLinearPredictor::NonLinearPredictor(Frame& f, float quantFactor)
	: Predictor(f, NONLINEAR_PREDICTOR, quantFactor, 0, m_defFunctors[0])
{
}

NonLinearPredictor::NonLinearPredictor(float quantFactor, uint nRows, uint nCols, VideoFormat format, const std::vector<int>& errors)
	: Predictor(NONLINEAR_PREDICTOR, quantFactor, 0, m_defFunctors[0], nRows, nCols, format, errors)
{
}

NonLinearPredictor::NonLinearPredictor(float quantFactor, uint nRows, uint nCols, VideoFormat format, const std::vector<int>&& errors)
	: Predictor(NONLINEAR_PREDICTOR, quantFactor, 0, m_defFunctors[0], nRows, nCols, format, errors)
{
}