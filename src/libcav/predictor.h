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



enum PredictorType
{
	LINEAR_PREDICTOR = 1,
	NONLINEAR_PREDICTOR
};

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
	Predictor(const Frame& f, uint index) : m_f(f), m_functorIndex(index)
	{
	}
	/**
	 * Get the frame the predictor's working on.
	 * @return constant reference  to the Frame
	 */
	virtual const Frame& frame() const
	{
		return m_f;
	}
	virtual std::vector<int> predict() const = 0;
	//virtual void set_predictor(uint predictor) = 0;

	int index() const
	{
		return m_functorIndex;
	}
	virtual Frame guess(const std::vector<int>& errors, uint nRows, uint nCols, VideoFormat format)  const = 0;
protected:
	const Frame& m_f;
	/**
	 * Index of the function used to predict.
	 * A negative number means is used defined and not one of the library defaults.
	 */
	int m_functorIndex;
};

class NonLinearPredictor : public Predictor
{
public:
	virtual std::vector<int> predict();
};


#endif