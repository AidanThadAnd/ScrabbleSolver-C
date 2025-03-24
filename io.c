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
