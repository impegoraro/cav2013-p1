/*
 * yuvResize.cc
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
#include <getopt.h>
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

void my_reduce(Frame& f, Frame& f2, uint factor){
        
        uint r, c; // row, column
        uint rIdx = 0; // pixel index in resized frame
		
        //Y
        for(r = 0 ; r < f.y().rows() ; r += factor) {
                for(c = 0 ; c < f.y().rows(); c += factor) {
						f2.y()[rIdx] = f.y()[r*f.y().cols() + c];
                        rIdx++;
                }
        }

        //U and V
        rIdx = 0;
        for(r = 0 ; r < f.rows() ; r += factor) {
                for(c = 0 ; c < f.cols() ; c += factor) {
						f2.u()[rIdx] = f.u()[r*f.u().cols() + c];
						f2.v()[rIdx] = f.v()[r*f.u().cols() + c];
						rIdx++;
                }
        }
}
        
void my_expand(Frame& f, Frame& f2, uint factor){
        
        uint r, c; // row, column
        uint i ,j;
        uint rIdx, oIdx = 0; 	// pixel index in original and resized frames
        
        //Y
        for(r = 0 ; r < f.y().rows() ; r++) {
                for(c = 0 ; c < f.y().cols() ; c++) {
                        rIdx = r*factor*f2.y().cols() + c*factor;
                        
                        for(i = 0 ; i < factor ; i++) {
                                for(j = 0 ; j < factor ; j++) {  
                                    f2.y()[rIdx + j + i*f2.y().cols()] = f.y()[oIdx];
                                }
                        }
                        oIdx++;
                }
        }
        
        
        //U and V
        oIdx = 0;
        for(r = 0 ; r < f.rows() ; r++) {
                for(c = 0 ; c < f.cols() ; c++) {
                        rIdx = r*factor*f2.cols() + c*factor;
                        for(i = 0 ; i < factor ; i++) {
                                for(j = 0 ; j < factor ; j++) {
                                        f2.u()[rIdx + j + i*f2.cols()] = f.u()[oIdx];
                                        f2.v()[rIdx + j + i*f2.cols()] = f.v()[oIdx];
                                }
                        }
                        oIdx++;
                }
        }
}

int main(int argc, char** argv)
{
	int nextOp, factor = 0;
	char *src = NULL, *dst = NULL;
    bool showHelp = false;
	string oper;
	const char* shortops = "hs:d:o:f:";
	const struct option longops[] = {
		{"help", 0, NULL, 'h'},
		{"factor", 1, NULL, 'f'},
		{"operation", 1, NULL, 'o'},
		{"source", 1, NULL, 's'},
		{"destination", 1, NULL, 'd'}
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
			case 'o':
				if(oper != "E" && oper != "Expand" && oper != "expand") {
				oper = "EX";
				}
				else if(oper != "R" && oper != "Reduce" && oper != "reduce") {
				oper = "RE";
				}
				else {showHelp = true; nextOp = -1; break;}
			break;
			case 'f':
				factor = atoi(optarg);
                if(factor <= 0) {showHelp = true; nextOp = -1; break;}
			break;
			case '?':
				nextOp = -1;
			break;
		}
	} while(nextOp !=- 1);

	if(showHelp) {
		cerr<< "Usage: yuvResize [OPTIONS] <src1> <dest> <op> <factor>"<<endl;
		cout<< "The program is able to resize the source by a factor."<<endl;
		cout<<"  -h, --help           Shows this help message."<<endl
			<<"  -s, --source         Specifies the input video filepath."<<endl
			<<"  -d, --destination    Specifies the output video filepath."<<endl
			<<"  -o, --operation      Operation that will take place: Reduce (R) / Expand (E)."<<endl
			<<"  -f, --factor         Factor used by the operation 1:F."<<endl;
		cout<<"yuvResize  Copyright (C) 2013  Universidade de Aveiro  - MIETC Audio and Video Coding"<<endl;
		cout<< "Authors:"<<endl;
		cout<< "    Ilan Pegoraro N. 41450"<<endl;
		cout<< "    Luis Neves    N. 41528"<<endl;
        showHelp = false;
		return 1;
	}

	try {

        string path(argv[1]);
        string path2(argv[2]);
        Video vsrc((string)src);
		int newRows = (oper=="EX" ? vsrc.rows()*factor : vsrc.rows()/factor);
        int newCols = (oper=="EX" ? vsrc.cols()*factor : vsrc.cols()/factor);
        Video dsrc((string)dst, newRows, newCols, vsrc.fps(), vsrc.format());

        Frame *f, *f2;
        int end = false;
		while(!end) {
			try {
				
				f = vsrc.getFrame();
				f2 = new Frame(newRows, newCols);
                if(oper=="EX")
                    my_expand(*f, *f2,factor);
                else
                    my_reduce(*f,*f2, factor);
				dsrc.putFrame(*f2);
			} catch (VideoEndedException& e) {
				end = true;
				continue;
			}
			delete f;
		}
	} catch (FileNotFoundException& e) {
		cerr<< "File not found"<< endl;
	}
	return 0;
}
