#include "solver.h"
#include <stdbool.h>

/*
Functions we need
- Find words ()
- DFS algo (recursive)

*/

//DFS
void dfs(TrieNode *node, char *prefix, int *depth, int x, int y, Square board[BOARD_SIZE][BOARD_SIZE], char *combinationToTest, Move foundMoves[], int *totalMovesFound, int direction, int combinationLength){

    if(node == NULL){
        return;
    }

    if(node->isWord){
        Move newMove;
        newMove.row = x;
        newMove.col = y;
        newMove.direction = direction;
        newMove.word = prefix;
        newMove.score = 1;

        foundMoves[*totalMovesFound] = newMove;
        *totalMovesFound += 1;
    }

    if(*depth == 0){
        return;
    }

    strcat(prefix, combinationToTest);
    


    switch (direction){
        case UP:
        if(x > 0){
            dfs(node->children[board[x-1][y].letter - 'A'], prefix, depth+1, x-1, y, board, combinationToTest, foundMoves, totalMovesFound, direction, combinationLength);
        }
        break;
        case DOWN:
        if(x < BOARD_SIZE - 1){
            dfs(node->children[board[x+1][y].letter - 'A'], prefix, depth+1, x+1, y, board, combinationToTest, foundMoves, totalMovesFound, direction, combinationLength);
        }
        break;
        case LEFT:
        if(y > 0){
            dfs(node->children[board[x][y-1].letter - 'A'], prefix, depth+1, x, y-1, board, combinationToTest, foundMoves, totalMovesFound, direction, combinationLength);
        }
        break;
        case RIGHT:
        if(y < BOARD_SIZE - 1){
            dfs(node->children[board[x][y+1].letter - 'A'], prefix, depth+1, x, y+1, board, combinationToTest, foundMoves, totalMovesFound, direction, combinationLength);
        }
        break;
    }
    
}

void findMoves(TrieNode *root, Move foundMoves[], int *totalMovesFound, Square board[BOARD_SIZE][BOARD_SIZE], char *combinationsToTest[], int totalCombinations){

    for (int i = 0; i < totalCombinations; i++){
        int combinationLength = strlen(combinationsToTest[i]);
        dfs(root, "", 0, 0, 0, board, combinationsToTest[i], foundMoves, totalMovesFound, UP, combinationLength);
        dfs(root, "", 0, 0, 0, board, combinationsToTest[i], foundMoves, totalMovesFound, DOWN, combinationLength);
        dfs(root, "", 0, 0, 0, board, combinationsToTest[i], foundMoves, totalMovesFound, LEFT, combinationLength);
        dfs(root, "", 0, 0, 0, board, combinationsToTest[i], foundMoves, totalMovesFound, RIGHT, combinationLength);
    }
}




// Function to swap two characters
void swap(char *x, char *y) {
    char temp = *x;
    *x = *y;
    *y = temp;
}

// Function to print all permutations of a string
void permute(char *letters, int left, int right, char *combinations[], unsigned int *totalCombinations) {
    if (left == right) {
        combinations[*totalCombinations] = malloc(strlen(letters) + 1);
        strcpy(combinations[*totalCombinations], letters);
        *totalCombinations += 1;
    } 

    else {
        bool used[256] = { false }; // Optimization to ensure duplicate combinations are not included into the combinations array

        for (int i = left; i <= right; i++) {
            if (!used[(unsigned char)letters[i]]) {
                used[(unsigned char)letters[i]] = true;

                swap(&letters[left], &letters[i]);
                permute(letters, left + 1, right, combinations,totalCombinations);
                swap(&letters[left], &letters[i]);
            }
        }
    }
}


//Loose idea as of now, will convert to full recursion at a later time
void generateCombinationsRecurse(const char *letters, int totalLetters, char *combination, int start, int index, char *combinations[], unsigned int *totalCombinations) {
    if (index > 0) {
        combination[index] = '\0'; // Null-terminate the combination
        permute(combination, 0, index - 1, combinations, totalCombinations);  // Generate all permutations of the current combination
    }

    bool used[256] = { false }; // Optimization to ensure duplicates are not added to combinations array

    for (int i = start; i < totalLetters; i++) {
        if (!used[(unsigned char)letters[i]]) {
            used[(unsigned char)letters[i]] = true;

            combination[index] = letters[i];
            generateCombinationsRecurse(letters, totalLetters, combination, i + 1, index + 1, combinations, totalCombinations);  // Recurse to generate the next combination
        }
    }
}

// Function to sort an array of strings alphabetically,helpful with optimizing backtracking, current implementation is bubble sort, could switch to quicksort or mergesort if needed
void sortArrayAlphabetically(char *array[], unsigned int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (strcmp(array[j], array[j + 1]) > 0) {
                char *temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}


//The only "public" function to generate combinations, did this for the sake of cleaner code in other sections of the codebase
void generateCombinations(const char *letters, char *combinations[], unsigned int *totalCombinations){
    int totalLetters = strlen(letters);
    char currCombination[totalLetters+1];

    generateCombinationsRecurse(letters, totalLetters, currCombination, 0, 0, combinations, totalCombinations);
    sortArrayAlphabetically(combinations, *totalCombinations);
}
