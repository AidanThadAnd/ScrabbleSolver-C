/*---------- ID HEADER -------------------------------------
/   Author(s):    Aidan Andrews, Kyle Scidmore
/   File Name:    trieNodeTest.c
/
/   File Description:
/     This file is a standalone tester for the Trie data structure implementation.
/     It includes functions to create Trie nodes, insert words, search for words,
/     and free Trie memory. The main test function inserts a set of words into
/     the Trie and then searches for a different set of words, comparing the
/     results to verify the correct functionality of the Trie.
/
/---------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../dataStruct.h"

#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define RESET "\033[0m"
#define validOutput "./Testers/trieTests/validOutput.txt"

int compareResultsToValidOutput(const char *results[], int numResults, const char *validOutputFile);

int testTrie() {
    TrieNode *root = createTrieNode('\0');

    if (!root) {
        fprintf(stderr, "Failed to create Trie root.\n");
        return 0;
    }

    // Test words
    const char *words[] = {"APPLE", "BANANA", "GRAPE", "ORANGE", "PEACH", "PEAR"};
    const char *testWords[] = {"APPLE", "MANGO", "PEACH", "GRAPES", "BANANA"};
    int numTests = sizeof(testWords) / sizeof(testWords[0]);

    // Insert words into Trie
    for (size_t i = 0; i < sizeof(words) / sizeof(words[0]); i++) {
        insertWord(root, words[i]);
    }

    // Allocate memory for results
    const char **results = malloc(numTests * sizeof(char *));
    if (!results) {
        fprintf(stderr, "Memory allocation failed.\n");
        freeTrie(root);
        return 0;
    }

    // Search for words and store results
    for (int i = 0; i < numTests; i++) {
        results[i] = malloc(256 * sizeof(char));
        if (!results[i]) {
            fprintf(stderr, "Memory allocation failed.\n");
            for (int j = 0; j < i; j++) {
                free((void *)results[j]);
            }
            free(results);
            freeTrie(root);
            return 0;
        }
        snprintf((char *)results[i], 256, "Searching for '%s': %s", testWords[i], searchWord(root, testWords[i]) ? "FOUND" : "NOT FOUND");
    }

    // Compare results to valid output
    int passed = compareResultsToValidOutput(results, numTests, validOutput);

    // Free allocated memory
    for (int i = 0; i < numTests; i++) {
        free((char *)results[i]);
    }
    free(results);
    freeTrie(root);

    return passed;
}

int compareResultsToValidOutput(const char *results[], int numResults, const char *validOutputFile) {
    FILE *file = fopen(validOutputFile, "r");
    if (!file) {
        fprintf(stderr, "Failed to open valid output file: %s\n", validOutputFile);
        return 0;
    }

    char line[256];
    int lineIndex = 0;
    int mismatchFound = 1;

    while (fgets(line, sizeof(line), file) && lineIndex < numResults) {
        // Remove newline character from the line read from the file
        line[strcspn(line, "\n")] = 0;

        if (strcmp(line, results[lineIndex]) != 0) {
            mismatchFound = 0;
        }
        lineIndex++;
    }

    fclose(file);
    return mismatchFound;
}

int main() {
    int testPassed = testTrie();

    printf("%s[ %s ]%s Trie Test\n",
        (testPassed) ? GREEN : RED,
        (testPassed) ? "PASSED" : "FAILED",
        RESET);

    printf("All tests completed.\n");
    return 0;
}
