#include "predictors.h"

using namespace std;

void Predictors::read_file(string file)
{
	unsigned long long address;
	string behavior;

	ifstream infile(file.c_str()) 

	if(infile == NULL){
		cerr << "CAN'T OPEN FILE: " << file << endl; 
		exit(1);
	}

	// The following loop will read a hexadecimal # 
	// and a string each time and then output them
	while(infile >> hex >> address >> behavior)
	{
		num_branches++;

		stringstream ss; //string builder
		input temp;
s
		address = address.substr(2); //last two counter bits
		ss << address;

		if(behavior == 'T'){
			temp.prediction = 1; 
		}else if(behavior == 'NT'){
			temp.prediction = 0;
		}else{
			cerr << "Undefined branch input\n";
			exit(1); //abort
		}
	}

	infile.close()
}


void Predictors::write_file()
{
	//output file

	ifstream infile(file.c_str());
	if(infile == NULL)

}