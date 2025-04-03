/*---------- ID HEADER -------------------------------------
/   Author(s):    Aidan Andrews, Kyle Scidmore
/   File Name:    io.h
/
/   File Description:
/     This header file declares functions related to input/output operations,
/     specifically for loading a dictionary from a file into a Trie data structure.
/
/---------------------------------------------------------*/
#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dataStruct.h"


TrieNode *loadDictionary(const char *filename);
void printBestMove(Move bestMove, char *rack, Square board[BOARD_SIZE][BOARD_SIZE]);
char* findTilePlacements(Square board[BOARD_SIZE][BOARD_SIZE], char* usedLetters, Move bestMove);
void findUsedLetters(char *usedLetters, const char *word, char *rack);

#endif