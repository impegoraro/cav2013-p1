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
	Video(const std::string& fpath, uint rows, uint cols, uint fps, VideoFormat type);
	virtual ~Video();

	int rows();
	int cols();
	int fps();
	VideoFormat format();

	Frame* getFrame();
	void putFrame(Frame& f);

	void reset();
	void display();

protected:
	std::fstream m_stream;
	
	unsigned int m_rows;
	unsigned int m_cols;
	unsigned int m_fps;
	VideoFormat m_type;
};

#endif