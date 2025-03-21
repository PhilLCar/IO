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

int  fpath(const char *filename, int size, char buffer[size]);
int  fname(const char *filename, int size, char buffer[size]);
int  fext(const char *filename, int size, char buffer[size]);
int  fnamext(const char *filename, int size, char buffer[size]);
int  fexists(const char *filename, FilePermission permission);

#endif