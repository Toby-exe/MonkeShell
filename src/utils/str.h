#ifndef STR_H
#define STR_H

#include <stdlib.h>
#include "types.h"

// *** custom C standard library string.h *** //
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

// *** non string.h methods *** //
char get_char(const char *, int);
bool contains(const char *, const char *);

// *** may be useful for command line tokenizer *** //
/*char *c_strtok(char *str, const char *delim)
  char * strsep(char **stringp, const char *delim);*/

#endif
