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
#include "../dataStruct.h"
#include "../io.h"

#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define RESET "\033[0m"

#define validOutput "./Testers/dictionaryTests/validOutput.txt"

int compareResultsToValidOutput(const char *results[], int numResults, const char *validOutputFile);

int testDictionary(const char *filename) {
    TrieNode *root = loadDictionary(filename);

    if (!root) {
        fprintf(stderr, "Failed to load dictionary.\n");
        return 0;
    }

    const char *testWords[] = {"APPLE", "MANGO", "PEACH", "GRAPES", "BANANA", "GGGGGGG"};
    int numTests = sizeof(testWords) / sizeof(testWords[0]);

    const char **results = malloc(numTests * sizeof(char *));
    if (!results) {
        fprintf(stderr, "Memory allocation failed.\n");
        freeTrie(root);
        return 0;
    }
    
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

    int passed = compareResultsToValidOutput(results, numTests, validOutput);

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

    return mismatchFound;

    fclose(file);
}

int main() {

    char* validDictionary = "Testers/dictionaryTests/validDictionary.txt";
    char* invalidDictionary = "Testers/dictionaryTests/invalidDictionary.txt";

    int testValid = testDictionary(validDictionary);
    int testInvalid = testDictionary(invalidDictionary);

    printf("%s[ %s ]%s Valid Dictionary Test\n",
        (testValid) ? GREEN : RED,
        (testValid) ? "PASSED" : "FAILED",
        RESET);

    printf("%s[ %s ]%s Invalid Dictionary Test <- Supposed to fail\n",
        (testInvalid) ? GREEN : RED,
        (testInvalid) ? "PASSED" : "FAILED",
        RESET);
    printf("All tests completed.\n");
    
    return 0;
}
