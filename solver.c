/*---------- ID HEADER -------------------------------------
/   Author(s):    Aidan Andrews, Kyle Scidmore
/   File Name:    solver.c
/
/   File Description:
/     This file contains the main solving logic for the Scrabble solver.
/     It includes functions for generating letter combinations, finding valid
/     moves on the board using Depth-First Search (DFS), and selecting the
/     best move based on score.
/
/---------------------------------------------------------*/
#include "solver.h"
#include <stdbool.h>

static void findStartingSquare(const int x, const int y, const int direction, const int currentCombinationIndex, Move *move);
static void reverseString(char *oldString, char *newString);

/*---------- FUNCTION: dfs -----------------------------------
/   Function Description:
/     Performs Depth-First Search (DFS) to find valid word placements on the board.
/
/   Caller Input:
/     - TrieNode *head: Pointer to the root of the Trie.
/     - char *prefix: Current prefix of the word being formed.
/     - int *depth: Pointer to the current depth of the search.
/     - int x: Current column index.
/     - int y: Current row index.
/     - Square board[BOARD_SIZE][BOARD_SIZE]: The game board.
/     - char *combinationToTest: The letter combination to test.
/     - Move foundMoves[]: Array to store found moves.
/     - int *totalMovesFound: Pointer to the total number of moves found.
/     - int direction: Direction of the search (UP, DOWN, LEFT, RIGHT).
/     - int *currentCombinationIndex: Pointer to the current index in the letter combination.
/
/   Caller Output:
/     - void: No return value. Updates the foundMoves array and totalMovesFound.
/
/   Assumptions, Limitations, Known Bugs:
/     - N/A
/---------------------------------------------------------*/
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

/*---------- FUNCTION: reverseString -----------------------------------
/   Function Description:
/     Reverses a given string.
/
/   Caller Input:
/     - char *oldString: The string to be reversed.
/     - char *newString: The buffer to store the reversed string.
/
/   Caller Output:
/     - void: No return value. The reversed string is stored in newString.
/
/   Assumptions, Limitations, Known Bugs:
/     - Assumes newString has enough allocated space.
/---------------------------------------------------------*/
static void reverseString(char *oldString, char *newString)
{
    int length = strlen(oldString);
    for (int i = 0; i < length; i++)
    {
        newString[i] = oldString[length - i - 1];
    }
    newString[length] = '\0';
}

/*---------- FUNCTION: findStartingSquare -----------------------------------
/   Function Description:
/     Calculates the starting row and column coordinates of a word placement
/     based on its ending coordinates, direction, and word length.
/
/   Caller Input:
/     - const int x: The ending column coordinate of the word.
/     - const int y: The ending row coordinate of the word.
/     - const int direction: The direction of the word (UP, DOWN, LEFT, RIGHT).
/     - const int combinationLength: The length of the word.
/     - Move *move: Pointer to the Move structure where the starting coordinates will be stored.
/
/   Caller Output:
/     - void: No return value. The starting row and column are stored directly in the Move structure pointed to by 'move'.
/
/   Assumptions, Limitations, Known Bugs:
/     - Assumes 'move' is a valid pointer to a Move structure.
/     - Does not perform boundary checks; it's assumed the caller ensures valid coordinates.
/---------------------------------------------------------*/
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

/*---------- FUNCTION: resetValues -----------------------------------
/   Function Description:
/     Resets the depth, prefix, and current combination index for a new search.
/
/   Caller Input:
/     - int *depth: Pointer to the depth variable.
/     - char *prefix: Pointer to the prefix string.
/     - int *currentCombinationIndex: Pointer to the combination index.
/
/   Caller Output:
/     - void: No return value. Modifies the input pointers.
/
/   Assumptions, Limitations, Known Bugs:
/     - Assumes prefix has enough allocated memory.
/---------------------------------------------------------*/
static void resetValues(int *depth, char *prefix, int *currentCombinationIndex)
{
    *currentCombinationIndex = 0;
    *depth = 0;
    for (int i = 0; i < BOARD_SIZE + 1; i++)
    {
        prefix[i] = '\0';
    }
}

/*---------- FUNCTION: findMoves -----------------------------------
/   Function Description:
/     Finds all valid moves on the board for a given set of letter combinations.
/
/   Caller Input:
/     - TrieNode *root: Pointer to the root of the Trie.
/     - Move foundMoves[]: Array to store found moves.
/     - int *totalMovesFound: Pointer to the total number of moves found.
/     - Square board[BOARD_SIZE][BOARD_SIZE]: The game board.
/     - char *combinationsToTest[]: Array of letter combinations to test.
/     - int totalCombinations: Total number of combinations.
/
/   Caller Output:
/     - void: No return value. Updates the foundMoves array and totalMovesFound.
/
/   Assumptions, Limitations, Known Bugs:
/     - Assumes Trie and board are correctly initialized.
/     - Assumes BOARD_SIZE is defined.
/---------------------------------------------------------*/
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

/*---------- FUNCTION: swap -----------------------------------
/   Function Description:
/     Swaps two characters.
/
/   Caller Input:
/     - char *x: Pointer to the first character.
/     - char *y: Pointer to the second character.
/
/   Caller Output:
/     - void: No return value. Swaps the characters directly.
/
/   Assumptions, Limitations, Known Bugs:
/     - N/A
/---------------------------------------------------------*/
static void swap(char *x, char *y)
{
    char temp = *x;
    *x = *y;
    *y = temp;
}

/*---------- FUNCTION: permute -----------------------------------
/   Function Description:
/     Generates all permutations of a given string.
/
/   Caller Input:
/     - char *letters: The string to permute.
/     - int left: Starting index for permutation.
/     - int right: Ending index for permutation.
/     - char *combinations[]: Array to store generated combinations.
/     - unsigned int *totalCombinations: Pointer to the total number of combinations.
/
/   Caller Output:
/     - void: No return value. Updates the combinations array and totalCombinations.
/
/   Assumptions, Limitations, Known Bugs:
/     - N/A
/---------------------------------------------------------*/
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

/*---------- FUNCTION: generateCombinationsRecurse -----------------------------------
/   Function Description:
/     Recursively generates letter combinations and their permutations.
/
/   Caller Input:
/     - const char *letters: The input letters.
/     - int totalLetters: Total number of input letters.
/     - char *combination: Buffer to store the current combination.
/     - int start: Starting index for the next letter.
/     - int index: Current index in the combination buffer.
/     - char *combinations[]: Array to store generated combinations.
/     - unsigned int *totalCombinations: Pointer to the total number of combinations.
/
/   Caller Output:
/     - void: No return value. Updates the combinations array and totalCombinations.
/
/   Assumptions, Limitations, Known Bugs:
/     - N/A
/---------------------------------------------------------*/
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

/*---------- FUNCTION: sortArrayAlphabetically -----------------------------------
/   Function Description:
/     Sorts an array of strings alphabetically.
/
/   Caller Input:
/     - char *array[]: Array of strings to sort.
/     - unsigned int size: Size of the array.
/
/   Caller Output:
/     - void: No return value. Sorts the array directly.
/
/   Assumptions, Limitations, Known Bugs:
/     - Uses bubble sort.
/---------------------------------------------------------*/
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

/*---------- FUNCTION: generateCombinations -----------------------------------
/   Function Description:
/     Generates all possible combinations and permutations of the input letters.
/
/   Caller Input:
/     - const char *letters: The input letters to generate combinations from.
/     - char *combinations[]: Array to store the generated combinations.
/     - unsigned int *totalCombinations: Pointer to the variable tracking the number of generated combinations.
/
/   Caller Output:
/     - void: No return value. The generated combinations are stored in the 'combinations' array, and the 'totalCombinations' variable is updated.
/
/   Assumptions, Limitations, Known Bugs:
/     - N/A
/---------------------------------------------------------*/
void generateCombinations(const char *letters, char *combinations[], unsigned int *totalCombinations)
{
    int totalLetters = strlen(letters);
    char currCombination[totalLetters + 1];

    generateCombinationsRecurse(letters, totalLetters, currCombination, 0, 0, combinations, totalCombinations);
    sortArrayAlphabetically(combinations, *totalCombinations);
}

/*---------- FUNCTION: pickBestMove -----------------------------------
/   Function Description:
/     Selects the Move with the highest score from an array of Move structures.
/
/   Caller Input:
/     - Move foundMoves[]: Array of Move structures representing potential moves.
/     - int totalMovesFound: The number of moves in the array.
/
/   Caller Output:
/     - Move: The Move structure with the highest score.
/
/   Assumptions, Limitations, Known Bugs:
/     - Assumes the 'foundMoves' array contains at least one Move.
/---------------------------------------------------------*/
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