#ifndef CAV_VIDEO_H
#define CAV_VIDEO_H

#include <string>

#include "frame.h"

class Video
{
public:
	Video();
	Video(int device);
	Video(const std::string& path);
	virtual ~Video();

	Frame getFrame();

protected:

private:

};

#endif