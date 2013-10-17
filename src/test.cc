#include <iostream>
#include <cstdio>
#include <opencv2/opencv.hpp>

#include "video.h"
#include "block.h"
#include "frame.h"
#include "exceptions/cav-exceptions.h"
#include "frame422.h"
#include "frame420.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	int end = false, playing = true, inputKey, fps = 25;
	Frame *f; 

	if(argc == 1) {
		cerr<< "Usage: "<< argv[0]<< " <picture>"<<endl;
		return 1;
	}
	try {
		string path(argv[1]);
		Video v(path);
		f= v.getFrame();

		while(!end) {
			f->display();
			if(playing)
			{
				/* wait according to the frame rate */
				inputKey = waitKey(1.0 / v.fps() * 1000);
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
			delete f;
			try {
				f = v.getFrame();
			} catch (exception& e) {
				cout<< "Video ended"<<endl;
				end = true;
			}
		}
		delete f;
	} catch (FileNotFoundException& e) {
		cerr<< "File not found"<< endl;
	}
	
	return 0;
}
