#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "vec.h"
// Returns null terminated substring
// Takes char*, start, and end indices
char* getSubstring(char*, long unsigned int, long unsigned int);

// Separate strings by special characters and store them in a vector structure
void getArgs(char*, long unsigned int, vec*);
