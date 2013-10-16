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

	unsigned int rows();
	unsigned int cols();

	Frame& operator=(const Frame& rhs);
	Frame& operator=(Frame&& rhs);

	Block& y();
	Block& u();
	Block& v();

	static Frame* create_from_file(std::string& path);
protected:
	unsigned int m_uvRows;
	unsigned int m_uvCols;

	Block* m_y;
	Block* m_u;
	Block* m_v;

	// Dumb constructor that doesnt initializes anything. 
	// Its purpose is to help the parse function without using the heap.
	Frame();
	Frame(unsigned int rows, unsigned int cols, unsigned int uvRows, unsigned int uvCols);
	virtual Frame convert();
};

#endif