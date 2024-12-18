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

#include <diagnostic.h>

#include "file.h"
#include "osal.h"

#define DIRECTORY_PATH_MAXLEN 2048
#define DIRECTORY_FILE_MAXLEN 2048

typedef enum dir_type {
  DIRTYPE_FILE,
  DIRTYPE_DIRECTORY,
  DIRTYPE_OTHER
} DirectoryType;

typedef struct dir_item {
  char          name[DIRECTORY_FILE_MAXLEN];
  DirectoryType type;
} DirectoryItem;

typedef struct dir_iterator {
  char           path[DIRECTORY_PATH_MAXLEN];
  char           filter[DIRECTORY_FILE_MAXLEN];
  void          *handle;
  DirectoryItem  current;
} DirectoryIterator;

int                dlastmod(const char *dirname);
DirectoryIterator *dopen(const char *dirname);
DirectoryItem     *dnext(DirectoryIterator **iterator);
void               dclose(DirectoryIterator **iterator);
void               dfullname(const DirectoryIterator *iterator, int size, char buffer[size]);
void               dcreate(const char *dirname);
void               dremove(const char *dirname);

#endif