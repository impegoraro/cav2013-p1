#include <iostream>
#include <string>

#include <opencv2/opencv.hpp>
#include <getopt.h>

#include "video.h"
#include "block.h"
#include "frame.h"
#include "exceptions/cav-exceptions.h"
#include "framergb.h"
#include "frame444.h"
#include "frame422.h"
#include "frame420.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	int nextOp;
	uint rows = 1, cols = 1;
	char *src = NULL, *dst = NULL;
	const char* shortops = "r:c:hs:d:";
	const struct option longops[] = {
		"help", 0, NULL, 'h',
		"rows", 1, NULL, 'r',
		"colums", 1, NULL, 'c',
		"source", 1, NULL, 's',
		"destination", 1, NULL, 'd'
	};

	do {
		nextOp = getopt_long(argc, argv, shortops, longops, NULL);
		switch(nextOp) {
			case 'h':
				nextOp = -1;
			break;
			case 's':
				src = optarg;
			break;
			case 'd':
				dst = optarg;
			break;
			case 'c':
				cols = atoi(optarg);
			break;
			case 'r':
				rows = atoi(optarg);
			break;
			case '?':
				nextOp = -1;
			break;
		}
	} while(nextOp !=- 1);

	if(src == NULL || dst == NULL) {
		cerr<< "Usage: yuvCopy [OPTIONS] -s <source> -d <destination>"<<endl<<endl;
		cout<<"Options are:"<<endl
			<<"  -h, --help           Shows this help message."<<endl
			<<"  -s, --source         Specifies the input video filepath."<<endl
			<<"  -d, --destination    Specifies the output video filepath."<<endl
			<<"  -c, --columns        Columns for the custom block."<<endl
			<<"  -r, --rows           Rows for the custom block."<<endl
			<<endl<< "The program is able to copy in the following formats: YUV444, YUV422 and YUV420."<<endl
			<< "Univesidade de Aveiro 2013 - MIETC Audio and Video Coding"<<endl
			<< "Authors:"<<endl
			<< "    Ilan Pegoraro N. 41450"<<endl
			<< "    Luis Neves    N. 41528"<<endl;
		return 1;
	}

	try {
		bool cont(true);
		uint fSize, bSize(rows * cols);
		Video vsrc((string)src);
		Video vdst((string)dst, vsrc.rows(), vsrc.cols(), vsrc.fps(), vsrc.format());

		while(cont) {
			try {
				Frame *f;
				f = vsrc.getFrame();

				if(rows == cols == 1) {
					vdst.putFrame(*f);
				} else {
					//cout<< "Here"<<endl;
					int nBlocks = (rows * cols), remaining = f->y().size() % (rows * cols);
					Frame444 f2(f->rows(), f->cols());
					int i(0);

					for(i = 0; i < f->y().size() - remaining; i+=nBlocks) {
						Block b = std::move(f->y().getSubBlock(i, rows, cols));
						Block b2(1,1);
						f2.y().setSubBlock(i, b);
						//cout<<"Comparing Y component using sublocks..."<<endl;
						//b2 = std::move(f2.y().getSubBlock(i, rows, cols));
						//assert(b2 == b);

						b = std::move(f->u().getSubBlock(i, rows, cols));
						f2.u().setSubBlock(i, b);
						//cout<<"Comparing U component using sublocks..."<<endl;
						//b2 = std::move(f2.u().getSubBlock(i, rows, cols));
						//assert(b2 == b);


						b = std::move(f->v().getSubBlock(i, rows, cols));
						f2.v().setSubBlock(i, b);
						//cout<<"Comparing V component using sublocks..."<<endl;
						//b2 = std::move(f2.v().getSubBlock(i, rows, cols));
						//assert(b2 == b);

					}
					if(remaining) {
						cout<< "Copying remaining "<< remaining<< " bytes..."<<endl;
						Block b = std::move(f->y().getSubBlock(i, 1, remaining));
						Block b2(1,1);
						f2.y().setSubBlock(i, b);
						//cout<<"Comparing Y component using sublocks..."<<endl;
						//b2 = std::move(f2.y().getSubBlock(i, 1, remaining));
						//assert(b2 == b);

						b = std::move(f->u().getSubBlock(i, 1, remaining));
						f2.u().setSubBlock(i, b);
						//cout<<"Comparing U component using sublocks..."<<endl;
						//b2 = std::move(f2.u().getSubBlock(i, 1, remaining));
						//assert(b2 == b);


						b = std::move(f->v().getSubBlock(i, 1, remaining));
						f2.v().setSubBlock(i, b);
						//cout<<"Comparing V component using sublocks..."<<endl;
						//b2 = std::move(f2.v().getSubBlock(i, 1, remaining));
						//assert(b2 == b);
					}


					vdst.putFrame(f2);
				}
				delete f;
			} catch(VideoEndedException& e) {
				cont = false;
			}
		}
		
	} catch (FileNotFoundException& e) {
		cerr<< "File not found"<< endl;
	}
	
	return 0;
}
