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

Video::Video()
	: VideoInterface(0, 0, 0, RGB), m_fromCam(true), m_video(0), m_headerSize{0}
{
	std::string device("/dev/video0");
	
	m_stream.open(device);
	m_fps = 30; //(int)m_video.get(CV_CAP_PROP_FPS);
	m_cols = (int)m_video.get(CV_CAP_PROP_FRAME_WIDTH);
	m_rows = (int)m_video.get(CV_CAP_PROP_FRAME_HEIGHT);
}

Video::Video(int number)
	: VideoInterface(0, 0, 0, RGB), m_fromCam(true), m_video(0), m_headerSize{0}
{
	std::string device("/dev/video");
	
	device += number;
	m_stream.open(device);
	m_fps = 30; //(int)m_video.get(CV_CAP_PROP_FPS);
	m_cols= (int)m_video.get(CV_CAP_PROP_FRAME_WIDTH);
	m_rows = (int)m_video.get(CV_CAP_PROP_FRAME_HEIGHT);
}

Video::Video(const std::string& fpath)
	: VideoInterface(0, 0, 0, YUV_444), m_stream(fpath, std::ios::in | std::ios::out | std::ios::binary), m_fromCam(false), m_video(), m_headerSize{0}
{
	int type;
	char c; // to get the newline
	if(!m_stream.good())
		throw FileNotFoundException();
	if(fpath.find(".yuv") != std::string::npos) {
		m_stream>> m_cols>> m_rows>> m_fps>> type>> c;
		m_headerSize = m_stream.tellg();
		m_video.release();
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
	} else {
		// In AVI Mode

		m_video.open(fpath);
		m_fps = 30; //(int)m_video.get(CV_CAP_PROP_FPS);
		m_cols= (int)m_video.get(CV_CAP_PROP_FRAME_WIDTH);
		m_rows = (int)m_video.get(CV_CAP_PROP_FRAME_HEIGHT);
		m_fromCam = true;
		m_type = RGB;
	}
}

Video::Video(const std::string& fpath, uint rows, uint cols, uint fps, VideoFormat format)
	: VideoInterface(rows, cols, fps, format), m_stream(fpath, std::ios::in | std::ios::out | std::ios::trunc), m_fromCam(false), m_headerSize{0}
{
	if(!m_stream.good())
		throw FileNotFoundException();

	m_stream<< m_cols<< " "<< m_rows<< " "<< m_fps<< " "<< format<< std::endl;

}

Video::~Video()
{
	if(!m_fromCam)
		m_stream.close();
}

Frame* Video::getFrame()
{
	Frame *f(NULL);
	unsigned char buffer[m_cols * m_rows * 3];
	uint cols(0), rows(0), size(0);
	int y, u, v;

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

	assert(f != NULL);
	if(m_fromCam) {
		int r, g, b, y, u, v;
		Mat tmpFrame;
		
		delete f;
		if(!m_video.read(tmpFrame)) {
			throw VideoEndedException();
		}
		// Unneeded
		f = new Frame444(rows, cols);

		for(uint i = 0; i < rows * cols * 3; i += 3) {
			b = tmpFrame.ptr()[i];
			g = tmpFrame.ptr()[i + 1];
			r = tmpFrame.ptr()[i + 2];

			y = r *  .299 + g *  .587 + b *  .114 ;
			u = r * -.169 + g * -.332 + b *  .500  + 128.;
			v = r *  .500 + g * -.419 + b * -.0813 + 128.;

			/* clipping to [0 ... 255] */
			if(y < 0) y = 0;
			if(u < 0) u = 0;
			if(v < 0) v = 0;
			if(y > 255) y = 255;
			if(u > 255) u = 255;
			if(v > 255) v = 255;

			f->y()[i / 3] = y;
			f->u()[i / 3] = u;
			f->v()[i / 3] = v;
		}
	} else {
	
		m_stream.read((char*)buffer, size);
		for(uint i = 0 ; i < m_rows * m_cols * 3 ; i += 3)
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
	}
	return f;
}

void Video::putFrame(Frame& f)
{
	assert(f.getFormat() == m_type);
	unsigned char *buffer;
	uint size(0);

	buffer = f.packedMode(size);
	m_stream.write((char*)buffer, size);
	delete buffer;
}

void Video::reset()
{
	string tmp;
	if(!m_fromCam) {
		if(!m_stream.is_open())
			throw FileNotOpenException();
		m_stream.clear();
		m_stream.seekg(0, std::ios_base::beg);
		getline(m_stream, tmp); // skip the header
	}
}

void Video::convert(const std::string& path, VideoFormat dest)
{
	Video vdst(path, m_rows, m_cols, m_fps, dest);
	Frame *f = NULL, f2;
	int end = false;

	reset();
	while(!end) {
		try {
			f = getFrame();
			f2 = std::move(f->convert(dest));
			vdst.putFrame(f2);

		} catch (VideoEndedException& e) {
			end = true;
			continue;
		}
		delete f;
	}
}

uint Video::getTotalFrames()
{
	m_stream.clear(); // cleans eof so that tellg works

    uint curpos{m_stream.tellg()};
	m_stream.seekg(0, m_stream.end);
    uint vsize{m_stream.tellg()};
	
	m_stream.seekg(curpos, m_stream.beg);

	return (vsize / getFrameSize());
}