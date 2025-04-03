/*---------- ID HEADER -------------------------------------
/   Author(s):    Aidan Andrews, Kyle Scidmore
/   File Name:    io.c
/
/   File Description:
/     This file implements functions for loading a Scrabble dictionary 
/     from a file into a Trie data structure for word lookup.
/
/---------------------------------------------------------*/
#include "io.h"

/*---------- FUNCTION: loadDictionary -----------------------------------
/   Function Description:
/     Loads words from a specified file into a Trie data structure.
/     Each word is read, converted to uppercase, and inserted into the Trie.
/     Handles file opening errors and returns the root of the constructed Trie.
/
/   Caller Input:
/     - const char *filename: Path to the dictionary file containing words.
/
/   Caller Output:
/     - TrieNode *: Pointer to the root node of the constructed Trie.
/       Returns NULL and exits the program if file opening fails.
/
/   Assumptions, Limitations, Known Bugs:
/     - Assumes each word in the file is no longer than 15 character.
/     - Exits the program if the file cannot be opened.
/
-------------------------------------------------------------------------*/
TrieNode *loadDictionary(const char *filename){

    FILE *file = fopen(filename, "r");
    if (file == NULL){
        perror("Failed to open dictionary file");
        exit(1);
    }

    TrieNode *root = createTrieNode('\0');
    char word[16];

    while ( fscanf(file, "%15s", word) != EOF){

        for(int i = 0; word[i]; i++){
            word[i] = toupper(word[i]);
        }

        insertWord(root, word);
    }

    fclose(file);
    return root;

}


void printBestMove(Move bestMove, char *rack, Square board[BOARD_SIZE][BOARD_SIZE]) {

    char usedLetters[BOARD_SIZE] = {0};
    findUsedLetters(usedLetters, bestMove.word, rack);
    char *placements = findTilePlacements(board, usedLetters, bestMove);

    printf("Word: %s\nRack Letters Used: %s\nPosition: (%d,%d)\n", bestMove.word, usedLetters, bestMove.row, bestMove.col);

    switch (bestMove.direction) {
        case UP:
            printf("Direction: UP\n");
            break;
        case DOWN:
            printf("Direction: DOWN\n");
            break;
        case LEFT:
            printf("Direction: LEFT\n");
            break;
        case RIGHT:
            printf("Direction: RIGHT\n");
            break;
        default:
            printf(" Invalid direction\n");
    }
    printf("Tile Placements:\n");
    if (strlen(placements) > 0) {
        printf("%s", placements);
    } else {
        printf("No tile placements found.\n");
    }
    printf("Score: %d\n", bestMove.score);

    printf("Board after placement:\n");
    printBoard(board);
    free(placements);
    placements = NULL;
}


char* findTilePlacements(Square board[BOARD_SIZE][BOARD_SIZE], char* usedLetters, Move bestMove) {
    // Dynamically allocate memory for the result string
    char *placements = (char *)malloc(1024 * sizeof(char));
    if (!placements) {
        fprintf(stderr, "Memory allocation failed in findTilePlacements\n");
        return NULL;
    }
    memset(placements, 0, 1024);

    int row = bestMove.row - 1;
    int col = bestMove.col - 1;
    int direction = bestMove.direction;
    int i = 0;

    while ((unsigned long)i < strlen(usedLetters)) {
        switch (direction) {
            case UP: {
                if (board[row - i][col].letter == ' ') {
                    // Place the letter on the board
                    board[row - i][col].letter = usedLetters[i];
                    char temp[128];
                    sprintf(temp, "  Row: %d, Col: %d, Char: %c\n",
                            (row - i + 1), (col + 1), usedLetters[i]);
                    strcat(placements, temp);
                }
                break;
            }
            case DOWN: {
                if (board[row + i][col].letter == ' ') {
                    // Place the letter on the board
                    board[row + i][col].letter = usedLetters[i];
                    char temp[128];
                    sprintf(temp, "  Row: %d, Col: %d, Char: %c\n",
                            (row + i + 1), (col + 1), usedLetters[i]);
                    strcat(placements, temp);
                }
                break;
            }
            case LEFT: {
                if (board[row][col - i].letter == ' ') {
                    // Place the letter on the board
                    board[row][col - i].letter = usedLetters[i];
                    char temp[128];
                    sprintf(temp, "  Row: %d, Col: %d, Char: %c\n",
                            (row + 1), (col - i + 1), usedLetters[i]);
                    strcat(placements, temp);
                }
                break;
            }
            case RIGHT: {
                if (board[row][col + i].letter == ' ') {
                    // Place the letter on the board
                    board[row][col + i].letter = usedLetters[i];
                    char temp[128];
                    sprintf(temp, "  Row: %d, Col: %d, Char: %c\n",
                            (row + 1), (col + i + 1), usedLetters[i]);
                    strcat(placements, temp);
                }
                break;
            }
            default:
                fprintf(stderr, "Invalid direction\n");
                free(placements);
                return NULL;
        }
        i++;
    }
    return placements;
}

void findUsedLetters(char *usedLetters, const char *word, char *rack) {
    int usedIndex = 0;
    for (int i = 0; word[i] != '\0'; i++) {
        for (int j = 0; rack[j] != '\0'; j++) {
            if (word[i] == rack[j]) {
                usedLetters[usedIndex++] = rack[j];
                rack[j] = ' '; // Mark the letter as used
                break;
            }
        }
    }
    usedLetters[usedIndex] = '\0';
}