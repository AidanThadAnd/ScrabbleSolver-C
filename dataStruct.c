#include "dataStruct.h"

//Prototypes
static void changeValidPlacement(Square board[BOARD_SIZE][BOARD_SIZE], int row, int col);
static void checkValidPlacements(Square board[BOARD_SIZE][BOARD_SIZE]);
bool isValidPosition(int row, int col);
void boardDFS(Square board[BOARD_SIZE][BOARD_SIZE], int row, int col, bool **visited);
bool isBoardConnected(Square board[BOARD_SIZE][BOARD_SIZE]);
bool isBoardEmpty(Square board[BOARD_SIZE][BOARD_SIZE]);

TrieNode *createTrieNode(char letter){

    TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));

    if (node == NULL) {
        perror("Failed to allocate memory");
        exit(1);
    }
    node->letter = letter;
    node->isWord = 0;
    for (int i = 0; i < MAX_CHARACTERS; i++){
        node->children[i] = NULL;
    }
    return node;

}

void insertWord(TrieNode *root, const char *word){

    TrieNode *current = root;

    int len = strlen(word);

    for (int i = 0; i < len; i++){
        int index = word[i] - 'A';
        if (current->children[index] == NULL){
            current->children[index] = createTrieNode(word[i]);
        }
        current = current->children[index];
    }
    current->isWord = 1;

}

void freeTrie(TrieNode *node){

    if (node == NULL){
        return;
    }
    for (int i = 0; i < MAX_CHARACTERS; i++){
        freeTrie(node->children[i]);
    }
    free(node);

}


int searchWord(TrieNode *root, const char *word) {

    TrieNode *current = root;

    while (*word) {  // Iterate through the word character by character
        int index = *word - 'A';
        if (index < 0 || index >= 26 || !current->children[index]) {
            return 0; // Word not found
        }
        current = current->children[index];
        word++;  
    }

    return current->isWord; // Return 1 if the current node completes a word
}




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
            board[row][col].validPlacement = false;
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
    checkValidPlacements(board);

    fclose(file);
}



bool isValidPosition(int row, int col) {

    // Check if the position is within the board bounds
    return (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE);

}

void boardDFS(Square board[BOARD_SIZE][BOARD_SIZE], int row, int col, bool **visited) {

    // Base cases: out of bounds, already visited, or empty square
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE || visited[row][col] || board[row][col].letter == ' ') {
        return;
    }

    visited[row][col] = true; // Mark the current square as visited

    // Recursively visit adjacent squares
    boardDFS(board, row - 1, col, visited); // Up
    boardDFS(board, row + 1, col, visited); // Down
    boardDFS(board, row, col - 1, visited); // Left
    boardDFS(board, row, col + 1, visited); // Right
}

bool isBoardConnected(Square board[BOARD_SIZE][BOARD_SIZE]) {

    // Find the first non-empty square to start DFS
    int startRow = -1, startCol = -1;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j].letter != ' ') {
                startRow = i;
                startCol = j;
                break; // Found the start, exit inner loop
            }
        }
        if (startRow != -1) {
            break; // Found the start, exit outer loop
        }
    }

    // If the board is empty, it's considered connected
    if (startRow == -1) {
        return true;
    }

    // Allocate memory for the visited matrix
    bool **visited = (bool **)malloc(BOARD_SIZE * sizeof(bool *));
    for (int i = 0; i < BOARD_SIZE; ++i) {
        visited[i] = (bool *)malloc(BOARD_SIZE * sizeof(bool));
        memset(visited[i], false, BOARD_SIZE * sizeof(bool)); // Initialize to false
    }

    boardDFS(board, startRow, startCol, visited); // Start DFS from the first non-empty square

    // Check if all non-empty squares were visited
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j].letter != ' ' && !visited[i][j]) {
                // Free memory
                for (int k = 0; k < BOARD_SIZE; k++) {
                  free(visited[k]);
                }
                free(visited);
                return false; // Found a disconnected square
            }
        }
    }

    // Free the memory.
    for (int i = 0; i < BOARD_SIZE; i++) {
        free(visited[i]);
    }
    free(visited);

    return true; // All non-empty squares are connected
}

bool isBoardEmpty(Square board[BOARD_SIZE][BOARD_SIZE]) {

    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (board[row][col].letter != ' ') {
                return false;
            }
        }
    }
    return true;
}

bool validateBoard(Square board[BOARD_SIZE][BOARD_SIZE]) {

    if (isBoardEmpty(board)) {
        return true;
    }

    if (board[CENTER][CENTER].letter == ' ' && !isBoardEmpty(board)) {
        printf("Center square is not used!\n");
        return false;
    }

    if (!isBoardConnected(board)) {
        printf("Board is not connected!\n");
        return false;
    }

    return true;
}

void printBoard(Square board[BOARD_SIZE][BOARD_SIZE]) {

    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {

            printf("%c ", (board[row][col].letter == ' ' ? '_' : board[row][col].letter));

        }
        printf("\n");
    }
}

static void checkValidPlacements(Square board[BOARD_SIZE][BOARD_SIZE]){

    for(int row = 0; row < BOARD_SIZE; row++){
        for(int col = 0; col < BOARD_SIZE; col++){
            if(board[row][col].letter != ' '){
                changeValidPlacement(board, row, col);
                board[row][col].validPlacement = true;
            }
        }
    }

    //If board is empty, center square is valid
    if(board[BOARD_SIZE/2][BOARD_SIZE/2].letter == ' '){
        board[BOARD_SIZE/2][BOARD_SIZE/2].validPlacement = true;
    }
}

static void changeValidPlacement(Square board[BOARD_SIZE][BOARD_SIZE], int row, int col){
    if(row + 1 < BOARD_SIZE && board[row+1][col].letter == ' '){
        board[row +1][col].validPlacement = true;
    }
    if(row - 1 < BOARD_SIZE && board[row-1][col].letter == ' '){
        board[row -1][col].validPlacement = true;
    }
    if(col + 1 < BOARD_SIZE && board[row][col+1].letter == ' '){
        board[row][col +1].validPlacement = true;
    }
    if(col - 1 < BOARD_SIZE && board[row][col-1].letter == ' '){
        board[row][col -1].validPlacement = true;
    }
}