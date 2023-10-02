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
	int i;
	for (i = 0; *str1 == *str2 && i < n; i++)
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
	if (n > _strlen(src))
		n = _strlen(src);

	char *curr = dst + _strlen(dst);

	int i = 0;
	while (*src != '\0' && i < n)
	{
		*curr++ = *src++;
		i++;
	}

	*curr = '\0';

	return dst;
}

//do strycpy and strcat even need to return anything?
void _TESTstrcpy(char *dst, const char *src)
{
	char *curr = dst;

	while (*src != '\0')
		*curr++ = *src++;

	*curr = '\0';
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

	int i = 0;
	while (*src != '\0' && i < n)
	{
		*curr++ = *src++;
		i++;
	}

	*curr = '\0';

	return dst;
}

char *_strchr(const char *str, unsigned char c)
{
	while (*str != '\0' && *str != c)
		str++;

	return (char *)str;
}

//char *_strstr(const char *str, const char *substr)
//{
	/*const char *a, *b;

	b = substr;

	// empty substring
	if (*b == 0)
		return (char *)str;

	for (; *str != '\0'; str++)
	{
		if (*str != *b)
			continue;

		a = str;

		while (1)
		{
			if (*b == 0)
				return (char *)str;

			if (*a++ != *b++)
				break;
		}

		b = substr;
	}

	return 0;*/

	/*const char *p = str;
	const int len = _strlen(substr);

	if (!len)
		return str;

	for (; (p = _strchr(p, *substr)) != 0; p++)
	{
		if (strncmp(p, substr, len) == 0)
			return (char *)p;
	}
	return (0);*/
//}

/*char *_strtok(char *str, const char *delim)
{

}*/

char get_char(const char *str, int i)
{
	if (i < 0 || i >= _strlen(str))
		return NULL;

	return str[i];
}

//abba, ba
bool contains(const char *str, const char *substr)
{
	const char *a, *b;

	b = substr;

	// empty substring
	if (*b == '\0')
		return false;

	//look for first character of substr in str
	for (; *str != '\0'; str++)
	{
		if (*str != *b)
			continue;

		a = str;

		//first character has been found. now compare the rest of the substring
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
