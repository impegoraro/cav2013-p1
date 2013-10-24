#ifndef FRAME444_H_
#define FRAME444_H_

#include "frame.h"
#include "video-format.h"

/**
 * Intialiazes frame for YUV444 format. Same size for components Y, U and V.
 */
class Frame444 : public Frame
{
public:
	/**
	 * Constructs a frame in the YUV444 format.
	 * @param nRows - Number of rows.
	 * @param nCols - Number of columns.
	 */
	Frame444(unsigned int nRows, unsigned int nCols);

	/**
	 * Overrided method from the frame base class. This methods converts a frame in YUV444 format to the new format.
	 * @param dest - VideoFormat for the new Frame.
 	 */
	virtual Frame convert(VideoFormat dest);
};

#endif