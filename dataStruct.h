/*---------- ID HEADER -------------------------------------
/   Author(s):    Aidan Andrews, Kyle Scidmore
/   File Name:    datastruct.h
/
/   File Description:
/     This header file defines data structures and function prototypes used in the
/     Scrabble solver. It includes definitions for the TrieNode (for
/     word validation), Square (for board representation), and Move (for storing
/     potential plays). It also declares constants for board dimensions,
/     directions, and maximum characters.
/
/---------------------------------------------------------*/
#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_CHARACTERS 26 //For our use case with the Trie this represents all letters A-Z
#define BOARD_SIZE 15
#define CENTER 7

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

// Trie Node Structure
typedef struct TrieNode {
    char letter;             // The character stored in the node.
    int isWord;             // Flag indicating if a complete word ends at this node.
    struct TrieNode *children[MAX_CHARACTERS]; // Array of pointers to child nodes (one for each letter A-Z).
} TrieNode;

// Board Square Structure
typedef struct Square {
    char letter;            // The letter on this square (' ' for empty).
    int direction;          // Direction of the word (0: UP, 1: DOWN, 2: Left, 3: Right).
    int bonus;             // Bonus type for this square (0: None, 1: Double Letter, 2: Triple Letter, 3: Double Word, 4: Triple Word).
    bool validPlacement;   // Flag indicating if a letter can be placed on this square.
    bool usedBonus;        // Flag indicating if the bonus has been used. (Used to remove squares used by the input board)
} Square;

// Move Structure to store potential plays
typedef struct Move {
    int row;                // Row where the word starts.
    int col;                // Column where the word starts.
    int direction;          // Direction of the word (0: Horizontal, 1: Vertical).
    char word[BOARD_SIZE+1];// The word being played. (Extra space for null terminator)
    int score;              // Score of the play.
} Move;

// Function prototypes
TrieNode *createTrieNode(char letter);
void insertWord(TrieNode *root, const char *word);
void freeTrie(TrieNode *node);
int searchWord(TrieNode *root, const char *word);

void printBoard(Square board[BOARD_SIZE][BOARD_SIZE]);
void loadBoard(Square board[BOARD_SIZE][BOARD_SIZE], const char *filename);
void initBoard(Square board[BOARD_SIZE][BOARD_SIZE]);
bool validateBoard(Square board[BOARD_SIZE][BOARD_SIZE]);

#endif