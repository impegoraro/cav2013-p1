#include <iostream>
#include <opencv2/opencv.hpp>
#include <getopt.h>

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
	bool cont = true, showHelp = false;
	string ans;
	int nextOp;
	float luminance;
	char *src = NULL;
	int operation(0); // 0 -> invert; 1 black and white; 2 - luminance
	const char* shortops = "s:ibl:h";
	const struct option longops[] = {
		"help", 0, NULL, 'h',
		"invert", 0, NULL, 'i',
		"black-white", 0, NULL, 'b',
		"luminance", 1, NULL, 'l',
		"source", 1, NULL, 's'
	};

	do {
		nextOp = getopt_long(argc, argv, shortops, longops, NULL);
		switch(nextOp) {
			case 'h':
				nextOp = -1;
				showHelp = true;
			break;
			case 'i':
				operation = 0;
			break;
			case 'b':
				operation = 1;
			break;
			case 'l':
				operation = 2;
				luminance = atof(optarg);
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
		cerr<< "Usage: videoEffects [OPTIONS] <source>"<<endl<<endl;
		cout<<"Options are:"<<endl
			<<"  -h, --help           Shows this help message."<<endl
			<<"  -l, --luminance      Changes the luminance of each video frame."<<endl
			<<"  -i, --invert         Inverts the colors of each video frame."<<endl
			<<"  -b, --black-white    Turns the video to black and white."<<endl
			<<"  -s, --source         Video file path."<<endl
			<<endl<< "The program is able to apply effects in the following formats: RGB, YUV444, YUV422 and YUV420."<<endl
			<< "Univesidade de Aveiro 2013 - MIETC Audio and Video Coding"<<endl
			<< "Authors:"<<endl
			<< "    Ilan Pegoraro N. 41450"<<endl
			<< "    Luis Neves    N. 41528"<<endl;
		return 1;
	}


	try {
		Video *v;
		if(src == NULL)
			v = new Video(0);
		else {
			string path(src);
			v = new Video(path);
		}
		
		while(cont) {
			Frame *f = NULL;
			int end = false, playing = true, inputKey;

			while(!end) {
				try {
					f = v->getFrame();
					switch(operation){
					case 1:
						f->setBlackWhite();
						break;
					case 0:
						f->setInvertColors();
						break;
					case 2:
						f->changeLuminance(luminance);
						break;
					}
				} catch (VideoEndedException& e) {
					end = true;
					continue;
				}
				f->display();
				delete f;
				if(playing)
				{
					/* wait according to the frame rate */
					inputKey = waitKey(1.0 / v->fps() * 1000);
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
			
			do {
				cout<< "Do you want to play it again? [y/n]: ";
				cin>> ans;
				if(ans == "y" || ans == "Y") {
					cont = true;
					v->reset();
					break;
				}
				else if(ans == "n" || ans == "N") {
					cont = false;
					break;
				}
			} while(true);
		}
		delete v;
	} catch (FileNotFoundException& e) {
		cerr<< "File not found"<< endl;
	}
	
	return 0;
}