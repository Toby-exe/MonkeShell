#ifndef IO_H
#define IO_H

#include <fcntl.h>
#include <unistd.h>
#include "types.h"

#define CLEAR_SCREEN "\033[2J"
#define MOVE_PAGE_UP "\033[H"
#define RESET_COLOR "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define CYAN "\033[36m"
#define PURPLE "\033[35m"
#define BRIGHT_RED "\033[31;1m"
#define BRIGHT_GREEN "\033[32;1m"
#define BRIGHT_YELLOW "\033[33;1m"
#define BG_BRIGHT_YELLOW "\u001b[43;1m"
#define CUSTOM_COLOR_1 "\u001b[38;5;${226}m"


//NOTE: FILE *stream == int fd

// *** custom C standard library stdio.h *** //
// *** INPUT *** //
int c_fscanf(int, const char *format, ...);
char *c_fgets(char *, int, int);
ssize_t c_getline(char **, size_t *, int);

// *** OUTPUT *** //
int c_fprintf(File *stream, const char *format, ...);
int c_fputs(const char *s, int);
int c_write(const char *s, int fd, const char *color);


// *** ERROR / DEBUG *** //
//uses stderr instead of stdin or stdout
//void error(int status, int errnum, const char *format, ...);
//void perror(const char *s); (basically just printf)
//int ferror(File *stream);
//char *strerror(int errnum);

// *** non stdio.h methods *** //


#endif
