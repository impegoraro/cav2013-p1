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
	Frame *f;


	if(argc == 1) {
		cerr<< "Missing filename"<<endl;
		exit(1);
	}
	f= Frame::create_from_file((string)argv[1]);
	//for (int i=0; i < f->cols(); i++)
	//	cout<< f->y()[i]<< endl;

	for (int i = 0; i <= 6; i++) {
		stringstream ss;
		LinearPredictor lp(*f, i);
		ss<< "/home/ilan/Downloads/CAV/tmp/golomb-"<< i<< ".gmb";
		cout<<"Main: writing "<< ss.str()<< endl;
		std::vector<int> errors = lp.predict();

		Golomb g(lp, ss.str(), 16);
		g.encode();
		Golomb g2(lp, ss.str(), 16);
		std::vector<int> errors2 = g2.decode();

		Frame444 f2 = lp.guess(errors2, 720, 1280, YUV_444);
		f2.display();
	}


	//for (int i = 0; i <= 6; i++) {
	//	stringstream ss;
	//	LinearPredictor lp(*f, i);
	//	Predictor &p = lp;
	//	errors = move(p.predict());
	//	ss<< "/home/ilan/Downloads/CAV/tmp/golomb-"<< i<< ".gmb";
	//	cout<<"Main: writing "<< ss.str()<< endl;
	//	Golomb g(errors, ss.str(), 4);

	//	g.encode();
	//}


	// for(auto i : errors) {
	// 	if(nl % 20 == 0)
	// 		cout<< endl;
	// 	cout<< i << " ";
	// 	++nl;
	// }
	
	delete f;

	return 0;
}
