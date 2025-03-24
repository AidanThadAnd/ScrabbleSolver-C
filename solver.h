/*---------- ID HEADER -------------------------------------
/   Author(s):    Aidan Andrews, Kyle Scidmore
/   File Name:    solver.h
/
/   File Description:
/     This header file declares functions for the main solving logic of the
/     Scrabble solver. It includes function prototypes for finding valid moves 
/     on the board, generating letter combinations, and selecting the best move.
/
/---------------------------------------------------------*/
#ifndef SOLVER_H
#define SOLVER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "io.h"
#include "dataStruct.h"

#define MAX_TOTAL_COMBINATIONS 150000 // Highest amount of total combinations possible with 7 pieces in the players Hand


//The following functions are only included in the header to enable easier testing in tester.c
void findMoves(TrieNode *root, Move foundMoves[], int *totalMovesFound, Square board[BOARD_SIZE][BOARD_SIZE], char *combinationsToTest[], int totalCombinations);
void generateCombinations(const char *letters, char *combinations[], unsigned int *totalCombinations);
Move pickBestMove(Move foundMoves[], int totalMovesFound);


//The only "public" function to find the best move
Move findBestMove(TrieNode *root, Square board[BOARD_SIZE][BOARD_SIZE], char *rack);

#endif