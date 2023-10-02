#include "io.h"

// ==================== INPUT ==================== //
int _scanf(const char * format, ...){ return NULL; }
int _fscanf(File *stream, const char *format, ...){ return NULL; }

char *_gets(char *s){ return NULL; }

//adds newline character from input
/*
s = input buffer
size = number of characters
stream = input stream (ie stdin)
*/
char *_fgets(char *s, int size, File *stream)
{
    size_t bytes_read;

    //no characters read
    if(size <= 0)
        return NULL;

    while (size > 0)
    {
        while (*s != '\0' && *s != '\n')
		    s++;
        
        read(&stream, s, size);
    }

    *s = '\0';
    return s;
}  

ssize_t _getline(char **lineptr, size_t *n, File *stream){ return NULL; } //adds newline character from input

// ==================== OUTPUT ==================== //
int _printf(const char *, ...){ return NULL; }
int _fprintf(File *stream, const char *format, ...){ return NULL; }

int _puts(const char *s){ return NULL; }
int _fputs(const char *s, File *stream){ return NULL; }

// ==================== ERROR/DEBUG ==================== //