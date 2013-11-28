#ifndef _BITSTREAM_
#define _BITSTREAM_

#include <string>
#include <stdio.h>

#include "cav-header.h"
#include "predictor.h"

/* 
 *@class BitStream to read and write bits on a file.
 *@author Antonio J. R. Neves (an@ua.pt)
 */
class BitStream
{
public:
	/** Constructor 
		\param fileName The string containing the name of the encoded file.
		\param mode The string wb or rb.
		\param header - Coding header previously allocated */
	BitStream(const char *fileName, char *mode, CAVHeader* header);

	BitStream(const BitStream& bs)
		: buffer(bs.buffer), pos(bs.pos), fp(bs.fp), mode(bs.mode), m_fpath(bs.m_fpath)
	{ }

	BitStream(BitStream&& bs)
		: buffer(bs.buffer), pos(bs.pos), fp(bs.fp), mode(bs.mode), m_fpath(bs.m_fpath)
	{
		bs.fp = nullptr;
	}

	/** Destructor */ 
	~BitStream();
	/** Method to write a bit in the bitstream.
		\param value The bit to be written. */
	void writeBit(int value);
	/** Method to read a bit from the bitstream.
		\return 0 or 1 depending on the bit. */
	int readBit();
	/** Method to write several bits into the bitstream.
		\param nb The number of bits to write in the bitstream.
		\param value The value to be written*/
	void writeNBits(int value, int nb);
	/** Method to read several bits from the bitstream.
		\param nb The number of bits to read from the bitstream.
		\return The value contained in the n bits. */
	int readNBits(int nb);

	/** Gets the header 
	  \return File's header. */
	CAVHeader* getHeader()
	{
		return &m_header;
	}
	const std::string& getPath() const
	{
		return m_fpath;
	}
	/** Method to write the header.
		\param header The coding header. */
	void writeHeader(const CAVHeader* header);
	/** Reads the header from the bitstream.
	 	\param header Where to store the coding header.*/
	void readHeader(CAVHeader* header);

protected:

	/** Buffer to store the block data (bSize x bSize). */
	unsigned char buffer;

	/** Position on the buffer where the bit is being accessed. */
	int pos;

	/** File pointer */
	FILE *fp;

	/** Read or Write mode */
	int mode;

	/** Path to the file*/
	std::string m_fpath;

	CAVHeader m_header;
};

#endif
