#ifndef IO_H
#define IO_H

#include <fcntl.h>
#include <unistd.h>
#include "types.h"

//NOTE: all File types used to be FILE
// *** custom C standard library stdio.h *** //
// *** INPUT *** //
int _scanf(const char * format, ...);
int _fscanf(File *stream, const char *format, ...);

char *_gets(char *s);
char *_fgets(char *s, int size, File *stream);

ssize_t _getline(char **lineptr, size_t *n, File *stream); //adds newline character from input

// *** OUTPUT *** //
int _printf(const char *, ...);
int _fprintf(File *stream, const char *format, ...);

int _puts(const char *s);
int _fputs(const char *s, File *stream);

// *** ERROR / DEBUG *** //
//uses stderr instead of stdin or stdout
//void error(int status, int errnum, const char *format, ...);
//void perror(const char *s); (basically just printf)
//int ferror(File *stream);
//char *strerror(int errnum);

#endif
