/*---------- ID HEADER -------------------------------------
/   Author(s):    Aidan Andrews, Kyle Scidmore
/   File Name:    main.c
/
/   File Description:
/     This file is the main entry point for the Scrabble solver.
/     It takes a board file, rack letters, and a dictionary file as command-line
/     arguments. It loads the game board, generates letter combinations from
/     the rack, finds valid moves using the dictionary, and determines the
/     best move based on score. It then prints the best move details to the
/     console.
/
/---------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "dataStruct.h"
#include "solver.h"

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <board_file> <rack_letters> <dictionary_file>\n", argv[0]);
        return 1;
    }
    char *rack = argv[2];
    TrieNode *root = loadDictionary(argv[3]);
    Square board[BOARD_SIZE][BOARD_SIZE];

    initBoard(board);
    loadBoard(board, argv[1]);
    
    if (!validateBoard(board)) {
        printf("Board validation failed!\n");
        return 1;
    }

    if (!root) {
        fprintf(stderr, "Failed to load dictionary.\n");
        return 1;
    }

    Move bestMove = findBestMove(root, board, rack);
    printBestMove(bestMove, rack, board);
    
    freeTrie(root);
    return 0;
}

