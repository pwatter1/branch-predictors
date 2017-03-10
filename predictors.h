/* Predictors.h */

#ifndef PREDICTORS_H
#define PREDICTORS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h>

using namespace std;

struct in_put 
{
	unsigned long long address;
	unsigned int prediction; // NT or T
};

struct out_put
{
	unsigned long long num_correct; //use index to tell which predictor
};


class Predictors
{
	private:
		unsigned long long num_branches;
		vector<in_put> input; 
		vector<out_put> output; 

	public:
		void read_file(string file);
		void write_file(string file);

		void always_taken();
		void not_always_taken();

		void bimodial_single_bit();
		void bimodial_double_bit(); // 00 01 10 11

		void gshare();

		//void tournament();
};



#endif
