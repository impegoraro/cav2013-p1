#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <opencv2/opencv.hpp>

#include "video-format.h"
#include "frame.h"
#include "framergb.h"
#include "frame444.h"
#include "frame422.h"
#include "frame420.h"
#include "video.h"

using namespace cv;

Video::Video() : Video(0)
{
}

/** Construct a Video object from a device.
 * /param int - device number
 */
Video::Video(int number)
{
	std::string device("/dev/video");
	VideoCapture cap = VideoCapture(0);
	
	device += number;
	m_stream.open(device);
	m_fps = 25;
	m_cols= (int)cap.get(CV_CAP_PROP_FRAME_WIDTH);
	m_rows = (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	m_type = RGB;
}

/** Construct a Video object from a file.	
 * /param string - file path of the  
 */
Video::Video(const std::string& fpath)
	: m_stream(fpath)
{
	int type;
	char c; // to get the newline
	if(!m_stream.good())
		throw FileNotFoundException();

	m_stream>> m_cols>> m_rows>> m_fps>> type>> c;
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

/** Construct a Video object for writing.	
 * /param string - file path of the 
 *
 */
Video::Video(const std::string& fpath, uint rows, uint cols, uint fps, VideoFormat type)
	: m_stream(fpath), m_type(type), m_cols(cols), m_rows(rows), m_fps(fps)
{
	if(!m_stream.good())
		throw FileNotFoundException();

	m_stream<< m_cols<< " "<< m_rows<< " "<< m_fps<< " "<< type<< std::endl;

}

/**
 * Video Destructor.
 * Cleans the internal state of the class, closes the stream.
 */
Video::~Video()
{
	m_stream.close();
}

Frame* Video::getFrame()
{
	Frame *f;
	unsigned char buffer[m_cols * m_rows * 3];
	int cols, rows;
	int size;
	if(m_stream.eof())
		throw VideoEndedException();
	
	switch(m_type) {
		case RGB:
			rows = m_rows;
			cols = m_cols;
			size = m_rows * m_cols * 3;
			f = new FrameRGB(m_rows, m_cols);
		break;
		case YUV_444:
			rows = m_rows;
			cols = m_cols;
			size = m_rows * m_cols * 3;
			f = new Frame444(m_rows, m_cols);
		break;
		case YUV_422:
			rows = m_rows;
			cols = m_cols / 2;
			size = m_rows * m_cols + m_rows * cols * 2;
			f = new Frame422(m_rows, m_cols);
		break;
		case YUV_420:
			rows = m_rows / 2;
			cols = m_cols / 2;
			size = m_rows * m_cols + rows * cols * 2;
			f = new Frame420(m_rows, m_cols);
		break;
	}
	int y, u, v;
	m_stream.read((char*)buffer, size);
	for(int i = 0 ; i < m_rows * m_cols * 3 ; i += 3)
	{
		/* Accessing to planar infor */
		y = buffer[i / 3];
		f->y()[i / 3] = y;
		if(m_type == RGB || m_type == YUV_444 || ((i/3) < f->u().size())) {
			u = buffer[(i / 3) + (m_rows * m_cols)]; 
			v = buffer[(i / 3) + (m_rows * m_cols + rows * cols)];
			f->u()[i / 3] = u;
			f->v()[i / 3] = v;
		}
	}
	return f;
}

void Video::putFrame(Frame& f)
{
	assert(f.getFormat() == m_type);
	unsigned char buffer[m_cols * m_rows * 3];
	int cols, rows;
	int size;
	
	switch(m_type) {
		case RGB:
			rows = m_rows;
			cols = m_cols;
			size = m_rows * m_cols * 3;
		break;
		case YUV_444:
			rows = m_rows;
			cols = m_cols;
			size = m_rows * m_cols * 3;
		break;
		case YUV_422:
			rows = m_rows;
			cols = m_cols / 2;
			size = m_rows * m_cols + m_rows * cols * 2;
		break;
		case YUV_420:
			rows = m_rows / 2;
			cols = m_cols / 2;
			size = m_rows * m_cols + rows * cols * 2;
		break;
	}

	for(int i = 0 ; i < m_rows * m_cols * 3 ; i += 3)
	{
		/* Accessing to planar infor */
		buffer[i / 3] = f.y()[i / 3];
		if(m_type == RGB || m_type == YUV_444 || ((i/3) < f.u().size())) {
			buffer[(i / 3) + (m_rows * m_cols)] = f.u()[i / 3]; 
			buffer[(i / 3) + (m_rows * m_cols + rows * cols)] = f.v()[i / 3];
		}
	}
	m_stream.write((char*)buffer, size);
}

/**
 * Gets the rows that each frame represents
 * /returns int - Number of rows
 */
int Video::rows()
{
	return m_rows;
}

/**
 * Gets the columns that each frame represents
 * /returns int - Number of columns
 */
int Video::cols()
{
	return m_cols;
}

/**
 * Gets the frames per second
 * /returns int - Number of frames per second
 */
int Video::fps()
{
	return m_fps;
}

/**
 * Gets the video's format.
 * /returns VideosFormat - Current video format.
 */
VideoFormat Video::format()
{
	return m_type;
}

/**
 * Sets the posistion of the file to the begining.
 */
void Video::reset()
{
	string tmp;
	if(!m_stream.is_open())
		throw FileNotOpenException();
	m_stream.clear();
	m_stream.seekg(0, std::ios_base::beg);
	getline(m_stream, tmp); // skip the header
}

/**
 * Displays the video in a window.
 */
void Video::display()
{
	Frame *f = NULL;
	int end = false, playing = true, inputKey;

	while(!end) {
		try {
			f = getFrame();
		} catch (VideoEndedException& e) {
			end = true;
			continue;
		}
		f->display();
		delete f;
		if(playing)
		{
			/* wait according to the frame rate */
			inputKey = waitKey(1.0 / m_fps * 1000);
		}
		else
		{
			/* wait until user press a key */
			inputKey = waitKey(0);
		}
	
		/* parse the pressed keys, if any */
		switch((char)inputKey)
		{
			case 'q':
				end = 1;
				break;
			
			case 'p':
				playing = playing ? 0 : 1;
				break;
		}		
	}
}