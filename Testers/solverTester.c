/*---------- ID HEADER -------------------------------------
/   Author(s):    Aidan Andrews, Kyle Scidmore
/   File Name:    tester.c
/
/   File Description:
/     This file is a comprehensive tester for the Scrabble solver.
/     It includes functions to test the combination generator and the solver's
/     move finding logic. It loads a dictionary, sets up test scenarios with
/     example letters and board configurations, and compares the solver's
/     output against expected results.
/
/---------------------------------------------------------*/
#include "../io.h"
#include "../dataStruct.h"
#include "../solver.h"
#include <stdio.h>

#define MAX_TOTAL_COMBINATIONS 150000 // Highest amount of total combinations possible with 7 pieces in the players Hand

#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define RESET "\033[0m"

void testSolver(char exampleLetters[], TrieNode *root);
void strToLower(const char *src, char *dst);
void validateCombinations(char exampleLetters[], char *combinations[MAX_TOTAL_COMBINATIONS], int totalCombinations);
void printCombinations(char *combinations[MAX_TOTAL_COMBINATIONS], int totalCombinations);
void testCombinationGenerator(char exampleLetters[]);
void printFoundWords(char *foundWords[], int totalWordsFound);
void printFoundMoves(Move foundMoves[], int totalMovesFound);
void duplicateFinder(Move foundMoves[], int totalMovesFound);
void checkValidPlacements(Square board[BOARD_SIZE][BOARD_SIZE]);

int main()
{
    char testLetters1[] = {"ICKBEAM"}; 
    char testLetters2[] = {"ABSOLU"};  
    char testLetters3[] = {"ABCDEFG"}; 
    TrieNode *root = loadDictionary("Testers/solverTests/dictionary.txt");

    testCombinationGenerator(testLetters1);
    testCombinationGenerator(testLetters2);
    testCombinationGenerator(testLetters3);

    testSolver(testLetters1, root);
    testSolver(testLetters2, root);
    testSolver(testLetters3, root);
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

void testCombinationGenerator(char exampleLetters[])
{
    char *combinations[MAX_TOTAL_COMBINATIONS];
    unsigned int totalCombinations = 0;

    generateCombinations(exampleLetters, combinations, &totalCombinations);
    validateCombinations(exampleLetters, combinations, totalCombinations);
}

void validateCombinations(char exampleLetters[], char *combinations[MAX_TOTAL_COMBINATIONS], int totalCombinations)
{
    bool error = false;
    char filePath[256];
    snprintf(filePath, sizeof(filePath), "Testers/solverTests/%s/%sCombinations.txt", exampleLetters, exampleLetters);
    FILE *file = fopen(filePath, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Failed to open file: %s\n", filePath);
        return;
    }

    char line[256];
    for (int i = 0; i < totalCombinations; i++)
    {

        if (fgets(line, sizeof(line), file) == NULL)
        {
            fprintf(stderr, "Error: Not enough lines in file for combination %s\n", combinations[i]);
            fclose(file);
            return;
        }

        // Remove newline character from the line read from the file
        line[strcspn(line, "\n")] = '\0';

        if (strcmp(line, combinations[i]) != 0)
        {
            error = true;
        }
    }
    printf("%s[ %s ]%s Combination test: %s\n",
           (!error) ? GREEN : RED,
           (!error) ? "PASSED" : "FAILED",
           RESET,
           exampleLetters);

    fclose(file);
}

void validateSolver(Move foundMoves[], int totalMovesFound, char exampleLetters[])
{
    bool error = false;
    char filePath[256];
    snprintf(filePath, sizeof(filePath), "Testers/solverTests/%s/%sOutput.txt", exampleLetters, exampleLetters);
    FILE *file = fopen(filePath, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Failed to open file: %s\n", filePath);
        return;
    }

    char line[256];
    for (int i = 0; i < totalMovesFound; i++)
    {

        if (fgets(line, sizeof(line), file) == NULL)
        {
            fprintf(stderr, "Error: Not enough lines in file for combination %s\n", exampleLetters);
            fclose(file);
            return;
        }
        char moveString[256];
        snprintf(moveString, sizeof(moveString), "Row: %i, Col: %i, Direction: %i, Word: %s, Score: %i",
                 foundMoves[i].row, foundMoves[i].col, foundMoves[i].direction, foundMoves[i].word, foundMoves[i].score);

        // Remove newline character from the line read from the file
        line[strcspn(line, "\n")] = '\0';

        if (strcmp(line, moveString) != 0)
        {
            printf("Expected: %s\n", line);
            printf("Found: %s\n", moveString);
            error = true;
        }
    }
    printf("%s[ %s ]%s Valid Move Test: %s\n",
           (!error) ? GREEN : RED,
           (!error) ? "PASSED" : "FAILED",
           RESET,
           exampleLetters);

    fclose(file);
}

void testSolver(char exampleLetters[], TrieNode *root)
{
    char *combinations[MAX_TOTAL_COMBINATIONS];
    unsigned int totalCombinations = 0;

    generateCombinations(exampleLetters, combinations, &totalCombinations);

    Move *foundMoves = malloc(100000 * sizeof(Move));
    if (foundMoves == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    int foundCount = 0;

    Square board[BOARD_SIZE][BOARD_SIZE];
    initBoard(board);

    char filePath[256];
    char lowerExampleLetters[7];

    strToLower(exampleLetters, lowerExampleLetters);
    snprintf(filePath, sizeof(filePath), "Testers/solverTests/%s/%sBoard.txt", lowerExampleLetters, lowerExampleLetters);

    loadBoard(board, filePath);

    findMoves(root, foundMoves, &foundCount, board, combinations, totalCombinations);

    validateSolver(foundMoves, foundCount, exampleLetters);

    free(foundMoves);
}

void strToLower(const char *src, char *dst)
{
    while (*src)
    {
        *dst = tolower((unsigned char)*src);
        src++;
        dst++;
    }
    *dst = '\0';
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