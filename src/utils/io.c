#include "io.h"

// ==================== INPUT ==================== //
int _fscanf(int fd, const char *format, ...) { return -1; }

// adds newline character from input
char *_fgets(char *buffer, int size, int fd)
{
    int bytesRead = 0;
    char ch;

    if (size <= 0 || buffer == NULL)
        return NULL;

    while (bytesRead < size - 1)
    {
        ssize_t n = read(fd, &ch, 1);

        if (n < 0)
        {
            //perror("read"); // replace with _perror("read");
            return NULL;
        }

        if (n == 0)
        {
            // End of file reached
            if (bytesRead == 0)
                return NULL; // Nothing read

            break;
        }

        buffer[bytesRead++] = ch;

        // Stop reading at newline (need a char *delim for custom delimiter)
        if (ch == '\n')
            break;
    }

    buffer[bytesRead] = '\0'; // Null-terminate the string

    return buffer;
}

// adds newline character from input
ssize_t _getline(char **lineptr, size_t *n, int fd)
{
    return 0;
}

// ==================== OUTPUT ==================== //
int _fprintf(File *stream, const char *format, ...) { return -1; }

int _fputs(const char *s, int fd) 
{ 
    if(s == NULL)
        return -1;

    int msg_len = 0;
    while(s[msg_len] != '\0')
        msg_len++;

    ssize_t bytes_written = write(fd, s, msg_len);

    //write error
    if(bytes_written == -1)
        return -1;

    return bytes_written;
}

// ==================== ERROR/DEBUG ==================== //