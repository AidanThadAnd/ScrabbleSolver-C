#include "solver.h"
#include <stdbool.h>


/* FUNCTION PROTOTYPES*/
static void findStartingSquare(const int x, const int y, const int direction, const int currentCombinationIndex, Move *move);
static void reverseString(char *oldString, char *newString);
static void dfs(TrieNode *head, char *prefix, int *depth, int x, int y, Square board[BOARD_SIZE][BOARD_SIZE], char *combinationToTest, Move foundMoves[], int *totalMovesFound, int direction, int *currentCombinationIndex);
static void resetValues(int *depth, char *prefix, int *currentCombinationIndex);
static void findMovesForBoard(
    TrieNode *root,
    Move foundMoves[],
    int *totalMovesFound,
    Square board[BOARD_SIZE][BOARD_SIZE],
    char *combination,
    int x,
    int y,
    int *depth,
    char *prefix,
    int *currentCombinationIndex);
static void findMovesForCombination(
    TrieNode *root,
    Move foundMoves[],
    int *totalMovesFound,
    Square board[BOARD_SIZE][BOARD_SIZE],
    char *combination,
    int combinationIndex);
void findMoves(
    TrieNode *root,
    Move foundMoves[],
    int *totalMovesFound,
    Square board[BOARD_SIZE][BOARD_SIZE],
    char *combinationsToTest[],
    int totalCombinations);
static void swap(char *x, char *y);
static void permute(char *letters, int left, int right, char *combinations[], unsigned int *totalCombinations);
static void generateCombinationsRecurse(const char *letters, int totalLetters, char *combination, int start, int index, char *combinations[], unsigned int *totalCombinations);




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

static void resetValues(int *depth, char *prefix, int *currentCombinationIndex)
{
    *currentCombinationIndex = 0;
    *depth = 0;
    for (int i = 0; i < BOARD_SIZE + 1; i++)
    {
        prefix[i] = '\0';
    }
}

static void findMovesForBoard(
    TrieNode *root,
    Move foundMoves[],
    int *totalMovesFound,
    Square board[BOARD_SIZE][BOARD_SIZE],
    char *combination,
    int x,
    int y,
    int *depth,
    char *prefix,
    int *currentCombinationIndex)
{
    if (x >= BOARD_SIZE)
    {
        return;
    }

    if (y >= BOARD_SIZE)
    {
        findMovesForBoard(root, foundMoves, totalMovesFound, board, combination, x + 1, 0, depth, prefix, currentCombinationIndex);
        return;
    }

    if (board[y][x].validPlacement)
    {
        dfs(root, prefix, depth, x, y, board, combination, foundMoves, totalMovesFound, UP, currentCombinationIndex);
        resetValues(depth, prefix, currentCombinationIndex);

        dfs(root, prefix, depth, x, y, board, combination, foundMoves, totalMovesFound, DOWN, currentCombinationIndex);
        resetValues(depth, prefix, currentCombinationIndex);

        dfs(root, prefix, depth, x, y, board, combination, foundMoves, totalMovesFound, LEFT, currentCombinationIndex);
        resetValues(depth, prefix, currentCombinationIndex);

        dfs(root, prefix, depth, x, y, board, combination, foundMoves, totalMovesFound, RIGHT, currentCombinationIndex);
        resetValues(depth, prefix, currentCombinationIndex);
    }

    findMovesForBoard(root, foundMoves, totalMovesFound, board, combination, x, y + 1, depth, prefix, currentCombinationIndex);
}

static void findMovesForCombination(
    TrieNode *root,
    Move foundMoves[],
    int *totalMovesFound,
    Square board[BOARD_SIZE][BOARD_SIZE],
    char *combination,
    int combinationIndex)
{
    if (combinationIndex >= 1) // base case to avoid redundant calls
    {
        return;
    }

    int currentCombinationIndex = 0;
    int depth = 0;
    char prefix[BOARD_SIZE + 1] = "";
    findMovesForBoard(root, foundMoves, totalMovesFound, board, combination, 0, 0, &depth, prefix, &currentCombinationIndex);
}

void findMoves(
    TrieNode *root,
    Move foundMoves[],
    int *totalMovesFound,
    Square board[BOARD_SIZE][BOARD_SIZE],
    char *combinationsToTest[],
    int totalCombinations)
{
    if (totalCombinations <= 0)
    {
        return;
    }

    findMovesForCombination(root, foundMoves, totalMovesFound, board, combinationsToTest[0], 0);

    // Recurse to handle the rest of the combinations
    findMoves(root, foundMoves, totalMovesFound, board, combinationsToTest + 1, totalCombinations - 1);
}

// Function to swap two characters
static void swap(char *x, char *y)
{
    char temp = *x;
    *x = *y;
    *y = temp;
}

static void permuteRecursive(char *letters, int left, int right, char *combinations[], unsigned int *totalCombinations, bool used[], int i)
{
    if (i > right)
    {
        return;
    }

    unsigned char c = (unsigned char)letters[i];
    if (!used[c])
    {
        used[c] = true;
        swap(&letters[left], &letters[i]);
        permute(letters, left + 1, right, combinations, totalCombinations);
        swap(&letters[left], &letters[i]);
    }

    permuteRecursive(letters, left, right, combinations, totalCombinations, used, i + 1);
}

static void permute(char *letters, int left, int right, char *combinations[], unsigned int *totalCombinations)
{
    if (left == right)
    {
        combinations[*totalCombinations] = malloc(strlen(letters) + 1);
        strcpy(combinations[*totalCombinations], letters);
        (*totalCombinations)++;
        return;
    }

    bool used[256] = { false };
    permuteRecursive(letters, left, right, combinations, totalCombinations, used, left);
}

static void generateCombinationsRecursive(const char *letters, int totalLetters, char *combination, int i, int index, char *combinations[], unsigned int *totalCombinations, bool used[])
{
    if (i >= totalLetters)
    {
        return;
    }

    unsigned char c = (unsigned char)letters[i];
    if (!used[c])
    {
        used[c] = true;
        combination[index] = letters[i];
        generateCombinationsRecurse(letters, totalLetters, combination, i + 1, index + 1, combinations, totalCombinations);
    }

    generateCombinationsRecursive(letters, totalLetters, combination, i + 1, index, combinations, totalCombinations, used);
}

static void generateCombinationsRecurse(const char *letters, int totalLetters, char *combination, int start, int index, char *combinations[], unsigned int *totalCombinations)
{
    if (index > 0)
    {
        combination[index] = '\0';
        permute(combination, 0, index - 1, combinations, totalCombinations);
    }

    bool used[256] = { false };
    generateCombinationsRecursive(letters, totalLetters, combination, start, index, combinations, totalCombinations, used);
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

Move findBestMove(TrieNode *root, Square board[BOARD_SIZE][BOARD_SIZE], char *rack)
{
    size_t rack_len = strlen(rack);
    char *upper_rack = malloc(rack_len + 1); 
    unsigned int totalCombinations = 0;
    char *combinations[MAX_TOTAL_COMBINATIONS];
    int totalMovesFound = 0;

    for (size_t i = 0; i < rack_len; i++) {
        upper_rack[i] = toupper(rack[i]);
    }
    upper_rack[rack_len] = '\0'; 

    generateCombinations(upper_rack, combinations, &totalCombinations); 

    Move *foundMoves = malloc(100000 * sizeof(Move));
    if (foundMoves == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(upper_rack);
        freeTrie(root);
    }

    

    findMoves(root, foundMoves, &totalMovesFound, board, combinations, totalCombinations);

    Move bestMove = pickBestMove(foundMoves, totalMovesFound);

    free(upper_rack);
    free(foundMoves);
    for (unsigned int i = 0; i < totalCombinations; i++) {
        free(combinations[i]);
    }

    return bestMove;
}