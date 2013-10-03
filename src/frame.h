#ifndef CAV_FRAME_H
#define CAV_FRAME_H

#include "block.h"
#include "video-format.h"
class Frame
{
public:
	Frame(unsigned int nRows, unsigned int nCols, VideoFormat format);
	virtual ~Frame();

protected:

private:
	VideoFormat m_format;

	int m_uvRows;
	int m_uvCols;

	Block* m_y;
	Block* m_u;
	Block* m_v;
};

#endif