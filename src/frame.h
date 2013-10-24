#ifndef CAV_FRAME_H
#define CAV_FRAME_H

#include <string>

#include "block.h"
#include "video-format.h"


class Frame
{
public:
	Frame();
	Frame(unsigned int nRows, unsigned int nCols);
	Frame(const Frame& f);
	Frame(Frame&& f);
	virtual ~Frame();

	VideoFormat getFormat();

	void setBlock(const Block &y, const Block &u, const Block &v);
	void getBlock(Block& y, Block& u, Block& v);
	
	void setPixel(uint row, uint col, int y, int u, int v);
	void getPixel(uint row, uint col, int& y, int& u, int& v);
	void getPixel(uint pos, int& y, int& u, int& v);
	void display();

	void setBlackWhite();
	void setInvertColors();
	void changeLuminance(float factor);

	uint rows();
	uint cols();

	Frame& operator=(const Frame& rhs);
	Frame& operator=(Frame&& rhs);

	Block& y();
	Block& u();
	Block& v();

	virtual void write(const std::string& path);

	virtual Frame convert(VideoFormat dest);
	static Frame* create_from_file(const std::string& path);
protected:
	uint m_uvRows;
	uint m_uvCols;

	Block* m_y;
	Block* m_u;
	Block* m_v;

	VideoFormat m_format;
	// Dumb constructor that doesnt initializes anything. 
	// Its purpose is to help the parse function without using the heap.
	Frame(uint rows, uint cols, uint uvRows, uint uvCols, VideoFormat format);
};

#endif