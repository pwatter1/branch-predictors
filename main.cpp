/* Main.cpp */

#include "predictors.h"

using namespace std;

int main(int argc, char **argv)
{
	if(argc != 3){ 
		cerr << "Accepts only three files: ./predictor input.txt output.txt\n";
		exit(1);
	}

	Predictors predictor; //initialize object

	predictor.read_file(argv[1]);
	predictor.always_taken();
	predictor.not_always_taken();

	int* tables = { new int[16],  new int[32],  new int[128], 
					new int[256], new int[512],	new int[1024], new int[2048] }
	
	predictor.bimodial_single_bit(tables);
	//predictor.bimodial_double_bit(tables);

	predictor.write_file(argv[2]);

	cout << "Success - output.txt written.\n";
	return 0;
}
