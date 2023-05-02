#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN
#include <windows.h>
#else
#ifndef __USE_MISC
#define __USE_MISC
#endif
#include <dirent.h>
#endif

#include "file.h"
#include "osal.h"

#define DIRECTORY_PATH_MAXLEN 2048
#define DIRECTORY_FILE_MAXLEN 2048

typedef enum dir_type {
  DIRTYPE_FILE,
  DIRTYPE_DIRECTORY,
  DIRTYPE_OTHER
} DirectoryType;

typedef struct dir_info {
  char          name[DIRECTORY_FILE_MAXLEN];
  DirectoryType type;
} DirectoryInformation;

typedef struct dir_iterator {
  char                  path[DIRECTORY_PATH_MAXLEN];
  char                  name[DIRECTORY_FILE_MAXLEN];
  DIR                  *dir;
  DirectoryInformation  info;
} DirectoryIterator;

int                   dirlastmod(const char *dirname);
DirectoryIterator    *dir(const char *dirname);
DirectoryInformation *dirnext(DirectoryIterator *iterator);

#endif