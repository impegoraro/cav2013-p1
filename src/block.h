#ifndef CAV_BLOCK_H
#define CAV_BLOCK_H

#include "exceptions/cav-exceptions.h"

class Block
{
public:
	Block(uint rows, uint cols);
	Block(Block&& b);
	Block(const Block& b);

	virtual ~Block();

	void setPoint(uint row, uint col, int value);
	int getPoint(uint row, uint col);

	Block* dup();

	uint rows(void);
	uint cols(void);
	uint size(void);

	Block& operator=(const Block& rhs);
	Block& operator=(const char *rhs);
	Block& operator=(Block&& rhs);
	bool operator==(const Block& rhs);
	bool operator==(const char* rhs);
	int& operator[](uint index);
	int operator[](uint index) const;

	Block getSubBlock(uint begin, uint rows, uint cols);
	void setSubBlock(uint begin, Block& b);
	
	void print();


protected:
	Block();
	uint m_nRows;
	uint m_nCols;
	bool m_shouldClean;

	int *m_buffer;

};

#endif
