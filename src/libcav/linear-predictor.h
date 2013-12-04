/*
 * linear-predictor.h
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

#ifndef LIBCAV_LINEAR_PREDICTOR_H_
#define LIBCAV_LINEAR_PREDICTOR_H_

#include <functional>

#include "predictor.h"

/**
 * @class Implementation of linear predictors
 * @author Ilan Pegoraro (impegoraro@ua.pt)
 * @author Luís Neves (luispneves@ua.pt) 
 */
class LinearPredictor : public Predictor
{
public:
	/**
	 * @param f - const reference to a frame
	 */
	LinearPredictor(const Frame& f, int type = 0, float quantFactor = 1.0f);

	LinearPredictor(int index, float quantFactor, uint nRows, uint nCols, VideoFormat format, const std::vector<int>& errors);

	LinearPredictor(int index, float quantFactor, uint nRows, uint nCols, VideoFormat format, const std::vector<int>&& errors);

	/**
	 *
	 */
	LinearPredictor(LinearPredictor& p) : Predictor(p)
	{}

	/**
	 *
	 */
	LinearPredictor(LinearPredictor&& p) : Predictor(p)
	{}

	/*
	 * @param f - const reference to a frame
	 */
	//LinearPredictor(const Frame& f, std::function< int(int, int, int, int) >& functor);
	
	//virtual std::vector<int> predict() const ;
	//virtual Frame guess(const std::vector<int>& errors, uint nRows, uint nCols, VideoFormat format) const ;
protected: 
};


#endif