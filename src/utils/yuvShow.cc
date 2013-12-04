/*
 * yuvShow.cc
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
	bool cont = false, playing = true;
	string ans;
	bool showHelp = false;
	char *src = NULL;
	char nextOp;
	const char* shortops = "rmhs:";
	const struct option longops[] = {
		{"help", 0, NULL, 'h'},
		{"manual-fps", 0, NULL, 'm'},
		{"repeat", 0, NULL, 'r'},
		{"source", 1, NULL, 's'}
	};

	do {
		nextOp = getopt_long(argc, argv, shortops, longops, NULL);
		switch(nextOp) {
			case 'h':
				nextOp = -1;
				showHelp = true;
			break;
			case 'r':
				cont = true;
			break;
			case 'm':
				playing = false;
			break;
			case 's':
				src = optarg;
			break;
			case '?':
			default:
				showHelp = false;
			break;
		}
	} while(nextOp !=- 1);

	if(showHelp) {
		cerr<< "Usage: yuvShow [OPTIONS] <video>"<<endl;
		cout<< "The program is able to play videos in the following formats: RGB, YUV444, YUV422 and YUV420."<<endl
			<<"[OPTIONS]"<<endl
			<<"  -h, --help                    Shows this help message."<<endl
			<<"  -s, --source                  Filename to read the video from. Missing this switch means to read from camera."<<endl
			<<"  -r, --repeat                  Ask to repeat the video when finishes."<<endl
			<<"  -m, --manual-fps              Use any key to step throughout the video instead of using the video's FPS."<<endl<<endl;
		cout<<"yuvShow  Copyright (C) 2013  Universidade de Aveiro  - MIECT Audio and Video Coding"<<endl;
		cout<< "Authors:"<<endl;
		cout<< "    Ilan Pegoraro N. 41450"<<endl;
		cout<< "    Luis Neves    N. 41528"<<endl;
		return 1;
	}
	try {
		VideoInterface *v;
		if(src == NULL) {
			cout<< "Using webcam..."<<endl;
			v = new Video();
		} else {
			cout<< "Using file..."<<endl;
			string path(src);
			if(path.find(".gmb") != std::string::npos)
				v = new VideoEncoded(path);
			else v = new Video(path);
		}
		
		v->display(playing);

		while(cont) {
			cout<< "Do you want to play it again? [y/n]: ";
			cin>> ans;
			if(ans == "y" || ans == "Y") {
				cont = true;
				v->reset();
			}
			else if(ans == "n" || ans == "N") {
				cont = false;
				break;
			}
			cout<< "Total Video Frames are: "<< v->getTotalFrames()<< endl;
			v->display(playing);
		}
		delete v;

	} catch (FileNotFoundException& e) {
		cerr<< "File not found"<< endl;
	}
	
	return 0;
}
