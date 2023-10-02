#ifndef IO_H
#define IO_H

#include "types.h"

// *** custom C standard library stdio.h *** //
// *** INPUT *** //
int _scanf(const char * format, ...);
//int _fscanf(FILE *stream, const char *format, ...);

char *_gets(char *s);
//char *_fgets(char *s, int size, FILE *stream);

//ssize_t _getline(char **lineptr, size_t *n, FILE *stream); //adds newline character from input

// *** OUTPUT *** //
int _printf(const char *, ...);
//int _fprintf(FILE *stream, const char *format, ...);

int _puts(const char *s);
//int _fputs(const char *s, FILE *stream);

// *** ERROR / DEBUG *** //
//uses stderr instead of stdin or stdout
//void error(int status, int errnum, const char *format, ...);
//void perror(const char *s); (basically just printf)
//int ferror(FILE *stream);
//char *strerror(int errnum);

#endif
