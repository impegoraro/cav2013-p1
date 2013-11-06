/*
 * yuvCopy.cc
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
#include <string>

#include <opencv2/opencv.hpp>
#include <getopt.h>

#include <libcav/video.h>
#include <libcav/block.h>
#include <libcav/frame.h>
#include <libcav/exceptions/cav-exceptions.h>
#include <libcav/framergb.h>
#include <libcav/frame444.h>
#include <libcav/frame422.h>
#include <libcav/frame420.h>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	int nextOp;
	uint rows = 1, cols = 1;
	bool verbose(false);
	char *src = NULL, *dst = NULL;
	const char* shortops = "vr:c:hs:d:";
	const struct option longops[] = {
		"help", 0, NULL, 'h',
		"rows", 1, NULL, 'r',
		"colums", 1, NULL, 'c',
		"source", 1, NULL, 's',
		"destination", 1, NULL, 'd',
		"verbose", 0, NULL, 'v'
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
			case 'v':
				verbose = true;
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
			<<"  -v, --verbose        Show debugging information and do some checks."<<endl
			<<endl<< "The program is able to copy in the following formats: YUV444, YUV422 and YUV420."<<endl
			<< "Univesidade de Aveiro 2013 - MIECT Audio and Video Coding"<<endl
			<< "Authors:"<<endl
			<< "    Ilan Pegoraro N. 41450"<<endl
			<< "    Luis Neves    N. 41528"<<endl;
		return 1;
	}

	try {
		bool cont(true);
		Video vsrc((string)src);
		Video vdst((string)dst, vsrc.rows(), vsrc.cols(), vsrc.fps(), vsrc.format());

		while(cont) {
			try {
				Frame *f;
				f = vsrc.getFrame();

				if((rows == cols) == 1) {
					vdst.putFrame(*f);
				} else {
					//cout<< "Here"<<endl;
					uint nBlocks = (rows * cols), remaining = f->y().size() % (rows * cols);
					Frame444 f2(f->rows(), f->cols());
					uint i(0);

					for(i = 0; i < f->y().size() - remaining; i+=nBlocks) {
						Block b = std::move(f->y().getSubBlock(i, rows, cols));
						Block b2(1,1);
						f2.y().setSubBlock(i, b);
						
						if(verbose) {
							cout<<"Comparing Y component using sublocks..."<<endl;
							b2 = std::move(f2.y().getSubBlock(i, rows, cols));
							assert(b2 == b);
						}

						b = std::move(f->u().getSubBlock(i, rows, cols));
						f2.u().setSubBlock(i, b);
						
						if(verbose) {
							cout<<"Comparing U component using sublocks..."<<endl;
							b2 = std::move(f2.u().getSubBlock(i, rows, cols));
							assert(b2 == b);
						}


						b = std::move(f->v().getSubBlock(i, rows, cols));
						f2.v().setSubBlock(i, b);
						
						if(verbose) {
							cout<<"Comparing V component using sublocks..."<<endl;
							b2 = std::move(f2.v().getSubBlock(i, rows, cols));
							assert(b2 == b);
						}

					}
					if(remaining) {
						if(verbose)
							cout<< "Copying remaining "<< remaining<< " bytes..."<<endl;
						Block b = std::move(f->y().getSubBlock(i, 1, remaining));
						Block b2(1,1);
						f2.y().setSubBlock(i, b);
						if(verbose) {
							cout<<"Comparing Y component using sublocks..."<<endl;
							b2 = std::move(f2.y().getSubBlock(i, 1, remaining));
							assert(b2 == b);
						}

						b = std::move(f->u().getSubBlock(i, 1, remaining));
						f2.u().setSubBlock(i, b);
						if(verbose) {
							cout<<"Comparing U component using sublocks..."<<endl;
							b2 = std::move(f2.u().getSubBlock(i, 1, remaining));
							assert(b2 == b);
						}


						b = std::move(f->v().getSubBlock(i, 1, remaining));
						f2.v().setSubBlock(i, b);
						if(verbose) {
							cout<<"Comparing V component using sublocks..."<<endl;
							b2 = std::move(f2.v().getSubBlock(i, 1, remaining));
							assert(b2 == b);
						}
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
