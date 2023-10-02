#ifndef TYPES_H
#define TYPES_H

// *** custom types *** //
typedef enum { false, true } bool;
typedef unsigned char UINT8;
typedef unsigned int  UINT16;
typedef unsigned long UINT32;
typedef unsigned int File;

//typedef size_t
//typedef ssize_t
//typedef FILE

//malloc
//free
/* random driver for explanation
#include <stdio.h>
#include <stdlib.h>

int main()
{
    char *buffer;
    size_t bufsize = 32;
    size_t characters;

    buffer = (char *)malloc(bufsize * sizeof(char));
    if( buffer == NULL)
    {
        perror("Unable to allocate buffer");
        exit(1);
    }

    printf("Type something: ");
    characters = getline(&buffer,&bufsize,stdin);
    printf("%zu characters were read.\n",characters);
    printf("You typed: '%s'\n",buffer);

    return(0);
}
*/

// *** constants *** //
#define NULL (0) //not needed because of unistd
#define FILE_STDIN 0
#define FILE_STDOUT 1
#define FILE_STDERR 2

#endif