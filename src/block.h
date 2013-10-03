#ifndef CAV_BLOCK_H
#define CAV_BLOCK_H

class Block
{
public:
	Block();
	Block(unsigned int rows, unsigned int cols);
	virtual ~Block();

	void setPoint(unsigned int row, unsigned int col, int value);
	void print();
protected:

private:
	unsigned int m_nRows;
	unsigned int m_nCols;

	int *m_buffer;
};

#endif