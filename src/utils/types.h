/**
 * @file types.h
 * @brief A header file defining custom types and constants for file descriptors
 * @authors Tobias Wonwdwosse, Jayden Mingle
 * 
 * Details: This header file defines several custom types and constants to be used in the program. The custom 
 * types include a boolean type (bool), and unsigned integer types of various sizes (UINT8, UINT16, UINT32). 
 * It also defines a File type as an unsigned int. In addition, it defines constants for standard input, output, 
 * and error file descriptors (FILE_STDIN, FILE_STDOUT, FILE_STDERR).
 * 
 * Assumptions/Limitations: The types and constants defined in this file are assumed to be compatible with the 
 * rest of the program. The File type is defined as an unsigned int, which may not be compatible with all systems 
 * or libraries. The UINT8, UINT16, and UINT32 types are defined using the char, int, and long types respectively,
 * which may not always be 8, 16, and 32 bits on all systems.
 *
 *@date 2023-10-20
 */
#ifndef TYPES_H
#define TYPES_H

// *** custom types *** //
typedef enum { false, true } bool;
typedef unsigned char UINT8;
typedef unsigned int  UINT16;
typedef unsigned long UINT32;
typedef unsigned int File;

#define FILE_STDIN 0
#define FILE_STDOUT 1
#define FILE_STDERR 2

#endif