#include <stdio.h>
#include "io.h"

//eventually replace this with a series of CMake google tests
int main()
{
    char s[20];
	printf("Enter a string\n");
    _fgets(s, 20, 0);
    printf("The string you read in was: %s\n", s);

    return 0;
}