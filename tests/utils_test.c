/**
 * @file test.c
 * @authors Tobias Wondwossen, Jayden Mingle
 * 
 * @date 2023-10-20 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/utils/str.h"
#include "../src/utils/io.h"
#include "../src/utils/mem.h"

#define T_WHITE "\033[0;37m"
#define T_RED "\033[0;31m"
#define T_GREEN "\033[0;32m"
#define T_CYAN "\033[0;36m"
#define T_RESET "\033[0m"
#define T_BOLD "\033[1m%s\033[0m\n"

void printTestResult(int expected, int actual, char *testName)
{
  if (expected == actual)
  {
    printf("\033[0;32m[PASS] \033[0;36m%s\n", testName);
  }
  else
  {
    printf("\033[0;31m[FAIL] \033[0;36m%s\n", testName);
    printf("\033[0;37mexpected: %d\nactual: %d\n", expected, actual);
  }

  // reset color
  printf(T_RESET);
}

void testStrLen()
{
  // print the name of the test suite in bold white
  printf(T_BOLD, "Testing string length");
  // test 1
  char *str1 = "Hello World";
  int expected1 = 11;
  int actual1 = c_strlen(str1);
  printTestResult(expected1, actual1, "testStrLen: length of string with 11 characters and a space");

  // test 2
  char *str2 = "";
  int expected2 = 0;
  int actual2 = c_strlen(str2);
  printTestResult(expected2, actual2, "testStrLen: length of empty string");

  // test 3
  char *str3 = "Hello World 0023 2890349854305 20983432";
  int expected3 = 39;
  int actual3 = c_strlen(str3);
  printTestResult(expected3, actual3, "testStrLen: length of string with 41 characters and spaces");
}

void testStrCmp()
{
  // print the name of the test suite in bold white
  printf(T_BOLD, "Testing string comparison");

  // test 1
  char *str1 = "Hello World";
  char *str2 = "Hello World";
  int expected1 = 0;
  int actual1 = c_strcmp(str1, str2);
  printTestResult(expected1, actual1, "testStrCmp: compare two identical strings");

  // test 2
  char *str3 = "Hello World";
  char *str4 = "Hello World 0023 2890349854305 20983432";
  int expected2 = -32;
  int actual2 = c_strcmp(str3, str4);
  printTestResult(expected2, actual2, "testStrCmp: compare two strings with different lengths");

  // test 3
  char *str5 = " ";
  char *str6 = "Hello World";
  int expected3 = -40;
  int actual3 = c_strcmp(str5, str6);
  printTestResult(expected3, actual3, "testStrCmp: compare two strings with different lengths");

  // test 4
  char *str7 = " ";
  char *str8 = " ";
  int expected4 = 0;
  int actual4 = c_strcmp(str7, str8);
  printTestResult(expected4, actual4, "testStrCmp: compare two empty strings");
}

void testStrncmp()
{
  printf(T_BOLD, "Testing string comparison with n characters");

  // test 1
  char *str1 = "Hello World";
  char *str2 = "Hello World";
  size_t n1 = 5;
  int expected1 = 0;
  int actual1 = c_strncmp(str1, str2, n1);
  printTestResult(expected1, actual1, "testStrncmp: compare first n characters of two identical strings");

  // test 2
  char *str3 = "Hello World";
  char *str4 = "Hello Universe";
  size_t n2 = 5;
  int expected2 = 0;
  int actual2 = c_strncmp(str3, str4, n2);
  printTestResult(expected2, actual2, "testStrncmp: compare first n characters of two different strings");

  // test 3
  char *str5 = "Hello";
  char *str6 = "World";
  size_t n3 = 5;
  int expected3 = -15;
  int actual3 = c_strncmp(str5, str6, n3);
  printTestResult(expected3, actual3, "testStrncmp: compare first n characters of two completely different strings");
}

void testStrcat()
{
  printf(T_BOLD, "Testing string concatenation");

  // test
  char str1[50] = "Hello";
  char *str2 = " World";
  char *expected = "Hello World";
  char *actual = c_strcat(str1, str2);
  printTestResult(strcmp(expected, actual), 0, "testStrcat: concatenate two strings");

  // test 2
  char str3[50] = " ";
  char *str4 = "Hello World";
  char *expected2 = " Hello World";
  char *actual2 = c_strcat(str3, str4);
  printTestResult(strcmp(expected2, actual2), 0, "testStrcat: concatenate two strings");
}

void testStrncat()
{
  printf(T_BOLD, "Testing string concatenation with n characters");

  // test
  char str1[50] = "Hello";
  char *str2 = " World";
  size_t n = 3;
  char *expected = "Hello Wo";
  char *actual = c_strncat(str1, str2, n);
  printTestResult(strcmp(expected, actual), 0, "testStrncat: concatenate first n characters of two strings");

  // test 2
  char str3[50] = " ";
  char *str4 = "Hello World";
  size_t n2 = 5;
  char *expected2 = " Hello";
  char *actual2 = c_strncat(str3, str4, n2);
  printTestResult(strcmp(expected2, actual2), 0, "testStrncat: concatenate first n characters of two strings");
}

void testStrcpy()
{
  printf(T_BOLD, "Testing string copy");

  // test
  char str1[50];
  char *str2 = "Hello World";
  char *expected = "Hello World";
  char *actual = c_strcpy(str1, str2);
  printTestResult(strcmp(expected, actual), 0, "testStrcpy: copy a string to another string");
}

void testStrncpy()
{
  printf(T_BOLD, "Testing string copy with n characters");

  // test
  char str1[50];
  char *str2 = "Hello World";
  size_t n = 5;
  char *expected = "Hello";
  char *actual = c_strncpy(str1, str2, n);
  printTestResult(strcmp(expected, actual), 0, "testStrncpy: copy first n characters of a string to another string");
}

void testStrchr()
{
  printf(T_BOLD, "Testing strchr");

  // test
  char *str = "Hello World";
  int c = 'W';
  char *expected = "World";
  char *actual = c_strchr(str, c);
  printTestResult(strcmp(expected, actual), 0, "testStrchr: find first occurrence of a character in a string");
}

void testStrrchr()
{
  printf(T_BOLD, "Testing strrchr");

  // test
  char *str = "Hello World";
  int c = 'o';
  char *expected = "orld";
  char *actual = c_strrchr(str, c);
  printTestResult(strcmp(expected, actual), 0, "testStrrchr: find last occurrence of a character in a string");
}

void testStrcspn()
{
  printf(T_BOLD, "Testing strcspn");

  // test 1
  char *str2 = "Hello World";
  char *reject2 = "Helo ";
  size_t expected2 = 0;
  size_t actual2 = c_strcspn(str2, reject2);
  printTestResult(expected2 == actual2 ? 0 : -1, 0, "testStrcspn: find length of initial segment not consisting of characters in reject");

  // test 2
  char *str3 = "Hello World";
  char *reject3 = "Helo";
  size_t expected3 = 0;
  size_t actual3 = c_strcspn(str3, reject3);
  printTestResult(expected3 == actual3 ? 0 : -1, 0, "testStrcspn: find length of initial segment not consisting of characters in reject");
}

void testMemset()
{
  printf(T_BOLD, "Testing memset");

  // test 1
  char str1[50] = "Hello World";
  int c1 = 'a';
  size_t n1 = 5;
  char *expected1 = "aaaaa World";
  char *actual1 = c_memset(str1, c1, n1);
  printTestResult(strcmp(expected1, actual1), 0, "testMemset: fill a block of memory with a specified value");

  // test 2
  char str2[50] = "Hello World";
  int c2 = 'b';
  size_t n2 = 11;
  char *expected2 = "bbbbbbbbbbb";
  char *actual2 = c_memset(str2, c2, n2);
  printTestResult(strcmp(expected2, actual2), 0, "testMemset: fill entire string with a specified value");

  // test 3
  char str3[50] = "Hello World";
  int c3 = 'c';
  size_t n3 = 0;
  char *expected3 = "Hello World";
  char *actual3 = c_memset(str3, c3, n3);
  printTestResult(strcmp(expected3, actual3), 0, "testMemset: fill zero characters of a string with a specified value");
}

void testGetChar()
{
  printf(T_BOLD, "Testing get_char");

  // test 1
  char *str1 = "Hello World";
  int i1 = 4;
  char expected1 = 'o';
  char actual1 = get_char(str1, i1);
  printTestResult(expected1 == actual1 ? 0 : -1, 0, "testGetChar: get character at specific index in a string");

  // test 2
  char *str2 = "Hello World";
  int i2 = -1;
  char expected2 = '\0';
  char actual2 = get_char(str2, i2);
  printTestResult(expected2 == actual2 ? 0 : -1, 0, "testGetChar: get character at negative index in a string");

  // test 3
  char *str3 = "Hello World";
  int i3 = c_strlen(str3);
  char expected3 = '\0';
  char actual3 = get_char(str3, i3);
  printTestResult(expected3 == actual3 ? 0 : -1, 0, "testGetChar: get character at index equal to string length");
}
void testIntToString()
{
  printf(T_BOLD, "Testing intToString");

  // test 1
  int num1 = 12345;
  char *expected1 = "12345";
  char *actual1 = intToString(num1);
  printTestResult(strcmp(expected1, actual1), 0, "testIntToString: convert a positive integer to a string");
}

void testCFputs()
{
  printf(T_BOLD, "Testing c_fputs");

  // test 1
  char *str1 = "Hello World";
  int fd1 = STDOUT_FILENO;
  int expected1 = strlen(str1);
  int actual1 = c_fputs(str1, fd1);
  printTestResult(expected1 == actual1 ? 0 : -1, 0, "testCFputs: write a string to stdout");

  // test 2
  char *str2 = NULL;
  int fd2 = STDOUT_FILENO;
  int expected2 = -1;
  int actual2 = c_fputs(str2, fd2);
  printTestResult(expected2 == actual2 ? 0 : -1, 0, "testCFputs: write a null string to stdout");

  // test 3
  char *str3 = "Hello World";
  int fd3 = -1;
  int expected3 = -1;
  int actual3 = c_fputs(str3, fd3);
  printTestResult(expected3 == actual3 ? 0 : -1, 0, "testCFputs: write a string to an invalid file descriptor");
}

void testCWrite()
{
  printf(T_BOLD, "Testing c_write");

  // test 1
  char *str2 = NULL;
  int fd2 = STDOUT_FILENO;
  char *color2 = "\033[31m"; // red color
  int expected2 = -1;
  int actual2 = c_write(str2, fd2, color2);
  printTestResult(expected2 == actual2 ? 0 : -1, 0, "testCWrite: write a null string to stdout");

  // test 1
  char *str3 = "Hello World";
  int fd3 = STDOUT_FILENO;
  char *color3 = NULL; // no color
  int expected3 = strlen(str3);
  int actual3 = c_write(str3, fd3, color3);
  printTestResult(expected3 == actual3 ? 0 : -1, 0, "testCWrite: write a non-colored string to stdout");
}

void testCFgets()
{
  printf(T_BOLD, "Testing c_fgets");

  // test 2
  char buffer2[50];
  int size2 = sizeof(buffer2);
  int fd2 = -1; // invalid file descriptor
  char *expected2 = NULL;
  char *actual2 = c_fgets(buffer2, size2, fd2);
  printTestResult(expected2 == actual2 ? 0 : -1, 0, "testCFgets: read a line from an invalid file descriptor");

  // test 3
  char *buffer3 = NULL;
  int size3 = 50;
  int fd3 = open("testfile.txt", O_RDONLY); // replace "testfile.txt" with the path to your test file
  char *expected3 = NULL;
  char *actual3 = c_fgets(buffer3, size3, fd3);
  printTestResult(expected3 == actual3 ? 0 : -1, 0, "testCFgets: read a line into a null buffer");
}


int main()
{
  // print in bold testing string library
  // make it orange first
  printf("\033[0;33m");
  printf(T_BOLD, "------- TESTING STRING LIBRARY -------");
  printf("\033[0;37m");
  testStrLen();
  testStrCmp();
  testStrcat();
  testStrncat();
  testStrncmp();
  testStrcpy();
  testStrncpy();
  testStrchr();
  testStrrchr();
  testStrcspn();
  testMemset();
  testGetChar();
  testIntToString();
  printf("\033[0;33m");
  printf(T_BOLD, "------- TESTING I/O LIBRARY -------");
  printf("\033[0;37m");
  testCFputs();
  testCWrite();
  testCFgets();

  return 0;
}


