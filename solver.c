#include "solver.h"
#include <stdbool.h>

static void findStartingSquare(const int x, const int y, const int direction, const int currentCombinationIndex, Move *move);
static void reverseString(char *oldString, char *newString);

// DFS
static void dfs(TrieNode *head, char *prefix, int *depth, int x, int y, Square board[BOARD_SIZE][BOARD_SIZE], char *combinationToTest, Move foundMoves[], int *totalMovesFound, int direction, int *currentCombinationIndex)
{
    if (x < 0 || x > BOARD_SIZE || y < 0 || y > BOARD_SIZE)
    {
        return;
    }

    if (*currentCombinationIndex > (int)strlen(combinationToTest))
    {
        return;
    }
    if(*currentCombinationIndex == 0){
        if(direction == UP && board[y+1][x].validPlacement){
            return;
        }
        if(direction == DOWN && board[y-1][x].validPlacement){
            return;
        }
        if(direction == LEFT && board[y][x+1].validPlacement){
            return;
        }
        if(direction == RIGHT && board[y][x-1].validPlacement){
            return;
        }
    }
    
    
    while (board[y][x].letter != ' ' && y < BOARD_SIZE && x < BOARD_SIZE) // Small issue with recursion where words at the bounds are returned early due to board[y][x].letter != ' ' returning true when accessing elements outside of the board
    {
        strncat(prefix, &board[y][x].letter, 1);
        *depth += 1;

        switch (direction)
        {
        case UP:
            y -= 1;
            break;
        case DOWN:
            y += 1;
            break;
        case LEFT:
            x -= 1;
            break;
        case RIGHT:
            x += 1;
            break;
        }
    }

    char *reversePrefix = malloc(strlen(prefix) + 1);
    reverseString(prefix, reversePrefix);

    int reversePrefixIsWord = 0;
    
    if(direction == UP || direction == LEFT){
        reversePrefixIsWord = searchWord(head, reversePrefix);
    }


    if ((reversePrefixIsWord || searchWord(head, prefix)) && *currentCombinationIndex == (int)strlen(combinationToTest))
    {
        Move newMove;

        newMove.direction = direction;
        newMove.score = *depth;
        if(reversePrefixIsWord){
            strcpy(newMove.word, reversePrefix);
            newMove.row = y;
            newMove.col = x;
        } else {
        strcpy(newMove.word, prefix);
        findStartingSquare(x, y, direction, *depth, &newMove);
        }
        foundMoves[*totalMovesFound] = newMove;
        *totalMovesFound += 1;
    }

    strncat(prefix, &combinationToTest[*currentCombinationIndex], 1);

    *depth += 1;
    *currentCombinationIndex += 1;
    free(reversePrefix);

    switch (direction)
    {
    case UP:
        dfs(head, prefix, depth, x, y - 1, board, combinationToTest, foundMoves, totalMovesFound, direction, currentCombinationIndex);

        break;
    case DOWN:
        dfs(head, prefix, depth, x, y + 1, board, combinationToTest, foundMoves, totalMovesFound, direction, currentCombinationIndex);

        break;
    case LEFT:
        dfs(head, prefix, depth, x - 1, y, board, combinationToTest, foundMoves, totalMovesFound, direction, currentCombinationIndex);

        break;
    case RIGHT:
        dfs(head, prefix, depth, x + 1, y, board, combinationToTest, foundMoves, totalMovesFound, direction, currentCombinationIndex);

        break;
    }
}

static void reverseString(char *oldString, char *newString)
{
    int length = strlen(oldString);
    for (int i = 0; i < length; i++)
    {
        newString[i] = oldString[length - i - 1];
    }
    newString[length] = '\0';
}

static void findStartingSquare(const int x, const int y, const int direction, const int combinationLength, Move *move)
{
    switch (direction)
    {
    case UP:
        move->row = y + combinationLength;
        move->col = x;
        break;
    case DOWN:
        move->row = y - combinationLength;
        move->col = x;
        break;
    case LEFT:
        move->col = x + combinationLength;
        move->row = y;
        break;
    case RIGHT:
        move->col = x - combinationLength;
        move->row = y;
        break;
    }
}

static void resetValues(int *depth, char *prefix, int *currentCombinationIndex)
{
    *currentCombinationIndex = 0;
    *depth = 0;
    for (int i = 0; i < BOARD_SIZE + 1; i++)
    {
        prefix[i] = '\0';
    }
}

void findMoves(TrieNode *root, Move foundMoves[], int *totalMovesFound, Square board[BOARD_SIZE][BOARD_SIZE], char *combinationsToTest[], int totalCombinations)
{
    for (int i = 0; i < totalCombinations; i++)
    {
        int currentCombinationIndex = 0;
        int depth = 0;
        char prefix[BOARD_SIZE + 1] = "";
        int x = 0;
        int y = 0;

        for (x = 0; x < BOARD_SIZE; x++)
        {
            for (y = 0; y < BOARD_SIZE; y++)
            {
                if (board[y][x].validPlacement)
                {
                    dfs(root, prefix, &depth, x, y, board, combinationsToTest[i], foundMoves, totalMovesFound, UP, &currentCombinationIndex);
                    resetValues(&depth, prefix, &currentCombinationIndex);
                    dfs(root, prefix, &depth, x, y, board, combinationsToTest[i], foundMoves, totalMovesFound, DOWN, &currentCombinationIndex);
                    resetValues(&depth, prefix, &currentCombinationIndex);
                    dfs(root, prefix, &depth, x, y, board, combinationsToTest[i], foundMoves, totalMovesFound, LEFT, &currentCombinationIndex);
                    resetValues(&depth, prefix, &currentCombinationIndex);
                    dfs(root, prefix, &depth, x, y, board, combinationsToTest[i], foundMoves, totalMovesFound, RIGHT, &currentCombinationIndex);
                    resetValues(&depth, prefix, &currentCombinationIndex);
                }
            }
        }
    }
}

// Function to swap two characters
static void swap(char *x, char *y)
{
    char temp = *x;
    *x = *y;
    *y = temp;
}

// Function to print all permutations of a string
static void permute(char *letters, int left, int right, char *combinations[], unsigned int *totalCombinations)
{
    if (left == right)
    {
        combinations[*totalCombinations] = malloc(strlen(letters) + 1);
        strcpy(combinations[*totalCombinations], letters);
        *totalCombinations += 1;
    }

    else
    {
        bool used[256] = {false}; // Optimization to ensure duplicate combinations are not included into the combinations array

        for (int i = left; i <= right; i++)
        {
            if (!used[(unsigned char)letters[i]])
            {
                used[(unsigned char)letters[i]] = true;

                swap(&letters[left], &letters[i]);
                permute(letters, left + 1, right, combinations, totalCombinations);
                swap(&letters[left], &letters[i]);
            }
        }
    }
}

// Loose idea as of now, will convert to full recursion at a later time
static void generateCombinationsRecurse(const char *letters, int totalLetters, char *combination, int start, int index, char *combinations[], unsigned int *totalCombinations)
{
    if (index > 0)
    {
        combination[index] = '\0';                                           // Null-terminate the combination
        permute(combination, 0, index - 1, combinations, totalCombinations); // Generate all permutations of the current combination
    }

    bool used[256] = {false}; // Optimization to ensure duplicates are not added to combinations array

    for (int i = start; i < totalLetters; i++)
    {
        if (!used[(unsigned char)letters[i]])
        {
            used[(unsigned char)letters[i]] = true;

            combination[index] = letters[i];
            generateCombinationsRecurse(letters, totalLetters, combination, i + 1, index + 1, combinations, totalCombinations); // Recurse to generate the next combination
        }
    }
}

// Function to sort an array of strings alphabetically, helpful with optimizing backtracking, current implementation is bubble sort, could switch to quicksort or mergesort if needed
static void sortArrayAlphabetically(char *array[], unsigned int size)
{
    for (unsigned int i = 0; i < size - 1; i++)
    {
        for (unsigned int j = 0; j < size - i - 1; j++)
        {
            if (strcmp(array[j], array[j + 1]) > 0)
            {
                char *temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

// The only "public" function to generate combinations, did this for the sake of cleaner code in other sections of the codebase
void generateCombinations(const char *letters, char *combinations[], unsigned int *totalCombinations)
{
    int totalLetters = strlen(letters);
    char currCombination[totalLetters + 1];

    generateCombinationsRecurse(letters, totalLetters, currCombination, 0, 0, combinations, totalCombinations);
    sortArrayAlphabetically(combinations, *totalCombinations);
}

Move pickBestMove(Move foundMoves[], int totalMovesFound)
{
    Move bestMoveFound = foundMoves[0];
    for (int i = 0; i < totalMovesFound; i++)
    {
        if (foundMoves[i].score > bestMoveFound.score)
        {
            bestMoveFound = foundMoves[i];
        }
    }
    return bestMoveFound;
}