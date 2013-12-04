#include <string>
#include <fstream>
#include <opencv2/opencv.hpp>
#include "video-interface.h"
#include "frame.h"


void VideoInterface::display(bool playing)
{
	Frame *f = NULL;
	int end = false, inputKey;

	while(!end) {
		try {
			f = getFrame();
		} catch (VideoEndedException& e) {
			end = true;
			continue;
		}
		f->display(false, "Video Playback");
		
		delete f;
		if(playing)
		{
			/* wait according to the frame rate */
			inputKey = cv::waitKey(1.0 / m_fps * 1000);
		}
		else
		{
			/* wait until user press a key */
			inputKey = cv::waitKey(0);
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