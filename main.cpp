/* Main.cpp */

#include "predictor.h"

using namespace std;

int main(int argc, char **argv)
{
	if(argc != 3){ 
		cerr << "Accepts three files: ./predictor input.txt output.txt\n";
		exit(1);
	}

	Predictor predictor; //initialize object

	predictor.read_file();
	predictor.always_taken();
	predictor.not_always_taken():



	predictor.write_file();

	cout << "Success - output.txt written\n";
	return 0;
}