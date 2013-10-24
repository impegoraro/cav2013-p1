#ifndef FRAME422_H_
#define FRAME422_H_

#include "frame.h"
#include "video-format.h"


/**
 * Intialiazes frame for 422. The components U and V have half the columns of Y.
 */
class Frame422 : public Frame
{
public:
	/**
	 * Constructs a frame with half the columns for the U and V components (YUV422 format).
	 * @param nRows - Number of rows.
	 * @param nCols - Number of columns.
	 */
	Frame422(unsigned int nRows, unsigned int nCols);

	/**
	 * Overrided method from the frame base class. This methods converts a frame in YUV422 format to the new format.
	 * @param dest - VideoFormat for the new Frame.
	 */
	virtual Frame convert(VideoFormat dest);
protected:
};

#endif