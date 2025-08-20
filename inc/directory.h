#ifndef DIRECTORY_H
#define DIRECTORY_H

// C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// CUT
#include <diagnostic.h>
#include "osal.h"
#include "file.h"
#include "path.h"
#include "io.export.h"

#ifdef WIN
#include <windows.h>
#else
#ifndef __USE_MISC
#define __USE_MISC
#endif
#include <dirent.h>
#endif


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

IO_EXPORT DirectoryIterator *dopen(const char *dirname);
IO_EXPORT void               dclose(DirectoryIterator *iterator);
IO_EXPORT DirectoryItem     *dnext(DirectoryIterator *iterator);
IO_EXPORT int                ddone(DirectoryIterator *iterator);
IO_EXPORT void               dclose(DirectoryIterator *iterator);
IO_EXPORT void               dname(const DirectoryIterator *iterator, int size, char buffer[size]);
IO_EXPORT int                dexists(const char *dirname);

#endif