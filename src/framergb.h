#ifndef CAV_FRAMERGB_H_
#define CAV_FRAMERGB_H_

#include "frame.h"


/**
 * Intialiazes frame for RGB format. Same size for components R, G and B.
 */
class FrameRGB : public Frame
{
public:
	/**
	 * Constructs a frame in the RGB format.
	 * @param nRows - Number of rows.
	 * @param nCols - Number of columns.
	 */
	FrameRGB(unsigned int nRows, unsigned int nCols);
	/**
	 * Overrided method from the frame base class. This methods converts a frame in RGB format to the new format.
	 * @param format - VideoFormat for the new Frame.
	 */
	virtual Frame convert(VideoFormat format);
};


#endif