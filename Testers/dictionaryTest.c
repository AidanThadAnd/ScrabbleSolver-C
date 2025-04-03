/*---------- ID HEADER -------------------------------------
/   Author(s):    Aidan Andrews, Kyle Scidmore
/   File Name:    dictionaryTest.c
/
/   File Description:
/     This file is a standalone tester for the dictionary loading and searching
/     functionality using the Trie data structure. It loads a dictionary from a
/     specified file and tests the presence of several words.
/
/---------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef struct TrieNode {
    char letter;             
    int isWord;              
    struct TrieNode *children[26]; 
} TrieNode;

TrieNode *createTrieNode(char letter){

    TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));

    if (node == NULL) {
        perror("Failed to allocate memory");
        exit(1);
    }
    node->letter = letter;
    node->isWord = 0;
    for (int i = 0; i < 26; i++){
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

int searchWord(TrieNode *root, const char *word) {

    TrieNode *current = root;

    while (*word) {  
        int index = *word - 'A';
        if (index < 0 || index >= 26 || !current->children[index]) {
            return 0; 
        }
        current = current->children[index];
        word++;  
    }

    return current->isWord;
}

void freeTrie(TrieNode *node){

    if (node == NULL){
        return;
    }
    for (int i = 0; i < 26; i++){
        freeTrie(node->children[i]);
    }
    free(node);

}

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

void testDictionary(const char *filename) {

    TrieNode *root = loadDictionary(filename);
    
    if (!root) {
        fprintf(stderr, "Failed to load dictionary.\n");
        return;
    }

    const char *testWords[] = {"APPLE", "MANGO", "PEACH", "GRAPES", "BANANA", "GGGGGGG"};
    int numTests = sizeof(testWords) / sizeof(testWords[0]);

    printf("Dictionary Test Results:\n");
    for (int i = 0; i < numTests; i++) {
        printf("Searching for '%s': %s\n", testWords[i], searchWord(root, testWords[i]) ? "FOUND" : "NOT FOUND");
    }

    freeTrie(root);
    printf("Memory successfully freed.\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <dictionary_file>\n", argv[0]);
        return 1;
    }

    testDictionary(argv[1]);
    return 0;
}
