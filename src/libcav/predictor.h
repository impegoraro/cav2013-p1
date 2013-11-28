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
#include <functional>


#include "frame.h"


enum PredictorType
{
	LINEAR_PREDICTOR = 1,
	NONLINEAR_PREDICTOR = 2
};

/**
 * @class Predictors
 * @author Ilan Pegoraro <impegoraro@ua.pt>
 * @author Luís Neves <luispneves@ua.pt>
 */
class Predictor
{
public:
	Predictor()
		: m_functor(), m_errors(), m_functorIndex(0), m_nRows(0), m_nCols(0), m_format(YUV_444), m_type(LINEAR_PREDICTOR)
	{ }
	/**
	 * @param f - Constant reference to a frame
	 */
	Predictor(const Frame& f, PredictorType type, int index, std::function<int(int, int, int, int)> functor) 
		: m_functor(functor), m_functorIndex(index), m_nRows(f.rows()), m_nCols(f.cols()), m_format(f.getFormat()), m_type(type)
	{
		m_errors = predict(f);
	}

	Predictor(PredictorType type, int index, std::function<int(int, int, int, int)> functor, uint nRows, uint nCols, VideoFormat format, const std::vector<int>& errors)
		: m_functor(functor), m_errors(errors), m_functorIndex(index), m_nRows(nRows), m_nCols(nCols), m_format(format), m_type(type)
	{
	}

	Predictor(PredictorType type, int index, std::function<int(int, int, int, int)> functor, uint nRows, uint nCols, VideoFormat format, const std::vector<int>&& errors)
		: m_functor(functor), m_errors(errors), m_functorIndex(index), m_nRows(nRows), m_nCols(nCols), m_format(format), m_type(type)
	{
	}

	Predictor(const Predictor& p)
		: m_functor(p.m_functor), m_errors(p.m_errors), m_functorIndex(p.m_functorIndex), m_nRows(p.m_nRows), m_nCols(p.m_nCols), m_format(p.m_format), m_type(p.m_type)
	{
	}

	Predictor(Predictor&& p)
		: m_functor(p.m_functor), m_errors(p.m_errors), m_functorIndex(p.m_functorIndex), m_nRows(p.m_nRows), m_nCols(p.m_nCols), m_format(p.m_format), m_type(p.m_type)
	{
	}

	//virtual void set_predictor(uint predictor) = 0;

	uint rows() const
	{
		return m_nRows;
	}

	uint cols() const
	{
		return m_nCols;
	}

	VideoFormat getFormat() const
	{
		return m_format;
	}

	uint frameSize()
	{
		uint size{0};
		switch(m_format) {
		case YUV_422: 
			size = m_nCols * m_nRows + ((m_nCols / 2) * m_nRows) * 2; 
			break;
		case YUV_420: 
			size = m_nCols * m_nRows + ((m_nCols / 2)  * (m_nRows / 2)) * 2; 
			break;
		case YUV_444:
		default: size = m_nCols * m_nRows * 3; break;

		}

		return size;
	}

	int index() const
	{
		return m_functorIndex;
	}
	Frame* guess()  const;

	const std::vector<int>& errors() const
	{
		return m_errors;
	}

	inline PredictorType type()
	{
		return m_type;
	}
protected:
	std::function< int(int, int, int, int) > m_functor;
	std::vector<int> m_errors;
	/**
	 * Index of the function used to predict.
	 * A negative number means is used defined and not one of the library defaults.
	 */
	int m_functorIndex;
	uint m_nRows;
	uint m_nCols;
	VideoFormat m_format;
	PredictorType m_type;

	std::vector<int> predict(const Frame& f) const;
};

#endif