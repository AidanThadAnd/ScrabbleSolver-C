#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "dataStruct.h"
#include "solver.h"

#define MAX_TOTAL_COMBINATIONS 150000 // Highest amount of total combinations possible with 7 pieces in the players Hand

int main(int argc, char *argv[]) {

    if (argc < 4) {
        return 1;
    }

    Square board[BOARD_SIZE][BOARD_SIZE];
    initBoard(board);
    loadBoard(board, argv[1]);

    if (!validateBoard(board)) {
        printf("Board validation failed!\n");
        return 1;
    }

    char *rack = argv[2];

    TrieNode *root = loadDictionary(argv[3]);
    
    if (!root) {
        fprintf(stderr, "Failed to load dictionary.\n");
        return 1;
    }

    unsigned int totalCombinations = 0;
    char *combinations[MAX_TOTAL_COMBINATIONS]; 
    generateCombinations(rack, combinations, &totalCombinations);

    Move *foundMoves = malloc(100000 * sizeof(Move));
    if (foundMoves == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    int totalMovesFound = 0;

    findMoves(root, foundMoves, &totalMovesFound, board, combinations, totalCombinations);

    Move bestMove;

    if (totalMovesFound > 0) {

        pickBestMove(foundMoves, totalMovesFound, &bestMove);
        printf("Best move: %s, Score: %d\n", bestMove.word, bestMove.score);

    } else {

        printf("No valid moves found.\n");

    }

    // Free allocated memory for combinations
    for (unsigned int i = 0; i < totalCombinations; i++) {
        free(combinations[i]);
    }

    freeTrie(root);
    return 0;
}


