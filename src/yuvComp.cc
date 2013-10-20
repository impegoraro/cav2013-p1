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

float calcESq(Frame& f, Frame& f2, uint component) {
        //TODO: check dimensions
        
        int lim = 1;
        Block *fBuffer, *f2Buffer;
        
        switch (component){
                case 0:
                        fBuffer = &f.y();
                        f2Buffer = &f2.y();
                        lim = fBuffer->rows() * fBuffer->cols();
                        break;
                case 1:
                        fBuffer = &f.u();
                        f2Buffer = &f2.u();
                        lim = fBuffer->rows() * fBuffer->cols();
                        break;
                case 2:
                        fBuffer = &f.v();
                        f2Buffer = &f2.v();
                        lim = fBuffer->rows() * fBuffer->cols();
                        break;
        }
        
        int sum = 0, i = 0;
        float eSq = 1.0/lim;
        
        for(i = 0 ; i < lim; i++) {
                sum += ((*fBuffer)[i] - (*f2Buffer)[i]) * ((*fBuffer)[i] - (*f2Buffer)[i]);
        }

        eSq *= sum;
        return eSq;
}

float inline calcPSNR(Frame& f, Frame& f2, int component){
        return 10.0 * log10f(255.0 * 255.0 / calcESq(f,f2,component));
}

int main(int argc, char** argv)
{
	if(argc != 3 || !strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) {
		cerr<< "Usage: yuvComp <src1> <src2>"<<endl<<endl;
		cout<< "The program is able to compare in the following formats: YUV444, YUV422 and YUV420."<<endl;
		cout<< "Univesidade de Aveiro 2013 - MIETC Audio and Video Coding"<<endl;
		cout<< "Authors:"<<endl;
		cout<< "    Ilan Pegoraro N. 41450"<<endl;
		cout<< "    Luis Neves    N. 41528"<<endl;
		return 1;
	}

	try {
		string path(argv[1]);
		string path2(argv[2]);
		Video v(path);
		Video v2(path2);

		Frame *f, *f2;
		int end = false;
		uint frames = 0;
        float sumY = 0.0f;
        float sumU = 0.0f;
        float sumV = 0.0f;

		while(!end) {
			try {
				f = v.getFrame();
				f2 = v2.getFrame();
			} catch (VideoEndedException& e) {
				end = true;
				continue;
			}

			sumY += calcPSNR(*f, *f2, 0);
            sumU += calcPSNR(*f, *f2, 1);
            sumV += calcPSNR(*f, *f2, 2);
                        
            frames++;

			delete f;
		}
		
		printf("Y: %f, U:%f, V:%f\n", sumY/(float)frames, sumU/(float)frames, sumV/(float)frames);


	} catch (FileNotFoundException& e) {
		cerr<< "File not found"<< endl;
	}
	
	return 0;
}
