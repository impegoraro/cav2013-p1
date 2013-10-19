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
	if(argc != 2 || !strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) {
		cerr<< "Usage: yuvCopy <src> <destination>"<<endl<<endl;
		cout<< "The program is able to copy in the following formats: YUV444, YUV422 and YUV420."<<endl;
		cout<< "Univesidade de Aveiro 2013 - MIETC Audio and Video Coding"<<endl;
		cout<< "Authors:"<<endl;
		cout<< "    Ilan Pegoraro N. 41450"<<endl;
		cout<< "    Luis Neves    N. 41528"<<endl;
		return 1;
	}

	try {
		string path(argv[1]);
		string path(argv[2]);
		Video v(path);
		Video v(path2)


	} catch (FileNotFoundException& e) {
		cerr<< "File not found"<< endl;
	}
	
	return 0;
}
