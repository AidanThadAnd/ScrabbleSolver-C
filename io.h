#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dataStruct.h"


TrieNode *loadDictionary(const char *filename);

#endif