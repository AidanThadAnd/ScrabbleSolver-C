/*---------- ID HEADER -------------------------------------
/   Author(s):    Aidan Andrews, Kyle Scidmore
/   File Name:    io.h
/
/   File Description:
/     This header file declares functions related to input/output operations,
/     specifically for loading a dictionary from a file into a Trie data structure.
/
/---------------------------------------------------------*/
#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dataStruct.h"


TrieNode *loadDictionary(const char *filename);

#endif