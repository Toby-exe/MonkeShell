/**
 * @file io.h
 * @brief A header file defining custom IO functions and color constants
 * @authors Tobias Wondwossen, Jayden Mingle
 * 
 * Details: This header file defines several custom IO functions that mimic the functionality of the standard C 
 * library. It includes functions for reading from and writing to files, and getting lines from a file. It also 
 * defines several color constants for use in terminal output.
 * 
 * Assumptions/Limitations: The functions in this file are assumed to be compatible with the rest of the program. 
 * The c_fscanf function uses a variable number of arguments, which must match the format string. The c_fgets and 
 * c_getline functions return a pointer to the read string, which may need to be freed by the caller. The c_fprintf, 
 * c_fputs, and c_write functions write to a file descriptor, which must be open for writing.
 *
 * @date 2023-10-20
 */

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


// NOTE: FILE *stream == int fd

int c_fscanf(int, const char *format, ...);
char *c_fgets(char *, int, int);
ssize_t c_getline(char **, size_t *, int);


int c_fprintf(File *stream, const char *format, ...);
int c_fputs(const char *s, int);
int c_write(const char *s, int fd, const char *color);

#endif
