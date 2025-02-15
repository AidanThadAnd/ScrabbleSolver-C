#ifndef DATASTRUCT_H
#define DATASTRUCT_H



// Trie Node Structure
typedef struct TrieNode {
    char letter;             // The character stored in the node.
    int isWord;             // Flag indicating if a complete word ends at this node.
    struct TrieNode *children[26]; // Array of pointers to child nodes (one for each letter a-z).
} TrieNode;

// Board Square Structure
typedef struct Square {
    char letter;            // The letter on this square (' ' for empty).
    int bonus;             // Bonus type for this square (0: None, 1: Double Letter, 2: Triple Letter, 3: Double Word, 4: Triple Word).
} Square;

// Move Structure to store potential plays
typedef struct Move {
    int row;                // Row where the word starts.
    int col;                // Column where the word starts.
    int direction;          // Direction of the word (0: Horizontal, 1: Vertical).
    char *word;             // The word being played.
    int score;              // Score of the play.
} Move;

// Function prototypes
TrieNode *createTrieNode(char letter);
void insertWord(TrieNode *root, const char *word);
void freeTrie(TrieNode *node);

#endif