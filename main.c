#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "dataStruct.h"
#include "solver.h"

#define MAX_TOTAL_COMBINATIONS 150000 // Highest amount of total combinations possible with 7 pieces in the players Hand

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <board_file> <rack_letters> <dictionary_file>\n", argv[0]);
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
    size_t rack_len = strlen(rack);
    char *upper_rack = malloc(rack_len + 1); // Allocate space for uppercase rack

    if (upper_rack == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    // Convert rack to uppercase
    for (size_t i = 0; i < rack_len; i++) {
        upper_rack[i] = toupper(rack[i]);
    }
    upper_rack[rack_len] = '\0'; // Null-terminate the uppercase string

    TrieNode *root = loadDictionary(argv[3]);

    if (!root) {
        fprintf(stderr, "Failed to load dictionary.\n");
        free(upper_rack); // Free allocated memory before exiting
        return 1;
    }

    unsigned int totalCombinations = 0;
    char *combinations[MAX_TOTAL_COMBINATIONS];
    generateCombinations(upper_rack, combinations, &totalCombinations); // Use uppercase rack

    Move *foundMoves = malloc(100000 * sizeof(Move));
    if (foundMoves == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(upper_rack);
        freeTrie(root);
        return 1;
    }

    int totalMovesFound = 0;

    findMoves(root, foundMoves, &totalMovesFound, board, combinations, totalCombinations);

    Move bestMove = pickBestMove(foundMoves, totalMovesFound);
    printf("Best move: %s, Score: %d, Row: %d, Column:%d", bestMove.word, bestMove.score, bestMove.row, bestMove.col);

    for (unsigned int i = 0; i < totalCombinations; i++) {
        free(combinations[i]);
    }

    free(foundMoves); 
    freeTrie(root);
    free(upper_rack);
    return 0;
}


