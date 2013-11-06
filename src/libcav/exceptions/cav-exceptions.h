/*
 * cav-exceptions.h
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

#ifndef CAV_EXCEPTIONS_H_
#define CAV_EXCEPTIONS_H_

#include <exception>
/**
 * Exception for when the dimensions given are invalid
 */
class InvalidDimensionException : public std::exception
{
public:
	/**
	 * Prints a user friendly message describing the error.
	*/
	virtual const char* what()
	{
		return "Dimentions are wrong.";
	}	
};

/**
 * Exception for when the array is accessed with an invalid index
 */
class IndexOutOfBoundsException : public std::exception
{
public:
	/**
	 * Prints a user friendly message describing the error.
	*/
	virtual const char* what()
	{
		return "index out of bounds of the array.";
	}
};

/**
 * Exception for when file path given could not be located 
 */
class FileNotFoundException : public std::exception
{
public:
	/**
	 * Prints a user friendly message describing the error.
	*/
	virtual const char* what()
	{
		return "File not found.";
	}
};

/**
 * Exception for when the video type is invalid
 */
class InvalidVideoTypeException : public std::exception
{
public:
	/**
	 * Prints a user friendly message describing the error.
	*/
	virtual const char* what()
	{
		return "Video must be a YUV 444, YUV 422 or YUV420.";
	}
};

/**
 * Exception for when the file is not open to be read or written on
 */
class FileNotOpenException : public std::exception
{
public:
	/**
	 * Prints a user friendly message describing the error.
	*/
	virtual const char* what()
	{
		return "File is not open.";
	}
};

/**
 * Exception for when the video reaches the end
 */
class VideoEndedException : public std::exception
{
public:
	/**
	 * Prints a user friendly message describing the error.
	*/
	virtual const char* what()
	 {
		return "Video has ended.";
	 }
};
#endif