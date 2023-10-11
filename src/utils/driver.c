#include <stdio.h>
#include "str.h"
#include "io.h"

void lenTest();
void cmpTest();
void catTest();
void cpyTest();
void chrTest();
void getTest();
void subTest();
void spnTest();
void memTest();

int main()
{

	int option = -1;

	do
	{
		printf("Enter string test driver option\n");
		scanf("%d", &option);
		(void)getchar(); // clean the input of newline

		switch (option)
		{
		case 0:
			lenTest();
			break;
		case 1:
			cmpTest();
			break;
		case 2:
			catTest();
			break;
		case 3:
			cpyTest();
			break;
		case 4:
			chrTest();
			break;
		case 5:
			getTest();
			break;
		case 6:
			subTest();
			break;
		case 7:
			spnTest();
			break;
		case 8:
			memTest();
			break;
		case -1:
			break;
		default:
			printf("Error! option %d does not exist. Try Again\n", option);
			break;
		}

		printf("\n=============================================\n\n");
	} while (option != -1);

	return 0;
}

void lenTest()
{
	// char *s = "hello";
	char s[20];
	printf("Enter a string\n");
	// gets(s);
	// fgets(s, 20, stdin); does length + 1
	fscanf(stdin, "%s", s);
	printf("length of %s = %d\n", s, _strlen(s));
}

void cmpTest()
{
	char s1[20];
	char s2[20];
	// char *s1 = "hello";
	// char *s2 = "hell";
	int cmp;

	printf("Enter string 1\n");
	gets(s1);
	printf("Enter string 2\n");
	gets(s2);

	cmp = _strcmp(s1, s2);

	printf("%d\n", cmp);
	if (cmp < 0)
		printf("%s is less than %s\n", s1, s2);
	else if (cmp > 0)
		printf("%s is greater than %s\n", s1, s2);
	else
		printf("%s is the same string as %s\n", s1, s2);
}

void catTest()
{
	char s1[5];
	char s2[15];
	char *s;
	int n;

	printf("Enter string 1\n");
	gets(s1);
	printf("Enter string 2\n");
	gets(s2);
	/*printf("Enter a value\n");
	  scanf("%d", &n);

	  s = _strncat(s1, s2, n);*/
	s = _strcat(s1, s2);
	printf("The new string is: %s\n", s);
	printf("%s has a length of %d", s, _strlen(s));
}

void cpyTest()
{
	char s1[10];
	char s2[15];
	int n;

	printf("Enter the destination string (can be empty)\n");
	gets(s1);
	printf("Enter a string to copy\n");
	gets(s2);
	// printf("Enter a value\n");
	// scanf("%d", &n);

	// s = _strncpy(s1, s2, n);
	
	_strcpy(s1, s2);
	printf("The new string is: %s\n", s1);
	printf("%s has a length of %d\n", s1, _strlen(s1));
}

void chrTest()
{
	char s1[20];
	char *s;
	char c;

	printf("Enter a string\n");
	gets(s1);
	printf("Enter a character to find\n");
	scanf("%c", &c);

	s = _strchr(s1, c);

	if (*s == '\0' && _strlen(s) != 1)
		printf("%c does not exist in the string", c);
	else
	{
		printf("The string after '%c' is: %s\n", c, s);
		printf("%c exists in the string", c);
	}
}

void getTest()
{
	char s1[20];
	char *s;
	char c;
	int i;

	printf("Enter a string\n");
	gets(s1);
	printf("Enter an index\n");
	scanf("%d", &i);

	c = get_char(s1, i);

	if (c == ' ')
		printf("index %d does not exist in the string\n", i);
	else
		printf("The character at index '%d' is: %c\n", i, c);
}

void subTest()
{
	char s1[20];
	char s2[20];
	// char *s;
	bool b;
	char c;
	int i;

	printf("Enter a string\n");
	gets(s1);
	printf("Enter a substring\n");
	gets(s2);

	// s = _strstr(s1, s2);
	b = contains(s1, s2);

	if (b == false)
		printf("'%s' does not exist in the string\n", s2);
	else
		printf("'%s' is in '%s'", s2, s1);
}

void spnTest()
{
	int len;
	char s1[20];
	char s2[20];

	printf("Enter a string\n");
	gets(s1);
	printf("Enter a substring\n");
	gets(s2);

	len = _strcspn(s1, s2);
   	printf("First matched character is at %d\n", len + 1);
}

void memTest()
{
	char *s;
	char s1[20];
	char c;
	int i;

	printf("Enter a string\n");
	fgets(s1, 20, stdin);
	printf("Enter a character\n");
	c = getchar();
	printf("Enter a length\n");
	scanf("%d", &i);

	_memset(s1, c, i);
	printf("The new string is: %s", s1);
}