/*
 * frameExtractor.cc
 * Copyright (C) 2013  Ilan Pegoraro and Luís Neves
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <getopt.h>
#include <libgen.h>
#include <cstring>
#include <errno.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <libcav.h>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
// 	if(argc == 1) {
// 		cerr<< "Missing filename"<<endl;
// 		exit(1);
// 	}
// 	Frame *f= Frame::create_from_file((string)argv[1]);
// 	stringstream ss;
// 	//Predictor lp(*f, i, [](int a, int b, int c, int d) -> int { return a; });
// 	NonLinearPredictor lp(*f);
// 	ss<< "/home/ilan/Downloads/CAV/tmp/golomb-0.gmb";
// 	cout<<"Main: writing "<< ss.str()<< endl;
// 	GolombCAVHeader header;
// 	{
// 		header.magic = GOLOMB_MAGIC;
// 		header.nCols = f->cols();
// 		header.nRows = f->rows();
// 		header.format = f->getFormat();
// 		header.m = 4;
// 		header.predictor = NONLINEAR_PREDICTOR;
// 		header.index = 0;
// 		BitStream bs(ss.str().c_str(), (char*)"wb", (CAVHeader*)&header);
// 		Golomb g(lp, bs, 4);
// 		g.encode();
// 	}

// 	cout<< "magic: "<< header.magic<< endl;
// 	cout<< "nCols: "<< header.nCols<< endl;
// 	cout<< "nRows: "<< header.nRows<< endl;
// 	cout<< "format: "<< header.format<< endl;
// 	cout<< "m: "<< header.m<< endl;
// 	cout<< "predictor: "<< header.predictor<< endl;
// 	cout<< "index: "<< header.index<< endl;
// 	{
// 		BitStream bs(ss.str().c_str(), (char*)"rb", (CAVHeader*)&header);
// 		cout<< "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<endl;
// 		cout<< "magic: "<< header.magic<< endl;
// 		cout<< "nCols: "<< header.nCols<< endl;
// 		cout<< "nRows: "<< header.nRows<< endl;
// 		cout<< "format: "<< header.format<< endl;
// 		cout<< "m: "<< header.m<< endl;
// 		cout<< "predictor: "<< header.predictor<< endl;
// 		cout<< "index: "<< header.index<< endl;

// 		Predictor pred{Golomb::decode(bs)};
// 		Frame* f2 = pred.guess();
// 		f2->display();
// 		delete f2;

// 	}
// }
	string ans;
	bool showHelp{false};
	char *src{NULL};
	char *dst{NULL};
	char nextOp;
	int pIndex{0};
	bool play{false};
	int m{4};
	bool shouldHaveDest{false};
	const char* shortops = "hd:p:s:";
	const struct option longops[] = {
		{"help", 0, NULL, 'h'},
		{"source", 1, NULL, 's'},
		{"destination", 1, NULL, 'd'},
		{"play", 1, NULL, 'p'}
	};

	do {
		nextOp = getopt_long(argc, argv, shortops, longops, NULL);
		switch(nextOp) {
			case 'h':
				nextOp = -1;
				showHelp = true;
			break;
			break;
			case 's':
				shouldHaveDest = true;
				src = optarg;
			break;
			case 'd':
				shouldHaveDest = false;
				dst = optarg;
			break;
			case 'p':
				src = optarg;
				play = true;
			break;
			case '?':
			default:
				showHelp = false;
			break;
		}
	} while(nextOp !=- 1);

	if(showHelp || src == NULL || shouldHaveDest) {
		cerr<< "Usage: yuvEncode [OPTIONS] <video>"<<endl;
		cout<< "The program is able to play videos in the following formats: RGB, YUV444, YUV422 and YUV420."<<endl
			<<"[OPTIONS]"<<endl
			<<"  -h, --help                    Shows this help message."<<endl
			<<"  -s, --source                  Filename to read the video from. Missing this switch means to read from camera."<<endl<<endl;
		cout<<"yuvEncode  Copyright (C) 2013  Universidade de Aveiro  - MIECT Audio and Video Coding"<<endl;
		cout<< "Authors:"<<endl;
		cout<< "    Ilan Pegoraro N. 41450"<<endl;
		cout<< "    Luis Neves    N. 41528"<<endl;
		return 1;
	}

	if(play) {
		bool end{false};
		stringstream ss;
		ss<< src;
		GolombCAVHeader header;
		BitStream bs(ss.str().c_str(), (char*)"rb", (CAVHeader*) &header);
		VideoCAVHeader *vh = (VideoCAVHeader*)((GolombCAVHeader*) &header)->undefined;

		while(!end) {
			for(uint i = 0; i < vh->nFrames; i++){ 
				Predictor pred = Golomb::decode(bs);
				Frame *f = pred.guess();
				f->display(false,  "VideoPlayback");
				delete f;
			 	waitKey(1.0 / vh->fps * 1000);
			}	
			
		}
	} else {
		try {
			Video *v;
			string path(src);
			stringstream ss;
			Frame *f = NULL;
			bool end{false};

			v = new Video(path);
			GolombCAVHeader header;
			VideoCAVHeader vHeader;

			header.magic = GOLOMB_MAGIC;
			header.nCols = v->cols();
			header.nRows = v->rows();
			header.format = v->format();
			header.predictor = LINEAR_PREDICTOR;
			header.index = pIndex;
			header.m = m;
			vHeader.magic = VIDEO_MAGIC; 
			vHeader.nFrames = v->getTotalFrames();
			vHeader.fps = v->fps();

			memcpy(header.undefined, (char*) &vHeader, sizeof(vHeader));

			ss<< dst;
			BitStream bs(ss.str().c_str(), (char*)"wb", (CAVHeader*) &header);
			while(!end) {
				try {
					f = v->getFrame();
				} catch (VideoEndedException& e) {
					end = true;
					continue;
				}
				LinearPredictor lp(*f, pIndex);
				Predictor &p = lp;
				Golomb g(p, bs, m);

				g.encode();

				delete f;
			}

			delete v;
		} catch (FileNotFoundException& e) {
			cerr<< "File not found"<< endl;
		}
	}

	return 0;
}
