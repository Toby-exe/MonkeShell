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
char *_fgets(char *buffer, int size, int fd)
{
    int bytesRead = 0;
    char ch;

    if (size <= 0 || buffer == NULL) {
        return NULL;
    }

    while (bytesRead < size - 1) {
        ssize_t n = read(fd, &ch, 1);

        if (n < 0) {
            perror("read");
            return NULL;
        }

        if (n == 0) {
            // End of file reached
            if (bytesRead == 0) {
                return NULL; // Nothing read
            }
            break;
        }

        buffer[bytesRead++] = ch;

        if (ch == '\n') {
            break; // Stop reading at newline
        }
    }

    buffer[bytesRead] = '\0'; // Null-terminate the string

    return buffer;
}  

ssize_t _getline(char **lineptr, size_t *n, File *stream){ return NULL; } //adds newline character from input

// ==================== OUTPUT ==================== //
int _printf(const char *, ...){ return NULL; }
int _fprintf(File *stream, const char *format, ...){ return NULL; }

int _puts(const char *s){ return NULL; }
int _fputs(const char *s, File *stream){ return NULL; }

// ==================== ERROR/DEBUG ==================== //