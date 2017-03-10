/* Predictors.cpp */

#include "predictors.h"

using namespace std;

void Predictors::read_file(string file) //trace input
{
	string address;
	string behavior;
	num_branches = 0;

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

		if (behavior == "T") {
			temp.prediction = 1; 
		} else if (behavior == "NT") {
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

void Predictors::bimodial_single_bit() 
{
	unsigned long long count = 0;
	int initial_state_prediction = 1; // T
	
	int table_sizes[7] = {16, 32, 128, 256, 512, 1024, 2048};

	int* tables[7];
	tables[0] = new int[16];
	tables[1] = new int[32];
	tables[2] = new int[128];
	tables[3] = new int[256];
	tables[4] = new int[512];
	tables[5] = new int[1024];
	tables[6] = new int[2048];

	for(int i = 0; i < 7; i++) // loop through each table size
	{
		for(unsigned long long j = 0; j < input.size(); j++)
		{
			int index = input.address % table_sizes[i];
			tables[i][index] = initial_state_prediction;
			
			if(tables[i][index] == input[j].prediction)
				count++;
		}

		_output temp;
		temp.num_correct = count; 
		temp.prediction = "Bimodial Single Bit";
		output[i] = temp;

		count = 0; //reset
	}

}

void Predictors::bimodial_double_bit()
{
	unsigned long long count = 0;
	int initial_state_prediction = 3; //TT
	// 00 = 0 01 = 1 10 = 2 11 = 3

	int table_sizes[7] = {16, 32, 128, 256, 512, 1024, 2048};

	int* tables[7];
	tables[0] = new int[16];
	tables[1] = new int[32];
	tables[2] = new int[128];
	tables[3] = new int[256];
	tables[4] = new int[512];
	tables[5] = new int[1024];
	tables[6] = new int[2048];

	for (int i = 0; i < 7; i++) //loop through tables
	{
		for(unsigned long long j = 0; j < input.size(); j++)
		{
			int index = input.address % table_sizes[i];
			tables[i][index] = initial_state_prediction;

			if(tables[i][index] > 1 && input[j].prediction == 1){       //correct
				count++; 
				if(tables[i][index] != 3)
					tables[i][index]++;

			}else if(tables[i][index] < 1 && input[j].prediction == 0){ //correct
				count++;
				if(tables[i][index] != 0)
					tables[i][index]--;
			
			}else if(tables[i][index] < 1 && input[j].prediction == 1){ // wrong
				tables[i][index]++;

			}else if(tables[i][index] > 1 && input[j].prediction == 0){ // wrong
				tables[i][index]--;
			}	
		}

		_output temp;
		temp.num_correct = count; 
		temp.prediction = "Bimodial Double Bit";
		output[i] = temp;

		count = 0; //reset
	}
}

/* 
gshare 
mod address by table size
then xor by global something 
*/