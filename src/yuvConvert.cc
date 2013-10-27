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
	bool showHelp = false;
	string ans;
	int nextOp;
	string format;
	char *src = NULL, *dst = NULL;
	VideoFormat vf(YUV_444);
	const char* shortops = "s:d:f:h";
	const struct option longops[] = {
		"help", 0, NULL, 'h',
		"format", 1, NULL, 'f',
		"destination", 1, NULL, 'd',
		"source", 1, NULL, 's'
	};
	
	do {
		nextOp = getopt_long(argc, argv, shortops, longops, NULL);
		switch(nextOp) {
			case 'h':
				nextOp = -1;
				showHelp = true;
			break;
			case 's':
				src = optarg;
			break;
			case 'd':
				dst = optarg;
			break;
			case 'f':
				format = string(optarg);
				if(format == "444")
					vf = YUV_444;
				else if(format == "422")
					vf = YUV_422;
				else if(format == "420")
					vf = YUV_420;
				else
					src = dst = NULL;
			break;
			case '?':
			default:
				showHelp = false;
			break;
		}
	} while(nextOp !=- 1);

	if(showHelp || src == NULL || dst == NULL) {
		cerr<< "Usage: yuvConvert [OPTIONS] -s <source> -d <destination>"<<endl<<endl;
		cout<<"Options are:"<<endl
			<<"  -h, --help           Shows this help message."<<endl
			<<"  -s, --source         Video source."<<endl
			<<"  -i, --destination    Video destination."<<endl
			<<"  -f  --format         Output format."<<endl
			<<endl<< "The program is able to convert between the following formats: RGB, YUV444, YUV422 and YUV420."<<endl
			<< "Univesidade de Aveiro 2013 - MIETC Audio and Video Coding"<<endl
			<< "Authors:"<<endl
			<< "    Ilan Pegoraro N. 41450"<<endl
			<< "    Luis Neves    N. 41528"<<endl;
		return 1;
	}


	try {
		Video v((string)src);
		v.convert((string)dst, vf);
		//Video vdst((string)dst, v.rows(), v.cols(), v.fps(), vf);
		//Frame *f = NULL, f2;
		//int end = false;
		//	
		//while(!end) {
		//	try {
		//		f = v.getFrame();
		//		f2 = std::move(f->convert(vf));
		//		vdst.putFrame(f2);
		//	} catch (VideoEndedException& e) {
		//		end = true;
		//		continue;
		//	}
		//	delete f;
		//}
	} catch (FileNotFoundException& e) {
		cerr<< "File not found"<< endl;
	}
	
	return 0;
}
