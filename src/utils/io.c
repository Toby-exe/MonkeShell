/**
 * @file io.c
 * @authors Tobias Wondwossen, Jayden Mingle
 * @date 2023-10-20 
 */
#include "io.h"

/**
 * @brief A placeholder function for formatted input from a file descriptor
 * @param[in] fd The file descriptor to read from
 * @param[in] format The format string that specifies how to interpret the input
 * @return -1 as this function is not implemented
 */
int c_fscanf(int fd, const char *format, ...) { return -1; }

/**
 * @brief Reads a line from a file descriptor into a buffer
 * @param[out] buffer The buffer to read the line into
 * @param[in] size The maximum number of characters to read
 * @param[in] fd The file descriptor to read from
 * @return A pointer to the buffer, or NULL if an error occurred or end of file was reached before any characters were read
 */
char *c_fgets(char *buffer, int size, int fd)
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
            // perror("read"); // replace with _perror("read");
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

/**
 * @brief A placeholder function for getting a line from a file descriptor
 * @param[out] lineptr A pointer to the variable that points to the buffer storing the line
 * @param[out] n A pointer to the variable that saves the size of the buffer
 * @param[in] fd The file descriptor to read from
 * @return 0 as this function is not implemented
 */
ssize_t c_getline(char **lineptr, size_t *n, int fd)
{
    return 0;
}

/**
 * @brief A placeholder function for formatted output to a file descriptor
 * @param[out] stream The file descriptor to write to
 * @param[in] format The format string that specifies how to format the output
 * @return -1 as this function is not implemented
 */
int v_fprintf(File *stream, const char *format, ...) { return -1; }


/**
 * @brief Writes a string to a file descriptor
 * @param[in] s The string to write
 * @param[in] fd The file descriptor to write to
 * @return The number of characters written, or -1 if an error occurred
 */
int c_fputs(const char *s, int fd)
{
    if (s == NULL)
        return -1;

    int msg_len = 0;
    while (s[msg_len] != '\0')
        msg_len++;

    ssize_t bytes_written = write(fd, s, msg_len);

    // write error
    if (bytes_written == -1)
        return -1;

    return bytes_written;
}

/**
 * @brief Writes a string to a file descriptor with optional color
 * @param[in] s The string to write
 * @param[in] fd The file descriptor to write to
 * @param[in] color The color to use for the output. If NULL, no color is used.
 * @return -1 if s is NULL, otherwise it returns the result of c_fputs function.
 */
int c_write(const char *s, int fd, const char *color) {
    if(s == NULL) return -1;

    if (color != NULL) {
        c_fputs(color, fd);
    }
    c_fputs(s, fd);
    if(color != NULL) {
        c_fputs(RESET_COLOR, fd);
    }
}