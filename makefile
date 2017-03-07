all: predictors

predictors: predictors.o 
	g++ -g predictors.o -o predictors

predictors.o: main.cpp predictors.cpp predictors.h
	g++ -c main.cpp predictors.cpp -o predictors.o

clean:
	rm predictors.o predictors
