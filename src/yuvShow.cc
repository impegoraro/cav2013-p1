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
		cerr<< "Usage: yuvShow <video>"<<endl<<endl;
		cout<< "The program is able to play videos in the following formats: YUV444, YUV422 and YUV420."<<endl;
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
			v->display();
			
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
