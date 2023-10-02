#ifndef STR_H
#define STR_H

#include "types.h"

// *** custom C standard library string.h *** //
int _strlen(const char *);

int _strcmp(const char *, const char *);
int _strncmp(const char *, const char *, int);

char *_strcat(char *, const char *);
char *_strncat(char *, const char *, int);

void _TESTstrcpy(char *, const char *);
char *_strcpy(char *, const char *);
char *_strncpy(char *, const char *, int);

char *_strchr(const char *, unsigned char);
char *_strstr(const char *, const char *);

//may be useful for the command line tokenizer function
//char *_strtok(char *str, const char *delim)

// *** non standard library methods *** //
char get_char(const char *, int);
bool contains(const char *, const char *);

#endif