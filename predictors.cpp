/* Predictors.cpp */

#include "predictors.h"
#include <math.h>
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
		in_put temp;

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

	for(unsigned long long i = 0; i < 26; i++) //loop through output vector
	{
		outfile << output[i].num_correct << "," << num_branches << "; ";
		
		if(i == 0 || i == 1 || i == 8 || i == 15 || i == 24)
			outfile << endl; //seperate based on predictor
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

	out_put temp;
	temp.num_correct = count;
	output.push_back(temp);
}

void Predictors::not_always_taken()
{
	unsigned long long count = 0;

	for(unsigned long long i = 0; i < input.size(); i++){
		if(input[i].prediction == 0)
			count++;
	}

	out_put temp;
	temp.num_correct = count;
	output.push_back(temp);
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
		for(int j = 0; j < table_sizes[i]; j++)
			tables[i][j] = initial_state_prediction; //set all to initial
	
		for(unsigned long long j = 0; j < input.size(); j++)
		{
			int index = input[j].address % table_sizes[i];
			
			if(tables[i][index] == 1 && input[j].prediction == 1){
				count++;
			}else if(tables[i][index] == 1 && input[j].prediction == 0){
				tables[i][index]--;
			}else if(tables[i][index] == 0 && input[j].prediction == 0){
				count++;
			}else if(tables[i][index] == 0 && input[j].prediction == 1){
				tables[i][index]++;
			}
		}		

		out_put temp;
		temp.num_correct = count; 
		output.push_back(temp);

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
		for(int j = 0; j < table_sizes[i]; j++)
			tables[i][j] = initial_state_prediction; //set all to initial

		for(unsigned long long j = 0; j < input.size(); j++)
		{
			int index = input[j].address % table_sizes[i];

			if(tables[i][index] > 1 && input[j].prediction == 1){       //correct
				count++; 
				if(tables[i][index] != 3)
					tables[i][index]++;

			}else if(tables[i][index] < 2 && input[j].prediction == 0){ //correct
				count++;
				if(tables[i][index] != 0)
					tables[i][index]--;
			
			}else if(tables[i][index] < 2 && input[j].prediction == 1){ // wrong
				tables[i][index]++;

			}else if(tables[i][index] > 1 && input[j].prediction == 0){ // wrong
				tables[i][index]--;
			}	
		}

		out_put temp;
		temp.num_correct = count; 
		output.push_back(temp);

		count = 0; //reset
	}
}

void Predictors::gshare()
{
	unsigned long long count = 0;
	vector<unsigned int> table (2048, 3);
	int global_history_register[9] = {0,0,0,0,0,0,0,0,0}; //3bit to 11bit masks
	int sizes[9] = {pow(2,3)-1,pow(2,4)-1,pow(2,5)-1, pow(2,6)-1,
					pow(2,7)-1,pow(2,8)-1,pow(2,9)-1, pow(2,10)-1, pow(2,11)-1}; //bits
	
	for (int i = 0; i < 9; i++) //loop through GHRs
	{
		for(unsigned long long j = 0; j < input.size(); j++)
		{
			//branch address and the global history are hashed together
			//mod size of table to grab appropriate number of bits
		
			int index = ((input[j].address ^ (global_history_register[i] & sizes[i])) % 2048);

			if((table[index] & 2) == (input[j].prediction << 1)) //correct
				count++;
			if(input[j].prediction == 1){
				if(table[index] != 3)
					table[index]++;
			}
			else
			{
				if(table[index] != 0)
					table[index]--;
			}

			global_history_register[i] <<= 1;
			global_history_register[i] |= input[j].prediction;
		}

		out_put temp;
		temp.num_correct = count; 
		output.push_back(temp);

		count = 0; //reset
	
	}
}

//selects between gshare and bimodial
void Predictors::tournament()
{
	unsigned long long count = 0;
	int selector_representation = 0; //(0) prefer gshare (1) - weakly prefer ... (3) prefer bimodial
	unsigned long long global_history_register[9]; //3bit to 11bit masks
	//int sizes[9] = {3,4,5,6,7,8,9,10,11};
	int sizes[9] = {pow(2,3)-1,pow(2,4)-1,pow(2,5)-1, pow(2,6)-1,
					pow(2,7)-1,pow(2,8)-1,pow(2,9)-1, pow(2,10)-1, pow(2,11)-1}; //bits
	
	
	vector<unsigned int> gshare_table (2048, 3);
	vector<unsigned int> bimodal_table (2048, 3);
	vector<unsigned int> tournament_table (2048, 3);

	for(int j = 0; j < 9; j++)
	{
		for(unsigned int i = 0; i < input.size(); i++)
		{
			int tIndex = input[i].address % 2048;
			int bIndex = input[i].address % 2048;
			int gIndex = (input[i].address ^ (global_history_register[j] & sizes[j])) % 2048;
			
			unsigned int bState = bimodal_table[bIndex];	
			unsigned int gState = gshare_table[gIndex];
			unsigned int tState = tournament_table[tIndex];

			unsigned int bimodalPredictedValue = bState & 2;
			unsigned int gsharePredictedValue  = gState & 2;

			if(input[i].prediction == 1){
				if(gState != 3)
					gState++;
				if(bState != 3)
					bState++;
			
			}else{
				if(gState != 0)
					gState--;
				if(bState != 0)
					bState--;
			}
			
			global_history_register[j] <<= 1;
			global_history_register[j] |= input[i].prediction;

			gshare_table[gIndex] = gState;
			bimodal_table[bIndex] = bState;
			
			if(gsharePredictedValue == bimodalPredictedValue){
				if((input[i].prediction << 1) == gsharePredictedValue){
					count++;
				}
			}
			else
			{
				//chose gshare prediction
				if((tState & 2) == 2){
					if((input[i].prediction << 1) == gsharePredictedValue){
						count++;
						
						if(tState != 3)
							tState++;
					}
					else{
						if(tState != 0)
							tState--;
					}
				}
				//chose bimodal prediction
				else
				{
					if((input[i].prediction << 1) == bimodalPredictedValue){
						count++;
						if(tState != 0)
							tState--;
					}
					else{
						if(tState != 3)
							tState++;
					}
				}
			}
			tournament_table[tIndex] = tState;
		}

		out_put temp;
		temp.num_correct = count; 
		output.push_back(temp);
		count = 0;
	}
}
