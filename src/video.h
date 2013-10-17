#ifndef CAV_VIDEO_H
#define CAV_VIDEO_H

#include <string>
#include <fstream>
#include "frame.h"

class Video
{
public:
	Video();
	Video(int device);
	Video(const std::string& path);
	virtual ~Video();

	int rows();
	int cols();
	int fps();

	Frame* getFrame();


protected:
	std::ifstream m_stream;
	
	unsigned int m_rows;
	unsigned int m_cols;
	unsigned int m_fps;
	VideoFormat m_type;
};

#endif