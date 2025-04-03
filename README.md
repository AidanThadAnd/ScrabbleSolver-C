# Scrabble Board Solver
This project is a C program which solves a given scrabble board with a given input rack

## Important distinctions from regular scrabble
This program has the following slightly different rules to the official scrabble game:
1. No individual letter points, all letters are worth 1 point
2. No additional points by achieving a BINGO (Placing 7-letters in a turn)

## Built with
- **C** - JS runtime

## Building the program
```bash
make all
```
This will create two programs
 - main 
 - solverAlgTest

### Running the programs
- main takes 3 arguments
```bash
./main {boardFile} {inputRack} {dictionaryFile}
```
- solverAlgTest takes no arguments



## Testing plan
1. Testing the solving Algorithm:
 - Build the file "solverAlgTest"
 - Run "solverAlgTest"
    - This program will utilize various input rack's and their known corresponding combinations
       - It will compare the algoirthms combinations to the known combinations, outputting sucess/failure to the output
    - The program will then utilize a Board, Dictionary, and Input to create all the possible moves and compare it to a known file with all the possible moves

2. Testing Board intialization
