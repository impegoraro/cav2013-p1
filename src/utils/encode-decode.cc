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

double encode(const Frame* actual, Predictor *p, uint m, BitStream& bs)
{
	Golomb g(*p, bs, m);
	g.encode();
	double enctime{g.getEncodeTime()};
	bs.flush();

	return enctime;
}

double encodeInterframe(const Frame* previous, const Frame* actual, Predictor *p, uint m, BitStream& bs, int radius, uint factor)
{
	double enctime{0.0};
	if(previous == nullptr) {
		Golomb g(*p, bs, m);
		g.encode();
		enctime = g.getEncodeTime();
		bs.flush();
	} else {
		GolombInterframe gi(bs, previous, actual, m, actual->rows() / factor, actual->cols() / factor, radius);
		gi.encode();
		enctime = gi.getEncodeTime();
		bs.flush();
	}

	return enctime;
}

int main(int argc, char** argv)
{
	string ans;
	bool showHelp{false};
	char *src{NULL};
	char *dst{NULL};
	char nextOp;
	int pIndex{0};
	bool play{false};
	int m{4};
	int quantY = 1;
	int quantU = 1;
	int quantV = 1;
	int radius = 0;
	bool shouldHaveDest{false};
	int predType = 1;
	uint factor = 2;
	bool block{false};
	const char* shortops = "hd:p:s:y:u:v:m:l:nbr:f:";
	const struct option longops[] = {
		{"help", 0, NULL, 'h'},
		{"source", 1, NULL, 's'},
		{"destination", 1, NULL, 'd'},
		{"quantization-y", 1, NULL, 'y'},
		{"quantization-u", 1, NULL, 'u'},
		{"quantization-v", 1, NULL, 'v'},
		{"factor", 1, NULL, 'f'},
		{"radius", 1, NULL, 'r'},
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
			case 'r':
				radius = atoi(optarg);
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
			case 'f':
				factor = (uint) atoi(optarg);
			break;
			case 'm':
				m = atoi(optarg);
				if(!isPowerOf2(m)) {
					cerr<< "Error: Invalid parameter golomb m parameter, the parameter must be power of 2."<<endl;
					abort();
				}
			break;
			case 'y':
				quantY = atoi(optarg);
				if(quantY <= 0)
					quantY = 1;
			break;
			case 'u':
				quantU = atoi(optarg);
				if(quantU <= 0.0)
					quantU = 1.0;
			break;
			case 'v':
				quantV = atoi(optarg);
				if(quantV <= 0)
					quantV = 1;
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
			<<"  -y, --quantization-y          Quantization factor Y (for lossy compression)."<<endl
			<<"  -u, --quantization-u          Quantization factor U (for lossy compression)."<<endl
			<<"  -v, --quantization-v          Quantization factor V (for lossy compression)."<<endl
			<<"  -f, --factor                  Factor use to obtain the block size for interframe coding (FrameSize / Factor)."<<endl
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
		//cout<< "Quantization factor: "<< header.quantFactor<<endl;
		cout<< "Predictor: "<< ((header.predictor == 1) ? "Linear" : "Non-Linear")<< " - "<< header.index<<endl;
		cout<< "Golomb factor: "<< header.m<<endl;
		//factor = ((GolombCAVHeader*) &header)->factor;
		bool firstFrame{true};
		Frame *f{nullptr};
		Frame *f2{nullptr};
		Frame *tmp{nullptr};
		while(!end) {
			for(uint i = 0; i < vh->nFrames; i++) { 
				if(!header.block) {
					Predictor pred = Golomb::decode(bs);
					if(firstFrame) {
						cout<< "Using intraframe coding"<<endl;
						cout<< "Block factor: "<< factor<< endl;

						cout<< "Quantization factor Y: "<< pred.quantizationFactorY()<< " U: "<< pred.quantizationFactorU()<< " V: "<< pred.quantizationFactorV()<<endl;
						firstFrame = false;
					}
					f = pred.guess();
					f->display(false,  "VideoPlayback");
				 	waitKey(1.0 / vh->fps * 1000);
					delete f;
				} else {
					if(firstFrame) {
						cout<< "Using interframe coding"<<endl;
						Predictor pred = Golomb::decode(bs);
						Frame *f = pred.guess();
						f->display(false,  "VideoPlayback");
						f2 = new Frame(*f);
						firstFrame = false;
					} else {
						if(tmp != nullptr)
							delete tmp;
						tmp = f2;

						GolombInterframe gi(bs, tmp, nullptr, header.m, tmp->rows() / factor, tmp->cols() / factor);
						f2 = gi.decode(header.m);
						f2->display(false,  "VideoPlayback");
					}
				 	waitKey(1.0 / vh->fps * 1000);
				 	//waitKey(0);
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
			header.factor = factor;
			header.index = pIndex;
			header.m = m;
			header.block = block;
			vHeader.magic = VIDEO_MAGIC; 
			vHeader.nFrames = v->getTotalFrames();
			vHeader.fps = v->fps();

			memcpy(header.undefined, (char*) &vHeader, sizeof(vHeader));

			ss<< dst;
			BitStream bs(ss.str().c_str(), (char*)"wb", (CAVHeader*) &header);
			if(block) {
				cout<< "Inter-frame encoding"<<endl;
				cout<< "Search radius: "<< radius<<endl;
				cout<< "Block factor: "<< factor<< endl;
			} else
				cout<< "Intra-frame encoding"<<endl;
			cout<< "Predictor: "<< ((header.predictor == 1) ? "Linear" : "Non-Linear")<< " - "<< header.index<<endl;
			cout<< "Quantization factor Y: "<< quantY<< " U: "<< quantU<< " V: "<< quantV<<endl;
			cout<< "Golomb factor: "<< m<<endl;
			double enctime{0.0};
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
					p = new LinearPredictor(*f, pIndex, quantY, quantU, quantV);
				else
					p = new NonLinearPredictor(*f, quantY, quantU, quantV);

				if(block)
					enctime += encodeInterframe(prev, f, p, m, bs, radius, factor);
				else
					enctime += encode(f, p, m, bs);

				delete p;
				if(prev != nullptr)
					delete prev;
				prev = f;
			}
			cout<< "Encode Time: "<< enctime<< " By Frame: "<< enctime / vHeader.nFrames <<endl;

			delete v;
		} catch (FileNotFoundException& e) {
			cerr<< "File not found"<< endl;
		}
	}

	return 0;
}
