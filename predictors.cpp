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
	int initial_state_prediction = 3; //TT - (00) = 0 (01) = 1 (10) = 2 (11) = 3
	int table[2048];
	unsigned long long global_history_register[9]; //3bit to 11bit masks

	global_history_register[0] = 0x0;   //3bit
	global_history_register[1] = 0x0;   //4bit
	global_history_register[2] = 0x00;  //5bit
	global_history_register[3] = 0x00;  //6bit
	global_history_register[4] = 0x00;  //7bit
	global_history_register[5] = 0x00;  //8bit
	global_history_register[6] = 0x000; //9bit
	global_history_register[7] = 0x000; //10bit
	global_history_register[8] = 0x000; //11bit

	for (int i = 0; i < 9; i++) //loop through GHR
	{
		for(int j = 0; j < 2048; j++)
			table[i] = initial_state_prediction; //set all to initial state

		for(unsigned long long j = 0; j < input.size(); j++)
		{
			//branch address and the global history are hashed together
			//mod size of table to grab appropriate number of bits
			int index = ((input[j].address ^ global_history_register[i]) % 2048);

			if(table[index] > 1 && input[j].prediction == 1){       //correct
				count++; 
				if(table[index] != 3)
					table[index]++;

			}else if(table[index] < 2 && input[j].prediction == 0){ //correct
				count++;
				if(table[index] != 0)
					table[index]--;
			
			}else if(table[index] < 2 && input[j].prediction == 1){ // wrong
				table[index]++;

			}else if(table[index] > 1 && input[j].prediction == 0){ // wrong
				table[index]--;
			}	
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
	int initial_state_prediction = 3; //TT - (00) = 0 (01) = 1 (10) = 2 (11) = 3
	int selector_representation = 0; //(0) prefer gshare (1) - weakly prefer ... (3) prefer bimodial
	unsigned long long global_history_register[9]; //3bit to 11bit masks
	int gshare_table[2048];
	int bimodial_table[2048];
	int selector_table[2048];

	global_history_register[0] = 0x0;   //3bit
	global_history_register[1] = 0x0;   //4bit
	global_history_register[2] = 0x00;  //5bit
	global_history_register[3] = 0x00;  //6bit
	global_history_register[4] = 0x00;  //7bit
	global_history_register[5] = 0x00;  //8bit
	global_history_register[6] = 0x000; //9bit
	global_history_register[7] = 0x000; //10bit
	global_history_register[8] = 0x000; //11bit

	for(int j = 0; j < 2048; j++) //set each to initial
		bimodial_tables[j] = initial_state_prediction; 

	//set up bimodial
	for(unsigned long long j = 0; j < input.size(); j++)
	{
		int index = input[j].address % 2048;
		
		if(bimodial_tables[index] == 1 && input[j].prediction == 0)
			bimodial_table[index]--;
		else if(bimodial_table[index] == 0 && input[j].prediction == 1)
			bimodial_table[index]++;
	}		

	//set up gshare
	for (int i = 0; i < 9; i++) //loop through global history register
	{
		for(int j = 0; j < 2048; j++)
			gshare_table[i] = initial_state_prediction; //set all to initial state

		for(unsigned long long j = 0; j < input.size(); j++)
		{
			//branch address and the global history are hashed together
			//mod size of table to grab appropriate number of bits
			int index = ((input[j].address ^ global_history_register[i]) % 2048);

			if(table[index] > 1 && input[j].prediction == 1){       //correct
				if(table[index] != 3)
					table[index]++;

			}else if(table[index] < 2 && input[j].prediction == 0){ //correct
				if(table[index] != 0)
					table[index]--;
			
			}else if(table[index] < 2 && input[j].prediction == 1){ // wrong
				table[index]++;

			}else if(table[index] > 1 && input[j].prediction == 0){ // wrong
				table[index]--;
			}	
		}
	}

	//compare predictions 
	for(unsigned long long i = 0; i < input.size(); i++)
	{
		for(int j = 0; j < 2048; j++)
		{
			if(bimodial_table[i] == gshare_table[i])
			{
				if(bimodial_table[i] == input[i].prediction)
					count++;
			}
			else
			{
				if(selector_table[i] == 2) //taking gshare
				{
					if(bimodial_table[i] == input[i].prediction)
					{
						count++;
						if(selector_table[i] != 3)
							selector_table[i]++; //shift to prefer bimodial
					}
					else
					{
						if(selector_table[i] != 0)	
							selector_table[i]--;
					}
				}
				else //taking bimodial
				{
					if(gshare_table[i] == input[i].prediction)
					{
						count++;
						if(selector_table[i] != 3)
							selector_table[i]++; //shift to prefer bimodial
					}
					else
					{
						if(selector_table[i] != 0)	
							selector_table[i]--;
					}
				}
			}

		}
	}

	out_put temp;
	temp.num_correct = count; 
	output.push_back(temp);
}
