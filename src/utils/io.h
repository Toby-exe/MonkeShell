#ifndef IO_H
#define IO_H

#include <fcntl.h>
#include <unistd.h>
#include "types.h"

//NOTE: FILE *stream == int fd

// *** custom C standard library stdio.h *** //
// *** INPUT *** //
int _fscanf(int, const char *format, ...);
char *_fgets(char *, int, int);
ssize_t _getline(char **, size_t *, int);

// *** OUTPUT *** //
int _fprintf(File *stream, const char *format, ...);
int _fputs(const char *s, int);

// *** ERROR / DEBUG *** //
//uses stderr instead of stdin or stdout
//void error(int status, int errnum, const char *format, ...);
//void perror(const char *s); (basically just printf)
//int ferror(File *stream);
//char *strerror(int errnum);

// *** non stdio.h methods *** //


#endif
