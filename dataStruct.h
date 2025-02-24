#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_CHARACTERS 26 //For our use case with the Trie this represents all letters A-Z
#define BOARD_SIZE 15
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
    int bonus;             // Bonus type for this square (0: None, 1: Double Letter, 2: Triple Letter, 3: Double Word, 4: Triple Word).
    bool visited;           // Flag indicating if this square has been visited.
} Square;

// Move Structure to store potential plays
typedef struct Move {
    int row;                // Row where the word starts.
    int col;                // Column where the word starts.
    int direction;          // Direction of the word (0: Horizontal, 1: Vertical).
    char *word;             // The word being played.
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


#endif