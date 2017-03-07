/* Predictors.cpp */

#include "predictors.h"

#include <string>

using namespace std;

void Predictors::read_file(string file) //trace input
{
	string address;
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
		ss >> hex >> temp.address; 

		if (behavior.compare("T")) {
			temp.prediction = 1; 
		} else if (behavior.compare("NT")) {
			temp.prediction = 0;
		} else {
			cerr << "Undefined branch input.\n";
			exit(1); //abort
		}

		input.push_back(temp); //store values
	}

	infile.close();
}

void Predictors::write_file(string file) //output.txt
{

	ofstream outfile(file.c_str());

	if(outfile == NULL){
		cerr << "Unable to open file: " << file << endl;
		exit(1);
	}

	for(unsigned long long i = 0; i < 26; i++) //loop through output array
	{
		outfile << output[i].num_correct << ", " << num_branches << "; ";
		
		if(i == 0 || i == 1 || i == 8 || i == 15 || i == 24)
			outfile << endl; //seperate based on which predictor
	}

	outfile << endl;
	outfile.close();
}

void Predictors::always_taken()
{
	unsigned long long count = 0;

	for(unsigned long long i = 0; i < input.size(); i++){
		if(input[i].prediction == 1)
			count++;
	}

	_output temp;
	temp.num_correct = count;
	temp.prediction = "Always Taken";
	output[0] = temp;
}

void Predictors::not_always_taken()
{
	unsigned long long count = 0;

	for(unsigned long long i = 0; i < input.size(); i++){
		if(input[i].prediction == 0)
			count++;
	}

	_output temp;
	temp.num_correct = count;
	temp.prediction = "Always Not Taken";
	output[1] = temp;
}

