#ifndef PREDICTORS_H
#define PREDICTORS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>

using namespace std;

struct _input 
{
	unsigned long long address;
	unsigned int prediction;
}

struct _output
{
	unsigned long long num_correct;
	string prediction;
}


class Predictor
{
	private:
		long long num_branches;
		
		// declare array of structs 
		// to contain each predictor's data
		struct _input input[6];
		struct _output output[6]; 

	public:
		void read_file(string file);
		void write_file(string file);
};



#endif
