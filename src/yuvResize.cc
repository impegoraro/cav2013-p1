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

void YuvResize::reduce(Frame& f, Frame& f2, int factor){
        
        int r, c; // row, column in the original frame
        int i ,j; // pixels created by the resizing
        int rIdx = 0; // pixel positions in resized buffer

        int sumY, sumU, sumV;
        int summed;
        //Y
        for(r = 0 ; r < f.y().rows() ; r += factor) {
                for(c = 0 ; c < f.y().rows(); c += factor) {
                        
                        // the value of the new pixel is the average of the reduced(removed) pixels
                        /*if (improved){
                                sumY = 0;
                                summed = 0;
                                //~ f2.yBufferRaw[rIdx];
                                for (int i=-factor/2; i<factor/2;i++)
                                        for (int j=-factor/2; j<factor/2; j++){
                                                if (r+i >=0 && r+i<f.nRows && c+j>=0 && c+j<=f.nCols){
                                                        sumY += f.yBufferRaw[(r+i)*f.nCols + (c+j)];
                                                        summed++;
                                                }
                                        }
                                f2.yBufferRaw[rIdx]=sumY/summed;
                        } else {*/
                                f2.y()rIdx] = f.y()[r*f.y().cols() + c];
                        //}
                        rIdx++;
                }
        }

        //U and V
        rIdx = 0;
        for(r = 0 ; r < f.rows() ; r += factor) {
                for(c = 0 ; c < f.cols() ; c += factor) {
                        
                       /* if (improved){
                                sumU = 0;
                                sumV = 0;
                                summed = 0;
                                for (int i=-factor/2; i<factor/2;i++)
                                        for (int j=-factor/2; j<factor/2; j++){
                                                if (r+i >=0 && r+i<f.nRows && c+j>=0 && c+j<=f.nCols){
                                                        sumU+= f.uBufferRaw[(r+i)*f.nCols + (c+j)];
                                                        sumV+= f.vBufferRaw[(r+i)*f.nCols + (c+j)];
                                                        summed++;
                                                }
                                        }
                                f2.uBufferRaw[rIdx]=sumU/summed;
                                f2.vBufferRaw[rIdx]=sumV/summed;
                        } else {*/
                                f2.u()[rIdx] = f.u()[r*f.uvCols + c];
                                f2.v()[rIdx] = f.v()[r*f.uvCols + c];
                        //}
                        rIdx++;
                }
        }
}
        
void YuvResize::expand(Frame& f, Frame& f2, factor){
        
        int r, c; // row, column in the original frame
        int i ,j; // pixels created by the resizing
        int rIdx, oIdx = 0; // pixel positions in original and resized buffers

        // for bilinear interpolation
        uint val; //current pixel value
        uint rpos, rval; // righ pixel x position, right pixel value;
        uint bpos, bval; // bottom pixel y position, bottom pixel value;
        uint rbval; // right-bottom pixel value, position is (rpos, bpos)
        
        uint tival, bival;// top "line" interpolation value, bottom "line" interpolation value
        
        //Y
        for(r = 0 ; r < f.y().rows() ; r++) {
                for(c = 0 ; c < f.y().cols() ; c++) {
                        rIdx = r*factor*f2.y().cols() + c*factor;
                        /*
                        if (improved){
                                val = f.yBufferRaw[r*f.nCols + c];
                                
                                rpos = (r+1)*factor;
                                if ( c + 1 < f.nCols){ // handle interpolation with pixels out of bounds
                                        rval = f.yBufferRaw[r*f.nCols + c +1];
                                } else {
                                        rval = f.yBufferRaw[r*f.nCols + c]; //keep last pixel value
                                }
                                bpos = (c+1)*factor;
                                
                                if ( r + 1 < f.nRows){ // handle interpolation with pixels out of bounds
                                        bval = f.yBufferRaw[(r+1)*f.nCols + c];
                                } else {
                                        bval = f.yBufferRaw[r*f.nCols + c]; //keep last pixel value
                                }
                                
                                if ( r + 1 < f.nRows && c + 1 < f.nCols){
                                        bval = f.yBufferRaw[(r+1)*f.nCols + c + 1];
                                } else {
                                        bval = f.yBufferRaw[r*f.nCols + c];
                                }
                        }*/
                        
                        for(i = 0 ; i < factor ; i++) {
                                for(j = 0 ; j < factor ; j++) {
                                        /*if (improved) {
                                                tival = interpolate(r, val, rpos, rval, r+i);
                                                bival = interpolate(r, bval, rpos, rbval, r+i);
                                                
                                                f2.yBufferRaw[rIdx + j + i*f2.nCols] = interpolate(c, tival, bpos, bival, c+j);
                                        } else {*/
                                                f2.y()[rIdx + j + i*f2.y().cols()] = f.y()[oIdx];        
                                        //}
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
/*
int YuvResize::interpolate(int a, int aVal, int b, int bVal, int c){
        return aVal + (((c-a)*bVal-(c-a)*aVal)/(b-a));
}*/

int main(int argc, char** argv)
{
	int nextOp, factor;
	uint rows = 1, cols = 1;
	char *src = NULL, *dst = NULL;
	string oper;
	const char* shortops = "hs:d:o:f";
	const struct option longops[] = {
		"help", 0, NULL, 'h',
		"factor", 1, NULL, 'f',
		"operation", 1, NULL, 'o',
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
			case 'o':
				if(oper != 'E' && oper != 'Expand' && oper != 'expand') {
				oper = 'EX';
				}
				else if(oper != 'R' && oper != 'Reduce' && oper != 'reduce') {
				oper = 'RE';
				}
				else {return 1} // erro
			break;
			case 'f':
				if(factor <= 0) return 2; // erro
				factor = atoi(optarg);
			break;
			case '?':
				nextOp = -1;
			break;
		}
	} while(nextOp !=- 1);

	if(argc != 4 || !strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) {
		cerr<< "Usage: yuvResize [OPTIONS] -s <src1> -d <dest> -o <op> -f <factor>"<<endl<<endl;
		cout<<"  -h, --help           Shows this help message."<<endl
			<<"  -s, --source         Specifies the input video filepath."<<endl
			<<"  -d, --destination    Specifies the output video filepath."<<endl
			<<"  -o, --operation      Operation that will take place: Reduce (R) / Expand (E)."<<endl
			<<"  -f, --factor         Factor used by the operation 1:F."<<endl
			<< "The program is able to resize the source by a factor."<<endl;
			<< "Univesidade de Aveiro 2013 - MIETC Audio and Video Coding"<<endl;
			<< "Authors:"<<endl;
			<< "    Ilan Pegoraro N. 41450"<<endl;
			<< "    Luis Neves    N. 41528"<<endl;
		return 1;
	}

	try {
		int newRows = (oper==EX ? orig.nRows*factor : orig.nRows/factor);
        int newCols = (oper==EX ? orig.nCols*factor : orig.nCols/factor);

		string path(argv[1]);
		string path2(argv[2]);
		Video vsrc((string)src);
		Video dsrc((string)dst, newRows, newCols, vsrc.fps(), vsrc.format());

		Frame *f, *f2;
		int end = false;
		
		while(!end) {
			try {
				
				f = vsrc.getFrame();
				f2 = new Frame(newRows, newCols);
				dsrc.putFrame((oper==EX ? expand(*f2) : reduce(*f2)));
			} catch (VideoEndedException& e) {
				end = true;
				continue;
			}
			delete f;
		}
		
		printf("Y: %f, U:%f, V:%f\n", sumY/(float)frames, sumU/(float)frames, sumV/(float)frames);


	} catch (FileNotFoundException& e) {
		cerr<< "File not found"<< endl;
	}
	
	return 0;
}
