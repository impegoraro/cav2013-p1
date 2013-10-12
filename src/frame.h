#ifndef CAV_FRAME_H
#define CAV_FRAME_H

#include <string>

#include "block.h"
#include "video-format.h"


class Frame
{
public:
	Frame(unsigned int nRows, unsigned int nCols);
	Frame(const Frame& f);
	Frame(Frame&& f);
	virtual ~Frame();

	void setBlock(const Block &y, const Block &u, const Block &v);
	void getBlock(Block& y, Block& u, Block& v);
	
	void setPixel(int row, int col, int y, int u, int v);
	void getPixel(int row, int col, int& y, int& u, int& v);
	void getPixel(int pos, int& y, int& u, int& v);
	void display();

	int rows();
	int cols();

	Frame& operator=(const Frame& rhs);
	virtual Frame convert();

	Block& y();
	Block& u();
	Block& v();

protected:
	int m_uvRows;
	int m_uvCols;

	Frame(unsigned int rows, unsigned int cols, unsigned int uvRows, unsigned int uvCols);
	Block* m_y;
	Block* m_u;
	Block* m_v;
};

#endif