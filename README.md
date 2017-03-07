# branch_predictors

The goal of this project is to measure the effectiveness of various branch direction
prediction (“taken” or “non-taken”) schemes on several traces of conditional branch
instructions. Each trace contains a variable number of branch instructions, and for
each branch, the program counter (PC, word address is given) and the actual outcome
of the branch are recorded in a trace line. Several trace files are provided for
evaluating your predictor designs.

Your goal is to write a program in C or C++ that would use these traces to measure
the accuracy of various branch predictors that we studied in class. The branch
outcomes from the trace file should be used to train your predictors. 
