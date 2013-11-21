/*
 * golomb.h
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

#ifndef LIBCAV_GOLOMB_H_
#define LIBCAV_GOLOMB_H_

#include "coder.h"

/**
 * @class A class to implement the Golomb encoding
 * @author Ilan Pegoraro (impegoraro@ua.pt)
 * @author Luís Neves (luispneves@ua.pt) 
 */
class Golomb : public Coder
{
public:
	/**
	 * Constructs the golomb class
	 * @param errors - reference to the array of errors.
	 */
	Golomb(Predictor& pred, const std::string& fpath, uint m);

	/**
	 * Encodes the array of errors to a file stream
	 */
	virtual void encode();
	
	/**
	 * Recreates the array of errors from a file stream
	 */
	virtual std::vector<int> decode();

protected:
	/**
	 * Golomb's parameter in a power of two.
	 */
	uint m_m;
};


#endif