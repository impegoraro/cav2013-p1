/*
 * predictor.h
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

#ifndef LIBCAV_PREDICTOR_H_
#define LIBCAV_PREDICTOR_H_

#include <vector>

#include "frame.h"

/**
 * @class Predictors
 * @author Ilan Pegoraro <impegoraro@ua.pt>
 * @author Luís Neves <luispneves@ua.pt>
 */
class Predictor
{
public:
	/**
	 * @param f - Constant reference to a frame
	 */
	Predictor(const Frame& f) : m_f(f)
	{
	}

	//virtual int operator[](uint index) = 0;
	virtual std::vector<int> predict() = 0;

	virtual Frame guess(const std::vector<int>& errors, uint nRows, uint nCols, VideoFormat format) = 0;
protected:
	const Frame& m_f;
};

class NonLinearPredictor : public Predictor
{
public:
	virtual std::vector<int> predict();
};


#endif