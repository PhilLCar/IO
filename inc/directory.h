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

#include "osal.h"
#include "file.h"
#include "path.h"

typedef enum dir_type {
  DIRTYPE_FILE,
  DIRTYPE_DIRECTORY,
  DIRTYPE_OTHER,
  DIRTYPE_DONE
} DirectoryType;

typedef struct dir_item {
  char          name[PATH_MAX_LENGTH];
  DirectoryType type;
} DirectoryItem;

typedef struct dir_iterator {
  char           path[PATH_MAX_LENGTH];
  char           filter[PATH_MAX_LENGTH];
  void          *handle;
  DirectoryItem  current;
} DirectoryIterator;

DirectoryIterator *dopen(const char *dirname);
void               dclose(DirectoryIterator *iterator);
DirectoryItem     *dnext(DirectoryIterator *iterator);
int                ddone(DirectoryIterator *iterator);
void               dclose(DirectoryIterator *iterator);
void               dname(const DirectoryIterator *iterator, int size, char buffer[size]);
int                dexists(const char *dirname);

#endif