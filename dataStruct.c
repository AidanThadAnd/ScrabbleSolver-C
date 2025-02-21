#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dataStruct.h"


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

void freeTrie(TrieNode *node){

    if (node == NULL){
        return;
    }
    for (int i = 0; i < 26; i++){
        freeTrie(node->children[i]);
    }
    free(node);

}



