/*
 * subblock.cc
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
#include <algorithm>
#include <assert.h>
#include <cstring>
#include <memory>

#include "block.h"
#include "subblock.h"

SubBlock::SubBlock(uint begin, uint rows, uint cols, const Block& block)
	: Block(), m_begin(begin)
{
	assert(begin + rows * cols <= block.rows() * block.cols());
}

int& SubBlock::operator[](int index)
{
	assert(m_begin + index < m_nRows * m_nCols);
	return Block::operator[](m_begin + index);
}
