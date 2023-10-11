#include "str.h"

/*----------------------------------------------------------------------------------
NOTE:
all methods have been written with the assumption that the caller invokes them as
intended such that no memory corruption will take place
----------------------------------------------------------------------------------*/

int _strlen(const char *str)
{
	const char *start = str;

	while (*str++)
		;

	return str - start - 1;
}

int _strcmp(const char *str1, const char *str2)
{
	while (*str1 == *str2)
	{
		if (*str1++ == '\0' || *str2++ == '\0')
			break;
	}

	if (*str1 == '\0' && *str2 == '\0')
		return 0;
	else
		return *str1 - *str2;
}

int _strncmp(const char *str1, const char *str2, int n)
{
	for (int i = 0; *str1 == *str2 && i < n; i++)
	{
		if (*str1++ == '\0' || *str2++ == '\0')
			break;
	}

	if (*str1 == '\0' && *str2 == '\0')
		return 0;
	else
		return *str1 - *str2;
}

char *_strcat(char *dst, const char *src)
{
	char *curr = dst + _strlen(dst);

	while (*src != '\0')
		*curr++ = *src++;

	*curr = '\0';

	return dst;
}

char *_strncat(char *dst, const char *src, int n)
{
	char *curr = dst + _strlen(dst);

	if (n > _strlen(src))
		n = _strlen(src);

	for (int i = 0; *src != '\0' && i < n; i++)
		*curr++ = *src++;

	*curr = '\0';

	return dst;
}

char *_strcpy(char *dst, const char *src)
{
	char *curr = dst;

	while (*src != '\0')
		*curr++ = *src++;

	*curr = '\0';

	return dst;
}

char *_strncpy(char *dst, const char *src, int n)
{
	char *curr = dst;

	if (n > _strlen(src))
		n = _strlen(src);

	for (int i = 0; *src != '\0' && i < n; i++)
		*curr++ = *src++;

	*curr = '\0';

	return dst;
}

//int == unsigned char
char *_strchr(const char *str, int c)
{
	while(*str && *str != c)
		str++;
	
	return (char *)str;
}

char *_strrchr(const char *str, int c)
{
	const char *last = str + _strlen(str) - 1;

	while (last != str && *last != c)
		last--;

	return (char *)last;
}

char *_strstr(const char *str, const char *substr)
{
	const char *a, *b;

	b = substr;

	// empty substring
	if (*b == 0)
		return (char *)str;

	//look for first character of substr in str
	for (; *str != '\0'; str++)
	{
		if (*str != *b)
			continue;

		a = str;

		//first character has been found. now compare the rest of the substring
		while (1)
		{
			if (*b == 0)
				return (char *)str;

			if (*a++ != *b++)
				break;
		}

		b = substr;
	}

	return (0);
}

/*returns the offset of the first occurence of a character in str
that does not exist in accept*/
size_t _strspn (const char *str, const char *accept)
{
	const char *start = str;
	const char *c;

	for (; *str != '\0'; str++)
	{
		//compare the current str character with each accept character
		for (c = accept; *c != '\0'; c++)
		{
			if (*str != *c)
				goto done;
		}
	}

	done:	return str - start;
}

/*returns the offset of the first occurence of a character in str
that exists in reject*/
size_t _strcspn (const char *str, const char *reject)
{
	const char *start = str;
	const char *c;

	for (; *str != '\0'; str++)
	{
		//compare the current str character with each reject character
		for (c = reject; *c != '\0'; c++)
		{
			if (*str == *c)
				goto done;
		}
	}

	done:	return str - start;
}

void *_memset(void *s, int c, size_t n)
{
	unsigned char *byte_ptr = s;

    for (size_t i = 0; i < n; i++)
        byte_ptr[i] = (unsigned char)c;
		
    return s;
}

char get_char(const char *str, int i)
{
	if (i < 0 || i >= _strlen(str))
		return (0);

	return str[i];
}

//modified version of strstr() that only returns a boolean
bool contains(const char *str, const char *substr)
{
	const char *a, *b;

	b = substr;

	if (*b == '\0')
		return false;

	
	for (; *str != '\0'; str++)
	{
		if (*str != *b)
			continue;

		a = str;

		while (1)
		{
			if (*b == '\0')
				return true;

			if (*a++ != *b++)
				break;
		}

		b = substr;
	}

	return false;
}
