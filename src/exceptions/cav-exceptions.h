#ifndef CAV_EXCEPTIONS_H_
#define CAV_EXCEPTIONS_H_

#include <exception>

class InvalidDimensionException : public std::exception
{
public:
	virtual const char* what()
	{
		return "Dimentions are wrong.";
	}	
};

class IndexOutOfBoundsException : public std::exception
{
public:
	virtual const char* what()
	{
		return "index out of bounds of the array.";
	}
};

#endif