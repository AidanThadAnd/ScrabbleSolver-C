#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BOARD_SIZE 15
#define CENTER 7

typedef struct Square {
    char letter;            // The letter on this square (' ' for empty).
    int bonus;             // Bonus type for this square (0: None, 1: Double Letter, 2: Triple Letter, 3: Double Word, 4: Triple Word).
} Square;

void initBoard(Square board[BOARD_SIZE][BOARD_SIZE]) {

    // Hardcoded bonus values
    int doubleLetter[24][2] = {{0,3}, {0,11}, {2,6}, {2,8}, {3,0}, {3,7}, {3,14}, {6,2}, {6,6}, {6,8}, {6,12}, {7,3}, {7,11}, {8,2}, {8,6}, {8,8}, {8,12}, {11,0}, {11,7}, {11,14}, {12,6}, {12,8}, {14,3}, {14,11}};

    int tripleLetter[12][2] = {{1,5}, {1,9}, {5,1}, {5,5}, {5,9}, {5,13}, {9,1}, {9,5}, {9,9}, {9,13}, {13,5}, {13,9}};

    int doubleWord[16][2] = {{1, 1}, {2, 2}, {3, 3}, {4, 4}, {1, 13}, {2, 12}, {3, 11}, {4, 10}, {10, 4}, {11, 3}, {12, 2}, {13, 1}, {10, 10}, {11, 11}, {12, 12}, {13, 13}};

    int tripleWord[8][2] = {{0, 0}, {0, 7}, {0, 14}, {7, 0}, {7, 14}, {14, 0}, {14, 7}, {14, 14}};


    // Initialize all squares to empty
    for(int row = 0; row < BOARD_SIZE; row++){
        for(int col = 0; col < BOARD_SIZE; col++){
            board[row][col].letter = ' ';
            board[row][col].bonus = 0;
        }
    }

    //Assign bonus values to squares
    for(int i = 0; i < 24; i++){
        board[doubleLetter[i][0]][doubleLetter[i][1]].bonus = 1;
    }

    for(int i = 0; i < 12; i++){
        board[tripleLetter[i][0]][tripleLetter[i][1]].bonus = 2;
    }

    for(int i = 0; i < 16; i++){
        board[doubleWord[i][0]][doubleWord[i][1]].bonus = 3;
    }

    for(int i = 0; i < 8; i++){
        board[tripleWord[i][0]][tripleWord[i][1]].bonus = 4;
    }

}

void loadBoard(Square board[BOARD_SIZE][BOARD_SIZE], const char *filename) {

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open board file");
        exit(1);
    }

    char line[BOARD_SIZE + 2]; // +2 to account for newline and null terminator
    for (int row = 0; row < BOARD_SIZE; row++) {
        if (fgets(line, sizeof(line), file) == NULL) {
            fprintf(stderr, "Error: Unexpected end of board at row %d\n", row);
            exit(1);
        }

        for (int col = 0; col < BOARD_SIZE; col++) {
            if (line[col] == '_') {
                board[row][col].letter = ' '; // Treat '_' as an empty space
            } else {
                board[row][col].letter = line[col]; // Copy the character from the file
            }
        }
    }

    fclose(file);
}

//TODO: Validate board function (Ensure center letter is used, no isolated words, etc.)

void printBoard(Square board[BOARD_SIZE][BOARD_SIZE]) {

    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {

            printf("%c ", (board[row][col].letter == ' ' ? '_' : board[row][col].letter));

        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        return 1;
    }

    Square board[BOARD_SIZE][BOARD_SIZE];
    initializeBoard(board);
    loadBoard(board, argv[1]);

    printBoard(board);
    return 0;
}












