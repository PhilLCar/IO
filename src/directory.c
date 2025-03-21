#include <directory.h>

// Makes sure the path is terminated properly
/******************************************************************************/
void _dterminate(char *buffer, const char *path)
{
  int len = 0;

  do {
    buffer[len] = path[len];
  } while (path[len++]);

  while (path[len - 1] == PATH_MARKER) {
    buffer[--len] = 0;
  }
}

#ifdef WIN
////////////////////////////////////////////////////////////////////////////////
DirectoryIterator *dir(const char *dirname)
{
  WIN32_FIND_DATA  file;
  HANDLE           handle  = NULL;
  Array           *results = NULL;
  int              pattern = _is_pattern(dirname);
  char            *dirpath = pattern ? filepath(dirname) : _dterminate(dirname);

  if ((handle = FindFirstFile(dirname, &file)) == INVALID_HANDLE_VALUE) {
    return NULL;
  } else {
    results = NEW (array) (sizeof(DirItem));
  }

  do {
    char          buffer[2048];
    DirectoryItem di;

    sprintf(buffer, "%s%s", dirpath, file.cFileName);
    DirItem_cons(&di, buffer, 
      file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ? DIRITEM_DIRECTORY : DIRITEM_FILE);
    push(results, &di);
  }
  while(FindNextFile(handle, &file));

  FindClose(handle);
  if (dirpath != dirname) free(dirpath);
  return results;
}
#else
// Matches a file pattern
/******************************************************************************/
int _match(char *pattern, char *string)
{
  int i, j, si, sj;

  for (i = 0, j = 0, si = -1, sj = -1; string[j]; j++) {
    if (pattern[i] == '*') {
      if (si < 0) {
        si = i;
        sj = j;
      }
      // Attempt to continue matching
      if (pattern[i + 1] == string[j]) i += 2;
    } 
    else if (!pattern[i] || pattern[i] != string[j]) {
      if (si >= 0) {
        i = si;
        j = sj++;
      }
      else break;
    }
    else i++;
  }

  if (pattern[i] == '*') ++i;

  return !pattern[i] && !pattern[j];
}

////////////////////////////////////////////////////////////////////////////////
DirectoryIterator *dopen(const char *dirname)
{
  DirectoryIterator *di = malloc(sizeof(DirectoryIterator));

  // Does it contain a glob?
  if (strchr(dirname, '*') != NULL) {
    fnamext(dirname, sizeof(di->filter), di->filter);
    fpath  (dirname, sizeof(di->path),   di->path);
  } else {
    di->filter[0] = 0;
    _dterminate(di->path, dirname);
  }

  di->handle = opendir(di->path);

  if (di->handle) {
    dnext(di);
  } else {
    free(di);
    di = NULL;
  }
  
  return di;
}

////////////////////////////////////////////////////////////////////////////////
void dclose(DirectoryIterator *iterator)
{
  closedir(iterator->handle);

  free(iterator);
}


////////////////////////////////////////////////////////////////////////////////
DirectoryItem *dnext(DirectoryIterator *iterator)
{
  DirectoryItem *current = &iterator->current;

  struct dirent *d;

  while ((d = readdir(iterator->handle))) {
    // Skip relative directories
    if (!strcmp(d->d_name, ".") || !strcmp(d->d_name, "..")) continue;

    if (!iterator->filter[0] || _match(iterator->filter, d->d_name)) {

      sprintf(current->name, "%s", d->d_name);
      current->type = d->d_type == DT_DIR 
        ? DIRTYPE_DIRECTORY 
        : (d->d_type == DT_REG 
          ? DIRTYPE_FILE 
          : DIRTYPE_OTHER);
      break;
    }
  }

  if (!d) {
    current->type = DIRTYPE_DONE;
  }

  return current;
}

////////////////////////////////////////////////////////////////////////////////
int ddone(DirectoryIterator *iterator)
{
  int done = !iterator || iterator->current.type == DIRTYPE_DONE;
  
  if (done) {
    dclose(iterator);
  }

  return done;
}

////////////////////////////////////////////////////////////////////////////////
void dname(const DirectoryIterator *iterator, int size, char buffer[size])
{
  if (iterator) {
    pcombine(iterator->path, iterator->current.name, size, buffer);
  } else {
    FATAL("Iterator is NULL!");
  }
}

////////////////////////////////////////////////////////////////////////////////
int dexists(const char *dirname)
{
  DirectoryIterator *iterator = dopen(dirname);
  int                exists   = iterator != NULL;

  dclose(iterator);

  return exists;
}

#endif
