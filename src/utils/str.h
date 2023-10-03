#ifndef STR_H
#define STR_H

#include <unistd.h>
#include "types.h"

// *** custom C standard library string.h *** //
int _strlen(const char *);

int _strcmp(const char *, const char *);
int _strncmp(const char *, const char *, int);

char *_strcat(char *, const char *);
char *_strncat(char *, const char *, int);

char *_strcpy(char *, const char *);
char *_strncpy(char *, const char *, int);

char *_strchr(const char *, int);
char *_strrchr(const char *, int);

char *_strstr(const char *, const char *);

size_t _strspn (const char *, const char *);
size_t _strcspn (const char *, const char *);

void *_memset(void *, int, size_t);

// *** non string.h methods *** //
char get_char(const char *, int);
bool contains(const char *, const char *);

// *** may be useful for command line tokenizer *** //
/*char *_strtok(char *str, const char *delim)
  char * strsep(char **stringp, const char *delim);*/

#endif