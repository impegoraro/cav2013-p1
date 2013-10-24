#include <iostream>
#include <getopt.h>
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
	bool cont = false, playing = true;
	string ans;
	bool showHelp = false;
	char *src = NULL;
	char nextOp;
	const char* shortops = "rmhs:";
	const struct option longops[] = {
		"help", 0, NULL, 'h',
		"manual-fps", 0, NULL, 'm',
		"repeat", 0, NULL, 'r',
		"source", 1, NULL, 's'
	};

	do {
		nextOp = getopt_long(argc, argv, shortops, longops, NULL);
		switch(nextOp) {
			case 'h':
				nextOp = -1;
				showHelp = true;
			break;
			case 'r':
				cont = true;
			break;
			case 'm':
				playing = false;
			break;
			case 's':
				src = optarg;
			break;
			case '?':
			default:
				showHelp = false;
			break;
		}
	} while(nextOp !=- 1);

	if(showHelp) {
		cerr<< "Usage: yuvShow [OPTIONS] <video>"<<endl<<endl;
		cout<<"[OPTIONS]"<<endl
			<<"  -h, --help                    Shows this help message."<<endl
			<<"  -s, --source                  Filename to read the video from. Missing this switch means to read from camera."<<endl
			<<"  -r, --repeat                  Ask to repeat the video when finishes."<<endl
			<<"  -m, --manual-fps              Use any key to step throughout the video instead of using the video's FPS."<<endl;
		cout<< "The program is able to play videos in the following formats: RGB, YUV444, YUV422 and YUV420."<<endl;
		cout<< "Univesidade de Aveiro 2013 - MIETC Audio and Video Coding"<<endl;
		cout<< "Authors:"<<endl;
		cout<< "    Ilan Pegoraro N. 41450"<<endl;
		cout<< "    Luis Neves    N. 41528"<<endl;
		return 1;
	}
	try {
		Video *v;
		if(src == NULL) {
			cout<< "Using webcam..."<<endl;
			v = new Video();
		} else {
			cout<< "Using file..."<<endl;
			string path(src);
			v = new Video(path);
		}
		
		v->display(playing);
			
		while(cont) {
			cout<< "Do you want to play it again? [y/n]: ";
			cin>> ans;
			if(ans == "y" || ans == "Y") {
				cont = true;
				v->reset();
			}
			else if(ans == "n" || ans == "N") {
				cont = false;
				break;
			}
			v->display(playing);
		}
		delete v;
	} catch (FileNotFoundException& e) {
		cerr<< "File not found"<< endl;
	}
	
	return 0;
}
