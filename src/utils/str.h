#ifndef STR_H
#define STR_H

#include "types.h"

// *** custom C standard library string.h *** //
int _strlen(const char *);

int _strcmp(const char *, const char *);
int _strncmp(const char *, const char *, int);

char *_strcat(char *, const char *);
char *_strncat(char *, const char *, int);

char *_strcpy(char *, const char *);
char *_strncpy(char *, const char *, int);

char *_strchr(const char *, unsigned char);
char *_strstr(const char *, const char *);

//may be useful for the command line tokenizer function
//char *_strtok(char *str, const char *delim)

// *** unused lib methods*** //
/*
int strcoll(const char *str1, const char *str2);
size_t strcspn(const char *str1, const char *str2);
char *strerror(int errnum);
char *strpbrk(const char *str1, const char *str2);
char *strrchr(const char *str, int c);
size_t strspn(const char *str1, const char *str2);
size_t strxfrm(char *dest, const char *src, size_t n);
void *memchr(const void *str, int c, size_t n);
int memcmp(const void *str1, const void *str2, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
void *memmove(void *dest, const void *src, size_t n);
void *memset(void *str, int c, size_t n);
*/

// *** non standard library methods *** //
char get_char(const char *, int);
bool contains(const char *, const char *);

#endif
