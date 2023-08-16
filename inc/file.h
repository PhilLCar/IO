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

int filenamewoext(char *buffer, const char *filename);
int filepath(char *buffer, const char *filename);
int fileext(char *buffer, const char *filename);
int filenamewopath(char *buffer, const char *filename);
int fileexists(const char *filename, FilePermission permission);

#endif