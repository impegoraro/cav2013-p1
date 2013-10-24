#ifndef CAV_VIDEO_H
#define CAV_VIDEO_H

#include <string>
#include <fstream>
#include <opencv2/opencv.hpp>
#include "frame.h"

/**
 * Class that has all the information about the Video and manages accessing/displaying the Frames.
 */

class Video
{
public:
	/**
	 * Constructs a video reading from the first webcam.
	 */
	Video();
	/** Construct a Video object from a device.
	 * @param device - device number
	 */
	Video(int device);
	/** Construct a Video object from a file.	
	 * @param path - file path of the  
	 */
	Video(const std::string& path);
	/**
	 * Construct a Video object for writing.	
	 * @param fpath  - File path of the file
	 * @param rows - Number of rows
	 * @param cols - Number of columns.
	 * @param fps - Frames per second.
	 * @param format - VideoFormat.
	 */
	Video(const std::string& fpath, uint rows, uint cols, uint fps, VideoFormat format);
	/**
	 * Video Destructor.
	 * Cleans the internal state of the class, closes the stream.
	 */
	virtual ~Video();

	/**
	 * Gets the rows that each frame represents
	 * @return uint - Number of rows
	 */
	uint rows();
	/**
	 * Gets the columns that each frame represents
	 * @return uint - Number of columns
	 */
	uint cols();
	/**
	 * Gets the frames per second
	 * @return uint - Number of frames per second
	 */
	uint fps();
	/**
	 * Gets the video's format.
	 * @return VideoFormat - Current VideoFormat.
	 */
	VideoFormat format();

	/**
	 * Returns the videos next frame. Note that the frame is dynamically allocated, so the resposability of deleting it is delegated to the caller.
	 * @throws VideoEndedException
	 * @return Frame* - Dynamically allocated frame.
	 */
	Frame* getFrame();
	/**
	 * Writes the frame to the mass storage device.
	 * pre-condition: the frame to be written must match the videos format.
	 * @param f - A reference to a frame
	 */
	void putFrame(Frame& f);

	/**
	 * Sets the posistion of the file to the begining.
	 */
	void reset();
	/**
	 * Displays the video in a window.
	 * @param playing - if true the video runs with its FPS, otherwise a key must be pressed to step through the frames. (default: true)
	 */
	void display(bool playing = true);

protected:
	/**
	 * File stream to read of write the video (YUV444, YUV422 and YUV422 formats).
	 */
	std::fstream m_stream;
	/**
	 * Number of rows of the videos
	 */
	uint m_rows;
	/**
	 * Number of columns of the videos
	 */
	uint m_cols;
	/**
	 * Frame rate (Frames per second)
	 */
	uint m_fps;
	/**
	 * Whether we're reading from camera (or avi files) or YUV.
	 */
	bool m_fromCam;
	/**
	 * Access to the RGB video (camera or .avi)
	 */
	cv::VideoCapture m_video;
	/**
	 * Video's format.
	 */
	VideoFormat m_type;
};

#endif