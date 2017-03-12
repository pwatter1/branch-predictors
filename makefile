all: predictors

predictors: main.cpp predictors.cpp 
	g++ main.cpp predictors.cpp -o predictors

clean:
	rm predictors.o predictors
