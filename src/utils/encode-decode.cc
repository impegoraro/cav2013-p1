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

void encode(const Frame* actual, Predictor *p, uint m, BitStream& bs)
{
	Golomb g(*p, bs, m);
	g.encode();
	bs.flush();
}

void encodeInterframe(const Frame* previous, const Frame* actual, Predictor *p, uint m, BitStream& bs)
{
	if(previous == nullptr) {
		Golomb g(*p, bs, m);
		g.encode();
		bs.flush();
	} else {
		// (BitStream& bs, const Frame* pf, const Frame* nf, uint m, uint bWidth, uint bHeight)
		GolombInterframe gi(bs, previous, actual, m, actual->rows() / 4, actual->cols() / 4);
		//GolombInterframe gi(bs, previous, actual, m, 8, 8);
		gi.encode();
	}
}

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
	float quant = 1;
	bool shouldHaveDest{false};
	int predType = 1;
	bool block{false};
	const char* shortops = "hd:p:s:q:m:l:nb";
	const struct option longops[] = {
		{"help", 0, NULL, 'h'},
		{"source", 1, NULL, 's'},
		{"destination", 1, NULL, 'd'},
		{"quantization", 1, NULL, 'q'},
		{"golomb-factor", 1, NULL, 'm'},
		{"nonlinear-predictor", 0, NULL, 'n'},
		{"linear-predictor", 1, NULL, 'l'},
		{"block-interframe", 0, NULL, 'b'},
		{"play", 1, NULL, 'p'}
	};

	do {
		nextOp = getopt_long(argc, argv, shortops, longops, NULL);
		switch(nextOp) {
			case 'h':
				nextOp = -1;
				showHelp = true;
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
			case 'b':
				block = true;
			break;
			case 'l':
				pIndex = atoi(optarg);
				predType = 1;
				if(pIndex > 6) {
					cerr<<"Error: linear predictors allowed range from 0 to 6, [0, 6]."<<endl;
					abort();
				}
			break;
			case 'n':
				pIndex = 0;
				predType = 2;
			break;
			case 'm':
				m = atoi(optarg);
				if(!isPowerOf2(m)) {
					cerr<< "Error: Invalid parameter golomb m parameter, the parameter must be power of 2."<<endl;
					abort();
				}
			break;
			case 'q':
				quant = atof(optarg);
				if(quant <= 0.0)
					quant = 1.0;
			break;
			case '?':
			default:
				showHelp = false;
			break;
		}
	} while(nextOp !=- 1);

	if(showHelp || src == NULL || shouldHaveDest) {
		cerr<< "Usage: encondeDecode [OPTIONS] <video>"<<endl;
		cout<< "The program is able to encode/decode videos in the following formats: YUV444, YUV422 and YUV420."<<endl
			<<"[OPTIONS]"<<endl
			<<"  -h, --help                    Shows this help message."<<endl
			<<"  -s, --source                  Filename to read the video from."<<endl
			<<"  -d, --destination             Filename where to store the encoded the video."<<endl
			<<"  -q, --quantization            Quantization factor (for lossy compression)."<<endl
			<<"  -g, --golomb                  Golomb factor (m) to use while encoding/decoding."<<endl
			<<"  -n, --non-linear              Changes the default predictor to be the nonlinear predictor."<<endl
			<<"  -l, --linear                  Changes the default predictor to be a linear predictor (requires an integer [0, 6])."<<endl
			<<"  -p, --play                    Plays an encoded video."<<endl
			<<"  -b, --block-interframe        Uses the interframe method to encode the video."<<endl
			<<endl;
		cout<<"encondeDecode  Copyright (C) 2013  Universidade de Aveiro  - MIECT Audio and Video Coding"<<endl;
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
		cout<< "Number of Frames: "<< vh->nFrames<<endl;
		cout<< "Quantization factor: "<< header.quantFactor<<endl;
		cout<< "Predictor: "<< ((header.predictor == 1) ? "Linear" : "Non-Linear")<< " - "<< header.index<<endl;
		cout<< "Golomb factor: "<< header.m<<endl;
		bool firstFrame{true};
		Frame *f{nullptr};
		Frame *f2;
		while(!end) {
			for(uint i = 0; i < vh->nFrames; i++) { 
				if(!header.block) {
					Predictor pred = Golomb::decode(bs);
					f = pred.guess();
					f->display(false,  "VideoPlayback");
				 	waitKey(1.0 / vh->fps * 1000);
					delete f;
				} else {
					if(firstFrame) {
						Predictor pred = Golomb::decode(bs);
						Frame *f = pred.guess();
						f->display(false,  "VideoPlayback");
						f2 = new Frame(*f);
						firstFrame = false;
					} else {
						Frame *tmp = f2;
						GolombInterframe gi(bs, tmp, nullptr, header.m, f2->rows() / 2, f2->cols() / 2);
						f2 = gi.decode(header.m);
						f2->display(false,  "VideoPlayback");
					}
				 	//waitKey(1.0 / vh->fps * 1000);
				 	waitKey(0);
				}
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
			header.predictor = predType;
			header.quantFactor = quant;
			header.index = pIndex;
			header.m = m;
			header.block = block;
			vHeader.magic = VIDEO_MAGIC; 
			vHeader.nFrames = v->getTotalFrames();
			vHeader.fps = v->fps();

			memcpy(header.undefined, (char*) &vHeader, sizeof(vHeader));

			ss<< dst;
			BitStream bs(ss.str().c_str(), (char*)"wb", (CAVHeader*) &header);
			cout<< "Predictor: "<< ((header.predictor == 1) ? "Linear" : "Non-Linear")<< " - "<< header.index<<endl;
			cout<< "Quantization factor: "<< quant<<endl;
			cout<< "Golomb factor: "<< m<<endl;
			Predictor *p = {nullptr};
			Frame *prev = nullptr;
			while(!end) {
				try {
					f = v->getFrame();
				} catch (VideoEndedException& e) {
					end = true;
					continue;
				}
				if(predType == 1)
					p = new LinearPredictor(*f, pIndex, quant);
				else
					p = new NonLinearPredictor(*f, quant);

				if(block)
					encodeInterframe(prev, f, p, m, bs);
				else
					encode(f, p, m, bs);

				delete p;
				if(prev != nullptr)
					delete prev;
				prev = f;
			}

			delete v;
		} catch (FileNotFoundException& e) {
			cerr<< "File not found"<< endl;
		}
	}

	return 0;
}
