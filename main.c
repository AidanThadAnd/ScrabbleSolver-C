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
    printf("Best move: %s, Score: %d, Row: %d, Column:%d", bestMove.word, bestMove.score, bestMove.row, bestMove.col);

    freeTrie(root);
    return 0;
}


