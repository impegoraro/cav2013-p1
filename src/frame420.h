#ifndef FRAME420_H_
#define FRAME420_H_

#include "frame.h"
#include "video-format.h"


/**
 * Intialiazes frame for 420. The components U and V have half the rows and columns of Y.
 */
class Frame420 : public Frame
{
public:
	/**
	 * Constructs a frame in the YUV422 format, with half the rows and columns for the U and V components.
	 * @param nRows - Number of rows.
	 * @param nCols - Number of columns.
	 */
	Frame420(unsigned int nRows, unsigned int nCols);

	/**
	 * Overrided method from the frame base class. This methods converts a frame in YUV444 format to the new format.
	 * @param dest - VideoFormat for the new Frame.
	 */
	virtual Frame convert(VideoFormat dest);
protected:
};

#endif