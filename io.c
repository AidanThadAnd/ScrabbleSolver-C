#include "io.h"

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
