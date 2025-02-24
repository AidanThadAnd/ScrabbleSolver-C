#ifndef SOLVER_H
#define SOLVER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "io.h"
#include "dataStruct.h"

void findMoves(TrieNode *root, Move foundMoves[], int *totalMovesFound, Square board[BOARD_SIZE][BOARD_SIZE], char *combinationsToTest[], int totalCombinations);
void generateCombinations(const char *letters, char *combinations[], unsigned int *totalCombinations);


#endif