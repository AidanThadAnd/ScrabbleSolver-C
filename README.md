# Scrabble Board Solver

This project is a C program which solves a given scrabble board with a given input rack

## Important distinctions from regular scrabble

This program has the following slightly different rules to the official scrabble game:

1. No individual letter points, all letters are worth 1 point
2. No additional points by achieving a BINGO (Placing 7-letters in a turn)

## Built with

- **C**

## Building the program

```bash
make all
```

This will create two programs

- main
  - Tester programs:
  - solverAlgTest
  - trieNodeTester
  - boardTester
  - dictionaryTester

### Running the programs

- main takes 3 arguments

```bash
./main {boardFile} {inputRack} {dictionaryFile}
```

- solverAlgTest takes no arguments

## Testing plan

For testing purposes there are 2 different types of tester executables

1. Black-Box test (solverTester)

- Tests the code hollistically as it requires all code ranging from IO, Trie navigation, dictionary loading, board loading and finally solving for all the moves

2. White-box Tests (boardTester, dictionaryTester, trieNodeTester)

- Tests individual functions indepdenet of all other non-essential code
- trieNodeTester does not take an arguments
- boardTester/dictionaryTester both take 1 argument in the syntax

```bash
boardTester/dictionaryTester {boardStateFile/dictionaryFile}
```

### Running of Black-Box test

- Build the file "solverAlgTest"
- Run "solverAlgTest"
  - This program will utilize various input rack's and their known corresponding combinations
    - It will compare the algoirthms combinations to the known combinations, outputting sucess/failure to the output
  - The program will then utilize a Board, Dictionary, and Input to create all the possible moves and compare it to a known file with all the possible moves
