/**
 * @file str.c
 * @authors Tobias Wondwossen, Jayden Mingle
 *
 * Notes: all methods have been written with the assumption that the caller invokes them as
 * intended such that no memory corruption will take place
 *
 * @date 2023-10-20
 */
#include "str.h"

/**
 * @brief Returns the length of a string
 * @param[in] str The string to measure
 * @return The length of the string
 */
int c_strlen(const char *str)
{
	const char *start = str;

	while (*str++)
		;

	return str - start - 1;
}

/**
 * @brief Compares two strings
 * @param[in] str1 The first string to compare
 * @param[in] str2 The second string to compare
 * @return 0 if the strings are equal, a positive or negative value otherwise
 */
int c_strcmp(const char *str1, const char *str2)
{
	while (*str1 == *str2)
	{
		if (*str1 == '\0' || *str2 == '\0')
			return 0; // both strings are equal

		str1++;
		str2++;
	}

	return *str1 - *str2;
}

/**
 * @brief Compares two strings up to n characters
 * @param[in] str1 The first string to compare
 * @param[in] str2 The second string to compare
 * @param[in] n The maximum number of characters to compare
 * @return 0 if the strings are equal for the first n characters, a positive or negative value otherwise
 */
int c_strncmp(const char *str1, const char *str2, size_t n)
{
	for (size_t i = 0; i < n; i++)
	{
		if (*str1 != *str2)
			return *str1 - *str2;

		if (*str1 == '\0' || *str2 == '\0')
			return 0; // Reached the end of both strings before n characters

		str1++;
		str2++;
	}

	return 0; // Both strings are equal for the first n characters
}

/**
 * @brief Appends a string to another string
 * @param[out] dst The destination string
 * @param[in] src The source string to append
 * @return A pointer to the destination string
 */
char *c_strcat(char *dst, const char *src)
{
	char *curr = dst + c_strlen(dst);

	while (*src != '\0')
		*curr++ = *src++;

	*curr = '\0';

	return dst;
}

/**
 * @brief Appends a string to another string up to n characters
 * @param[out] dst The destination string
 * @param[in] src The source string to append
 * @param[in] n The maximum number of characters to append
 * @return A pointer to the destination string
 */
char *c_strncat(char *dst, const char *src, size_t n)
{
	char *curr = dst + c_strlen(dst);

	if (n > c_strlen(src))
		n = c_strlen(src);

	for (size_t i = 0; *src != '\0' && i < n; i++)
		*curr++ = *src++;

	*curr = '\0';

	return dst;
}

/**
 * @brief Copies a string to another string
 * @param[out] dst The destination string
 * @param[in] src The source string to copy from
 * @return A pointer to the destination string
 */
char *c_strcpy(char *dst, const char *src)
{
	char *curr = dst;

	while (*src != '\0')
		*curr++ = *src++;

	*curr = '\0';

	return dst;
}

/**
 * @brief Copies a string to another string up to n characters
 * @param[out] dst The destination string
 * @param[in] src The source string to copy from
 * @param[in] n The maximum number of characters to copy
 * @return A pointer to the destination string
 */
char *c_strncpy(char *dst, const char *src, size_t n)
{
	char *curr = dst;

	if (n > c_strlen(src))
		n = c_strlen(src);

	for (size_t i = 0; *src != '\0' && i < n; i++)
		*curr++ = *src++;

	*curr = '\0';

	return dst;
}

/**
 * @brief Finds the first occurrence of a character in a string
 * @param[in] str The string to search
 * @param[in] c The character to find
 * @return A pointer to the first occurrence of the character in the string, or NULL if the
 * character is not found
 */
char *c_strchr(const char *str, int c)
{
	while (*str && *str != c)
		str++;

	return (char *)str;
}

/**
 * @brief Finds the last occurrence of a character in a string
 * @param[in] str The string to search
 * @param[in] c The character to find
 * @return A pointer to the last occurrence of the character in the string, or NULL if the character is not found
 */
char *c_strrchr(const char *str, int c)
{
	const char *last = str + c_strlen(str) - 1;

	while (last != str && *last != c)
		last--;

	return (char *)last;
}

/**
 * @brief Finds the first occurrence of a substring in a string
 * @param[in] str The string to search
 * @param[in] substr The substring to find
 * @return A pointer to the first occurrence of the substring in the string, or NULL if the substring is not found
 */
char *c_strstr(const char *str, const char *substr)
{
	const char *a, *b;

	b = substr;

	// empty substring
	if (*b == 0)
		return (char *)str;

	// look for first character of substr in str
	for (; *str != '\0'; str++)
	{
		if (*str != *b)
			continue;

		a = str;

		// first character has been found. now compare the rest of the substring
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

/**
 * @brief Returns the length of the initial segment of str which consists entirely of characters not in accept
 * @param[in] str The string to search
 * @param[in] accept The string containing characters to match
 * @return The length of the initial segment of str which consists entirely of characters not in accept
 */
size_t c_strspn(const char *str, const char *accept)
{
	const char *start = str;
	const char *c;

	for (; *str != '\0'; str++)
	{
		// compare the current str character with each accept character
		for (c = accept; *c != '\0'; c++)
		{
			if (*str != *c)
				goto done;
		}
	}

done:
	return str - start;
}

/**
 * @brief Returns the length of the initial segment of str which consists entirely of characters in reject
 * @param[in] str The string to search
 * @param[in] reject The string containing characters to match
 * @return The length of the initial segment of str which consists entirely of characters in reject
 */
size_t c_strcspn(const char *str, const char *reject)
{
	const char *start = str;
	const char *c;

	for (; *str != '\0'; str++)
	{
		// compare the current str character with each reject character
		for (c = reject; *c != '\0'; c++)
		{
			if (*str == *c)
				goto done;
		}
	}

done:
	return str - start;
}

/**
 * @brief Fills a block of memory with a specified value
 * @param[out] s Pointer to the block of memory to fill
 * @param[in] c Value to be set. The value is passed as an int, but the function fills the block of memory using the unsigned char conversion of this value
 * @param[in] n Number of bytes to be set to the value
 * @return s is returned
 */
void *c_memset(void *s, int c, size_t n)
{
	unsigned char *byte_ptr = s;

	for (size_t i = 0; i < n; i++)
		byte_ptr[i] = (unsigned char)c;

	return s;
}

/**
 * @brief Returns a character from a string at a specified index
 * @param[in] str The string to get the character from
 * @param[in] i The index of the character to get
 * @return The character at index i in str, or 0 if i is out of bounds
 */
char get_char(const char *str, int i)
{
	if (i < 0 || i >= c_strlen(str))
		return (0);

	return str[i];
}

/**
 * @brief Checks if a string contains a substring
 * @param[in] str The string to search
 * @param[in] substr The substring to find
 * @return true if str contains substr, false otherwise
 */
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

/**
 * @brief Converts an integer to a string
 * @param[in] num The integer to convert
 * @return A pointer to the resulting string. The caller is responsible for freeing this string.
 */
char *intToString(int num)
{
	char *str = alloc(10 * sizeof(char));
	int i = 0, j;
	char temp;
	while (num != 0)
	{
		str[i++] = (num % 10) + '0';
		num /= 10;
	}
	// Reverse string
	for (j = 0; j < i / 2; j++)
	{
		temp = str[j];
		str[j] = str[i - j - 1];
		str[i - j - 1] = temp;
	}
	return str;
}