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