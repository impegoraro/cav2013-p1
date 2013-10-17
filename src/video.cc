#include <iostream>
#include <stdio.h>

#include "video-format.h"
#include "frame.h"
#include "video.h"

Video::Video()
{
}

/** Construct a Video object from a device.
	/param int - device number
 */
Video::Video(int number)
{
	std::string device("/dev/video");
	device += number;
	m_stream.open(device);
	m_cols = 720;
	m_rows = 480;
	m_fps = 25;
	m_type = YUV_444;
}

/** Construct a Video object from a file.	
	/param string - file path of the  
 */
Video::Video(const std::string& fpath)
	: m_stream(fpath)
{
	int type;
	if(!m_stream.good())
		throw FileNotFoundException();

	m_stream>> m_cols>> m_rows>> m_fps>> type;
	switch(type) {
		case 444:
			m_type = YUV_444;
		break;
		case 422:
			m_type = YUV_422;
		break;
		case 420:
			m_type = YUV_420;
		break;
		default:
		throw int();
	}
}

Video::~Video()
{
	m_stream.close();
}

Frame* Video::getFrame()
{
	Frame *f = new Frame(m_rows, m_cols);
	unsigned char buffer[m_cols * m_rows * 3];
	
	if(m_stream.eof())
		throw FileNotFoundException();
	
	int y, u, v;
	m_stream.read((char*)buffer, (m_cols * m_rows * 3));
	for(int i = 0 ; i < m_rows * m_cols * 3 ; i += 3)
	{
		/* Accessing to planar infor */
		y = buffer[i / 3]; 
		u = buffer[(i / 3) + (m_rows * m_cols)]; 
		v = buffer[(i / 3) + (m_rows * m_cols) * 2];
		f->y()[i / 3] = y;
		f->u()[i / 3] = u;
		f->v()[i / 3] = v;
	}
	return f;
}

int Video::rows()
{
	return m_rows;
}
int Video::cols()
{
	return m_cols;
}
int Video::fps()
{
	return m_fps;
}

/*
	std::ifstream stream(path);
	int cols, rows, type;
	int y, u, v;

	if(!stream.good())
		throw FileNotFoundException();

	stream>>m_cols>>m_rows>>m_fps>>m_type;
	unsigned char buffer[cols * rows * 3];
	
	while(stream.good()) {
		stream.read((char*)buffer, (cols * rows * 3));

		for(int i = 0 ; i < rows * cols * 3 ; i += 3)
		{ 
			Frame f;
			
			switch(type) {
			case 444:
				f = new Frame(rows, cols);
				m_type = YUV_444;
		 		break;
			case 422: {
				f = new Frame422(rows, cols);
				cols /= 2;
				m_type = YUV_422;
				break;
			}
			default:
				throw InvalidVideoTypeException();
			}
			/* Accessing to planar infor 
			y = buffer[i / 3]; 
			u = buffer[(i / 3) + (rows * cols)]; 
			v = buffer[(i / 3) + (rows * cols) * 2];
			f.y()[i / 3] = y;
			f.u()[i / 3] = u;
			f.v()[i / 3] = v;

			m_frames.push_back(std::move(f));
		}
	}
	stream.close();

*/