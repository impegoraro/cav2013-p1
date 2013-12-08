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
	bool showHelp = false, showFrame(false);
	string ans;
	int nextOp;
	uint count = -1;
	char *src = NULL, *dst = NULL;
	const char* shortops = "fs:d:hn:";
	const struct option longops[] = {
		{"help", 0, NULL, 'h'},
		{"frame", 0, NULL, 'f'},
		{"number-frames", 1, NULL, 'n'},
		{"output-directory", 1, NULL, 'd'},
		{"source", 1, NULL, 's'}
	};
	struct stat dstat;


	// Parsing command switches.
	do {
		nextOp = getopt_long(argc, argv, shortops, longops, NULL);
		switch(nextOp) {
			case 'h':
				nextOp = -1;
				showHelp = true;
			break;
			case 'f':
				showFrame = true;
			break;
			case 's':
				src = optarg;
			break;
			case 'd':
				dst = optarg;
			break;
			case 'n':
				count = std::stoi((string)optarg);
			break;
			case '?':
			default:
				showHelp = false;
			break;
		}
	} while(nextOp !=- 1);

	if(showHelp || (showFrame && src == NULL) || (!showFrame && dst == NULL)) {
		cerr<< "Usage: frameExtractor [OPTIONS] -s <source> -d <dir>"<<endl;
		cout<< "The program is able to extract frames to a directory in the following formats: RGB, YUV444, YUV422 and YUV420."<<endl;
		cout<<"Options are:"<<endl
			<<"  -h, --help                  Shows this help message."<<endl
			<<"  -d, --output-directory      Directory were to place the extracted frames."<<endl
			<<"  -n, --number-frames         Number of frames to extract, negative number means all frames."<<endl
			<<"  -f, --show-frame            Displays frame."<<endl
			<<"  -s, --source                Video file path."<<endl<<endl;

		cout<<"frameExtractor  Copyright (C) 2013  Universidade de Aveiro  - MIECT Audio and Video Coding <"<<endl
			<< "Authors:"<<endl
			<< "    Ilan Pegoraro N. 41450"<<endl
			<< "    Luis Neves    N. 41528"<<endl;
		exit(0);
	}

	if(showFrame) {
		Frame * f = Frame::create_from_file((string)src);

		f->display();
		delete f;
	} else {
		if(stat(dst, &dstat)) {
			cerr<< strerror(errno)<<endl<<"Aborting..."<<endl;
			exit(1);
		}
		
		if(!S_ISDIR(dstat.st_mode)) {
			cerr<<"Destination file must be a directory."<<endl<<"Aborting..."<<endl;
			exit(2);
		}

		if(!((dstat.st_mode & S_IWUSR) == S_IWUSR) && !((dstat.st_mode & S_IWGRP) == S_IWGRP) && !((dstat.st_mode & S_IWOTH) == S_IWOTH)) {
			cerr<< "Unable to write to the directory."<<endl<<"Aborting..."<<endl;
			exit(3);
		}

		try {
			Video *v;
			string frameName;

			if(src == NULL) {
				v = new Video(0);
				frameName  = ((string) "/camera-");
			}
			else {
				string path(src);
				frameName  = ((string) "/") + ((string) basename(src)) + (string)"-";
				v = new Video(path);
			}
			
			Frame *f = NULL;
			uint i = 0;

			while(i < count) {
				try {
					stringstream ss;
					ss<< i;
					f = v->getFrame();
					f->write(((string) dst) + frameName + ss.str() + ".yuv");
					delete f;
				} catch (VideoEndedException& e) {
					break;
				}
				++i;
			}
			delete v;
		} catch (FileNotFoundException& e) {
			cerr<< "File not found"<< endl;
		}
	}
	return 0;
}
