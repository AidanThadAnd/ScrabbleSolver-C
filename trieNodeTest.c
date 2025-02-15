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
        int index = tolower(word[i]) - 'a';
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
        int index = tolower(*word) - 'a';
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
    const char *words[] = {"apple", "banana", "grape", "orange", "peach", "pear"};
    const char *testWords[] = {"apple", "mango", "peach", "grapes", "banana"};

    // Insert words into Trie
    for (int i = 0; i < sizeof(words) / sizeof(words[0]); i++) {
        insertWord(root, words[i]);
    }

    // Search for words in Trie
    printf("Word Search Results:\n");
    for (int i = 0; i < sizeof(testWords) / sizeof(testWords[0]); i++) {
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
