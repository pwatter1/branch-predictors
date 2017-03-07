/* Predictors.cpp */

#include "predictors.h"

using namespace std;

void Predictors::read_file(string file)
{
	unsigned long long address;
	string behavior;

	ifstream infile(file.c_str());

	if(infile == NULL){
		cerr << "Unable to open file: " << file << endl; 
		exit(1); //abort
	}

	// The following loop will read a hexadecimal # 
	// as well as a string each time, then output them
	while(infile >> hex >> address >> behavior)
	{
		num_branches++;

		stringstream ss; //string builder
		_input temp;

		address = address.substr(2); //last two counter bits
		ss << address;
		hex >> temp.address; 

		if(behavior == 'T'){
			temp.prediction = 1; 
		}else if(behavior == 'NT'){
			temp.prediction = 0;
		}else{
			cerr << "Undefined branch input\n";
			exit(1); //abort
		}

		input.push_back(temp); //store
	}

	infile.close();
}

void Predictors::write_file(string file) //output.txt
{

	ofstream outfile(file.c_str());

	if(infile == NULL){
		cerr << "CAN'T OPEN FILE: " << file << endl;
		exit(1);
	}

	for(unsigned long long i = 0; i < 6; i++) //loop through output array
	{

	}

	outfile.close();
}

void Predictors::always_taken()
{
	unsigned long long count = 0;

	for(unsigned long long i = 0; i < _input.size(); i++){
		if(_input.prediction == 1)
			count++;
	}

	_output temp;
	temp.num_correct = count;
	temp.prediction = "Always Taken";
	output[0] = temp;
}

void not_always_taken()
{
	unsigned long long count = 0;

	for(unsigned long long i = 0; i < _input.size(); i++){
		if(_input.prediction == 0)
			count++;
	}

	_output temp;
	temp.num_correct = count;
	temp.prediction = "Always Not Taken";
	output[1] = temp;
}
