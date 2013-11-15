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
#include <vector>

/**
 * @class Coder abstract class that serves as a base of each coder specialization like Golomb.
 * @author Ilan Pegoraro <impegoraro@ua.pt>
 * @author Luís Neves <luispneves@ua.pt>
 */
class Coder
{
public:
	
	Coder(const std::vector<int>& errors, const std::string& fpath)
		: m_errors(std::move(errors)), m_fpath(fpath)
	{
	}

	virtual void encode() = 0;

	virtual std::vector<int> decode() = 0;
protected:
	const std::vector<int>& m_errors;
	std::string m_fpath;
};

#endif