/*
 * yuvConvert.cc
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
#include <opencv2/opencv.hpp>
#include <getopt.h>

#include <libcav.h>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	bool showHelp = false;
	string ans;
	int nextOp;
	string format;
	char *src = NULL, *dst = NULL;
	VideoFormat vf(YUV_444);
	const char* shortops = "s:d:f:h";
	const struct option longops[] = {
		{"help", 0, NULL, 'h'},
		{"format", 1, NULL, 'f'},
		{"destination", 1, NULL, 'd'},
		{"source", 1, NULL, 's'}
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
			case 'f':
				format = string(optarg);
				if(format == "444")
					vf = YUV_444;
				else if(format == "422")
					vf = YUV_422;
				else if(format == "420")
					vf = YUV_420;
				else
					src = dst = NULL;
			break;
			case '?':
			default:
				showHelp = false;
			break;
		}
	} while(nextOp !=- 1);

	if(showHelp || src == NULL || dst == NULL) {
		cerr<< "Usage: yuvConvert [OPTIONS] -s <source> -d <destination>"<<endl;
		cout<< "The program is able to convert between the following formats: RGB, YUV444, YUV422 and YUV420."<<endl;
		cout<<"Options are:"<<endl
			<<"  -h, --help           Shows this help message."<<endl
			<<"  -s, --source         Video source."<<endl
			<<"  -i, --destination    Video destination."<<endl
			<<"  -f  --format         Output format."<<endl<<endl;
		cout<<"yuvConvert  Copyright (C) 2013  Universidade de Aveiro  - MIECT Audio and Video Coding"<<endl
			<< "Authors:"<<endl
			<< "    Ilan Pegoraro N. 41450"<<endl
			<< "    Luis Neves    N. 41528"<<endl;
		return 1;
	}


	try {
		Video v((string)src);
		v.convert((string)dst, vf);
		//Video vdst((string)dst, v.rows(), v.cols(), v.fps(), vf);
		//Frame *f = NULL, f2;
		//int end = false;
		//	
		//while(!end) {
		//	try {
		//		f = v.getFrame();
		//		f2 = std::move(f->convert(vf));
		//		vdst.putFrame(f2);
		//	} catch (VideoEndedException& e) {
		//		end = true;
		//		continue;
		//	}
		//	delete f;
		//}
	} catch (FileNotFoundException& e) {
		cerr<< "File not found"<< endl;
	}
	
	return 0;
}
