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
	if(argc == 1) {
		cerr<< "Usage: "<< argv[0]<< " <picture>"<<endl;
		return 1;
	}
	try {
		string path(argv[1]);
		Video v(path);
		v.display();
		
	} catch (FileNotFoundException& e) {
		cerr<< "File not found"<< endl;
	}
	
	return 0;
}
