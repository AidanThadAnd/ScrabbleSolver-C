#include "io.h"
#include "dataStruct.h"
#include "solver.h"

#define MAX_TOTAL_COMBINATIONS 14700 // Highest amount of total combinations possible with 7 pieces in the players Hand

void testSolver();
void printCombinations(char *combinations[MAX_TOTAL_COMBINATIONS], int totalCombinations);
void testCombinationGenerator();
void printFoundWords(char *foundWords[], int totalWordsFound);
void printFoundMoves(Move foundMoves[], int totalMovesFound);
void duplicateFinder(Move foundMoves[], int totalMovesFound);
void checkValidPlacements(Square board[BOARD_SIZE][BOARD_SIZE]);

int main(int argc, char *argv[])
{
    (void)argc; // Temporaily suppress compiler warning
    (void)argv;

    //testCombinationGenerator();

    /*
    if (argc < 2) {
        printf("Missing required inputs");
        return 1;
    }
    */

    testSolver();
}

void printCombinations(char *combinations[MAX_TOTAL_COMBINATIONS], int totalCombinations)
{

    for (int i = 0; i < totalCombinations; i++)
    {
        printf("%s\n", combinations[i]);
    }

    printf("Total amount of combinations: %i\n", totalCombinations);
}

void printFoundWords(char *foundWords[], int totalWordsFound)
{
    for (int i = 0; i < totalWordsFound; i++)
    {
        printf("%s\n", foundWords[i]);
    }

    printf("Total amount of words found: %i\n", totalWordsFound);
}

void testCombinationGenerator()
{
    char exampleLetters[] = {"ABCDEFG"};

    char *combinations[MAX_TOTAL_COMBINATIONS];
    unsigned int totalCombinations;

    generateCombinations(exampleLetters, combinations, &totalCombinations);
    printCombinations(combinations, totalCombinations);
}

void testSolver()
{
    //char exampleLetters[] = {"AEINRST"}; // Leads to the most possible sub words in 7 characters
    //char exampleLetters[] = {"ABCDEFG"}; // Leads to the most possible sub words in 7 characters
    char exampleLetters[] = {"ASOLUTE"}; //Tests 8 letter word generation for ABSOLUTELY
    //char exampleLetters[] = {"QUICKBE"}; //Tests 9 letter word generation for ABSOLUTELY

    char *combinations[MAX_TOTAL_COMBINATIONS];
    unsigned int totalCombinations;

    generateCombinations(exampleLetters, combinations, &totalCombinations);



    Move *foundMoves = malloc(100000 * sizeof(Move));
    if (foundMoves == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    int foundCount = 0;
    TrieNode *root = loadDictionary("./dictionary.txt");

    Square board[BOARD_SIZE][BOARD_SIZE];
    initBoard(board);
    loadBoard(board, "./exampleBoard.txt");

    //checkValidPlacements(board);

    
    findMoves(root, foundMoves, &foundCount, board, combinations, totalCombinations);
    printFoundMoves(foundMoves, foundCount);
    //duplicateFinder(foundMoves, foundCount);
    

    free(foundMoves);
    freeTrie(root);
}

void printFoundMoves(Move foundMoves[], int totalMovesFound)
{
    for (int i = 0; i < totalMovesFound; i++)
    {
        printf("Row: %i, Col: %i, Direction: %i, Word: %s, Score: %i\n", foundMoves[i].row, foundMoves[i].col, foundMoves[i].direction, foundMoves[i].word, foundMoves[i].score);
    }
}

void duplicateFinder(Move foundMoves[], int totalMovesFound)
{
    for (int i = 0; i < totalMovesFound; i++)
    {
        for (int j = 0; j < i; j++)
        {
            if (foundMoves[i].row == foundMoves[j].row &&
                foundMoves[i].col == foundMoves[j].col &&
                foundMoves[i].direction == foundMoves[j].direction &&
                strcmp(foundMoves[i].word, foundMoves[j].word) == 0 &&
                foundMoves[i].score == foundMoves[j].score)
            {
                printf("Duplicate found: Row: %i, Col: %i, Direction: %i, Word: %s, Score: %i\n", foundMoves[i].row, foundMoves[i].col, foundMoves[i].direction, foundMoves[i].word, foundMoves[i].score);
                return;
            }
        }
    }
}

void checkValidPlacements(Square board[BOARD_SIZE][BOARD_SIZE])
{
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            if (board[row][col].validPlacement == true)
            {
                printf("A valid Placement is: Row: %i, Col: %i\n", row, col);
            }
        }
    }
}