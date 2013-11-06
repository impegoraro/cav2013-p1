/*
 * yuvComp.cc
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

#include <libcav.h>

using namespace std;
using namespace cv;

float calcESq(Frame& f, Frame& f2, uint component) {
        //TODO: check dimensions
        
        int lim(1);
        int sum(0);
        float eSq(0);
        Block *fBuffer(NULL), *f2Buffer(NULL);

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
        assert(fBuffer != NULL && f2Buffer != NULL); // safety check, usually not needed.
        eSq = 1.0/lim;
        
        for(int i = 0; i < lim; i++) {
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
		cerr<< "Usage: yuvComp <src1> <src2>"<<endl;
		cout<< "The program is able to compare in the following formats: YUV444, YUV422 and YUV420."<<endl<<endl;
        cout<<"yuvComp  Copyright (C) 2013  Universidade de Aveiro  - MIECT Audio and Video Coding"<<endl;
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
