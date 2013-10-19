#ifndef CAV_BLOCK_H
#define CAV_BLOCK_H

#include "exceptions/cav-exceptions.h"

class Block
{
public:
	//Block();
	Block(unsigned int rows, unsigned int cols);
	Block(Block&& b);
	Block(const Block& b);

	virtual ~Block();

	void setPoint(unsigned int row, unsigned int col, int value);
	int getPoint(unsigned int row, unsigned int col);

	Block* dup();

	unsigned int rows(void);
	unsigned int cols(void);
	unsigned int size(void);

	Block& operator=(const Block& rhs);
	Block& operator=(const char *rhs);
	Block& operator=(Block&& rhs);
	bool operator==(const Block& rhs);
	bool operator==(const char* rhs);
	int& operator[](unsigned int index);
	int operator[](unsigned int index) const;

	void print();
private:
	unsigned int m_nRows;
	unsigned int m_nCols;

	int *m_buffer;

};

#endif
