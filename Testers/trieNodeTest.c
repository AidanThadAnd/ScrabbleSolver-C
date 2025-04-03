/*---------- ID HEADER -------------------------------------
/   Author(s):    Aidan Andrews, Kyle Scidmore
/   File Name:    trieNodeTest.c
/
/   File Description:
/     This file is a standalone tester for the Trie data structure implementation.
/     It includes functions to create Trie nodes, insert words, search for words,
/     and free Trie memory. The main test function inserts a set of words into
/     the Trie and then searches for a different set of words, displaying the
/     results to verify the correct functionality of the Trie.
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

TrieNode *createTrieNode(char letter) {
    TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));

    if (node == NULL) {
        perror("Failed to allocate memory");
        exit(1);
    }

    node->letter = letter;
    node->isWord = 0;
    for (int i = 0; i < 26; i++) {
        node->children[i] = NULL;
    }
    return node;
}

void insertWord(TrieNode *root, const char *word){

    TrieNode *current = root; 
    int len = strlen(word);

    for (int i = 0; i < len; i++){ 

        int index = word[i]- 'A';

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
    for (int i = 0; i < 26; i++){
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


void testTrie() {
    TrieNode *root = createTrieNode('\0');

    // Test words 
    const char *words[] = {"APPLE", "BANANA", "GRAPE", "ORANGE", "PEACH", "PEAR"};
    const char *testWords[] = {"APPLE", "MANGO", "PEACH", "GRAPES", "BANANA"};

    // Insert words into Trie
    for (size_t i = 0; i < sizeof(words) / sizeof(words[0]); i++) {
        insertWord(root, words[i]);
    }

    // Search for words in Trie
    printf("Word Search Results:\n");
    for (size_t i = 0; i < sizeof(testWords) / sizeof(testWords[0]); i++) {
        printf("Searching for '%s': %s\n", testWords[i], searchWord(root, testWords[i]) ? "FOUND" : "NOT FOUND");
    }

    // Free memory
    freeTrie(root);
    printf("Trie memory successfully freed.\n");
}

int main() {

    testTrie();
    return 0;

}
