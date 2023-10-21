/**
 * @file str.h
 * @brief A header file defining custom string functions and types
 * @authors Tobias Wondwossen, Jayden Mingle
 * 
 * Details: This header file defines several custom string functions that mimic the functionality of the 
 * standard C library string.h. It includes functions for string length, comparison, concatenation, copy, 
 * character search, substring search, and span. It also defines a few non-standard string functions such 
 * as get_char, contains, and intToString.
 * 
 * Assumptions/Limitations: The functions in this file are assumed to be compatible with the rest of the program. 
 * The intToString function allocates memory for the resulting string, which must be freed by the caller. The 
 * get_char function returns 0 if the index is out of bounds. The contains function only returns a boolean 
 * indicating whether a substring is found in a string.
 *
 *@date 2023-10-20
 */
#ifndef STR_H
#define STR_H

#include <stdlib.h>
#include "types.h"
#include "mem.h"

int c_strlen(const char *);

int c_strcmp(const char *, const char *);
int c_strncmp(const char *, const char *, size_t);

char *c_strcat(char *, const char *);
char *c_strncat(char *, const char *, size_t);

char *c_strcpy(char *, const char *);
char *c_strncpy(char *, const char *, size_t);

char *c_strchr(const char *, int);
char *c_strrchr(const char *, int);

char *c_strstr(const char *, const char *);

size_t c_strspn(const char *, const char *);
size_t c_strcspn(const char *, const char *);

void *c_memset(void *, int, size_t);

char get_char(const char *, int);
bool contains(const char *, const char *);
char *intToString(int );


// *** may be useful for command line tokenizer *** //
/*char *c_strtok(char *str, const char *delim)
  char * strsep(char **stringp, const char *delim);*/

#endif
