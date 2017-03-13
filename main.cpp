/* Main.cpp */

using namespace std;

#include "predictors.h"

#include <map>

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
	predictor.bimodial_single_bit();
	predictor.bimodial_double_bit();
	predictor.gshare();
	predictor.tournament();

	predictor.write_file(argv[2]);

	cout << "Success - output.txt written.\n";
	return 0;
}
