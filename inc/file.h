#ifndef FILE_H
#define FILE_H

// C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef WIN
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "osal.h"

// Enumeration of file permission types
typedef enum file_permission {
  FILE_EXISTS,
  FILE_READ,
  FILE_WRITE,
  FILE_EXECUTE
} FilePermission;

int filenamewoext(const char *filename, char *buffer, int size);
int filepath(const char *filename, char *buffer, int size);
int filepathcombine(const char *path, const char *filename, char *buffer, int size);
int fileext(const char *filename, char *buffer, int size);
int filenamewopath(const char *filename, char *buffer, int size);
int fileexists(const char *filename, FilePermission permission);

#endif