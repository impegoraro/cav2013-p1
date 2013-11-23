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

#include "frame.h"
#include "frame444.h"
#include "frame422.h"
#include "frame420.h"

#include "predictor.h"
#include "linear-predictor.h"

constexpr int N_FUNCTORS = 7;
/* One more argument for the nonlinear JPEG LS predictors  */
std::function<int (int, int, int, int) > m_defFunctors[N_FUNCTORS]{
	{[](int a, int b, int c, int d) -> int { return a; }},
	{[](int a, int b, int c, int d) -> int { return b; }},
	{[](int a, int b, int c, int d) -> int { return c; }},
	{[](int a, int b, int c, int d) -> int { return a + b - c; }},
	{[](int a, int b, int c, int d) -> int { return a + (b - c) / 2; }},
	{[](int a, int b, int c, int d) -> int { return b + (a - c) / 2; }},
	{[](int a, int b, int c, int d) -> int { return (a + b) / 2; }}
};


LinearPredictor::LinearPredictor(const Frame& f)
	: LinearPredictor(f, 0)
{
}

LinearPredictor::LinearPredictor(const Frame& f, int type)
	: Predictor(f, LINEAR_PREDICTOR, type, m_defFunctors[type])
{
	assert(type < N_FUNCTORS);
}

LinearPredictor::LinearPredictor(int index, uint nRows, uint nCols, VideoFormat format, const std::vector<int>& errors)
	: Predictor(LINEAR_PREDICTOR, index, m_defFunctors[index], nRows, nCols, format, errors)
{
}

LinearPredictor::LinearPredictor(int index, uint nRows, uint nCols, VideoFormat format, const std::vector<int>&& errors)
	: Predictor(LINEAR_PREDICTOR, index, m_defFunctors[index], nRows, nCols, format, errors)
{
}

// LinearPredictor::LinearPredictor(const Frame& f, std::function< int(int, int, int, int) >& functor)
// 	: Predictor(f, -1, functor)
// {
// }