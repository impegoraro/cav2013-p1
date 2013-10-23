#include <iostream>
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
	bool cont = true;
	string ans;
	if(argc > 1 && (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h"))) {
		cerr<< "Usage: videoEffects <video>"<<endl<<endl;
		cout<< "The program is able to set effects on videos and play them in the following formats: RGB, YUV444, YUV422 and YUV420."<<endl;
		cout<< "Univesidade de Aveiro 2013 - MIETC Audio and Video Coding"<<endl;
		cout<< "Authors:"<<endl;
		cout<< "    Ilan Pegoraro N. 41450"<<endl;
		cout<< "    Luis Neves    N. 41528"<<endl;
		return 1;
	}
	try {
		Video *v;
		if(argc <= 1)
			v = new Video();
		else {
			string path(argv[1]);
			v = new Video(path);
		}
		
		while(cont) {
			Frame *f = NULL;
			int end = false, playing = true, inputKey;

			while(!end) {
				try {
					f = v->getFrame();
					//f->setBlackWhite();
					f->setInvertColors();
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
