#include "io.h"
#include "dataStruct.h"
#include "solver.h"

#define MAX_TOTAL_COMBINATIONS 14700 //Highest amount of total combinations possible with 7 pieces in the players Hand

void testSolver();
void printCombinations(char *combinations[MAX_TOTAL_COMBINATIONS], int totalCombinations);
void testCombinationGenerator();
void printFoundWords(char *foundWords[], int totalWordsFound);
void printFoundMoves(Move foundMoves[], int totalMovesFound);



int main(int argc, char *argv[])
{

    //testCombinationGenerator();

    /*
    if (argc < 2) {
        printf("Missing required inputs");
        return 1;
    }
    */
    
    

    testSolver();

}

void printCombinations(char *combinations[MAX_TOTAL_COMBINATIONS], int totalCombinations){
    
    for(int i = 0; i < totalCombinations; i++){
        printf("%s\n", combinations[i]);
    }

    printf("Total amount of combinations: %i\n", totalCombinations);
}


void printFoundWords(char *foundWords[], int totalWordsFound){
    for(int i = 0; i < totalWordsFound; i++){
        printf("%s\n", foundWords[i]);
    }

    printf("Total amount of words found: %i\n", totalWordsFound);
}

void testCombinationGenerator(){
    char exampleLetters[] = {"ABCDEFG"};

    char *combinations[MAX_TOTAL_COMBINATIONS]; 
    unsigned int totalCombinations;

    generateCombinations(exampleLetters, combinations, &totalCombinations);
    printCombinations(combinations, totalCombinations);
}


void testSolver(){
    char exampleLetters[] = {"ABCDEFG"};
    char *combinations[MAX_TOTAL_COMBINATIONS];
    unsigned int totalCombinations;

    generateCombinations(exampleLetters, combinations, &totalCombinations);

    /*
    printCombinations(combinations, totalCombinations);
    */

    Move *foundMoves = malloc(1000000 * sizeof(Move));
    if (foundMoves == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    exit(1);
    }
    
    int foundCount = 0;
    TrieNode *root = loadDictionary("./dictionary.txt");

    Square board[BOARD_SIZE][BOARD_SIZE];
    initBoard(board);
    loadBoard(board, "./exampleBoard.txt");

    findMoves(root, foundMoves, &foundCount, board, combinations, totalCombinations);
    printFoundMoves(foundMoves, foundCount);
    
    free(foundMoves);
    freeTrie(root);
}

void printFoundMoves(Move foundMoves[], int totalMovesFound){
    for(int i = 0; i < totalMovesFound; i++){
        printf("Row: %i, Col: %i, Direction: %i, Word: %s, Score: %i\n", foundMoves[i].row, foundMoves[i].col, foundMoves[i].direction, foundMoves[i].word, foundMoves[i].score);
    }
}


