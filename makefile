all: predictors

predictors: predictors.o 
	g++ -g predictors.o -o predictors

predictors.o: predictors.cpp predictors.h
	g++ -c predictors.cpp -o predictors.o

run:
	./predictors

clean:
	rm predictors.o predictors
