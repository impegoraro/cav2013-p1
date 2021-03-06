/*
 * coder.h
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

#ifndef LIBCAV_CODER_H_
#define LIBCAV_CODER_H_

#include <utility>
#include "predictor.h"
#include "bitstream.h"

/**
 * @class Coder abstract class that serves as a base of each coder specialization like Golomb.
 * @author Ilan Pegoraro <impegoraro@ua.pt>
 * @author Luís Neves <luispneves@ua.pt>
 */
class Coder
{
public:

	Coder(const Predictor& pred, BitStream& bs)
		: m_pred(pred), m_bs(bs), m_elapsed(0)
	{
	}

	Coder(Predictor&& pred, BitStream& bs)
		: m_pred(pred), m_bs(bs), m_elapsed(0)
	{
	}
	
	virtual Predictor& predictor()
	{
		return m_pred;
	}
	virtual const Predictor& predictor() const
	{
		return m_pred;
	}
	
	virtual void encode() = 0;
	
	double getEncodeTime() const
	{
		return m_elapsed;
	}
protected:
	Predictor m_pred;
	BitStream& m_bs;

	double m_elapsed;
};

#endif