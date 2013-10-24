#ifndef CAV_VIDEO_H
#define CAV_VIDEO_H

#include <string>
#include <fstream>
#include <opencv2/opencv.hpp>
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
	void display(int playing = true);

protected:
	std::fstream m_stream;
	
	uint m_rows;
	uint m_cols;
	uint m_fps;
	bool m_fromCam;
	cv::VideoCapture m_video;
	VideoFormat m_type;
};

#endif