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
		VideoInterface *v;
		VideoInterface *v2;
        uint nFrames;

		Frame *f, *f2;
		int end = false;
        if(path.find(".gmb") != std::string::npos)
            v = new VideoEncoded(path);
        else 
            v = new Video(path);
        if(path2.find(".gmb") != std::string::npos)
            v2 = new VideoEncoded(path2);
        else 
            v2 = new Video(path2);

        nFrames = min(v->getTotalFrames(), v2->getTotalFrames());
        float sumY{0.0}, sumU{0.0}, sumV{0.0};
        float Y, U, V;
        uint i{0};
		while(!end && i < nFrames) {
			try {
				f = v->getFrame();
				f2 = v2->getFrame();
                f->psnr(*f2, Y, U, V);
                sumY += Y;
                sumU += U;
                sumV += V;
            } catch (VideoEndedException& e) {
                end = true;
                continue;
            }
            i++;
            delete f;
        }
        //cout<< "y: " <<sumY<< ", u: "<< sumU<< ", v: "<< sumV<< endl;
        cout<< sumY<< " "<< sumU<< " "<< sumV<< " ";

        delete v;
        delete v2;
	} catch (FileNotFoundException& e) {
		cerr<< "File not found"<< endl;
	}
	return 0;
}
