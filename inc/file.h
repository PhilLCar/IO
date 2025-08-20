#ifndef FILE_H
#define FILE_H

// C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN
#include <windows.h>
#undef FILE_EXECUTE
#else
#include <unistd.h>
#endif

#include <osal.h>
#include "io.export.h"

// Enumeration of file permission types
typedef enum file_permission {
  FILE_EXISTS,
  FILE_READ,
  FILE_WRITE,
  FILE_EXECUTE
} FilePermission;

IO_EXPORT int fpath(const char *filename, int size, char buffer[size]);
IO_EXPORT int fname(const char *filename, int size, char buffer[size]);
IO_EXPORT int fext(const char *filename, int size, char buffer[size]);
IO_EXPORT int fnamext(const char *filename, int size, char buffer[size]);
IO_EXPORT int fexists(const char *filename, FilePermission permission);

#endif