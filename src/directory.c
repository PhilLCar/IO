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

// Check if the directory name is a pattern (glob)
/******************************************************************************/
int _is_pattern(const char *path)
{
  return strchr(path, '*') != NULL;
}

#ifdef WIN

void _fill_current_item(DirectoryItem *current, WIN32_FIND_DATAA *file)
{
  sprintf(current->name, "%s", file->cFileName);
  current->type = file->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ? DIRTYPE_DIRECTORY : DIRTYPE_FILE;
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

#endif

////////////////////////////////////////////////////////////////////////////////
DirectoryIterator *dopen(const char *dirname)
{
  DirectoryIterator *di = malloc(sizeof(DirectoryIterator));

  // Does it contain a glob?
  if (_is_pattern(dirname)) {
    fnamext(dirname, sizeof(di->filter), di->filter);
    fpath  (dirname, sizeof(di->path),   di->path);
  } else {
    di->filter[0] = 0;
    _dterminate(di->path, dirname);
  }

#ifdef WIN
  {
    WIN32_FIND_DATAA file;

    di->handle = FindFirstFileA(dirname, &file);

    if (di->handle == INVALID_HANDLE_VALUE) {
      free(di);
      di = NULL;
    } else {
      _fill_current_item(&di->current, &file);
    }
  }
#else
  if ((di->handle = opendir(di->path))) {
    dnext(di);
  } else {
    free(di);
    di = NULL;
  }
#endif
  
  return di;
}

////////////////////////////////////////////////////////////////////////////////
void dclose(DirectoryIterator *iterator)
{
#ifdef WIN
  FindClose(iterator->handle);
#else
  closedir(iterator->handle);
#endif

  free(iterator);
}


////////////////////////////////////////////////////////////////////////////////
DirectoryItem *dnext(DirectoryIterator *iterator)
{
  DirectoryItem *current = &iterator->current;

#ifdef WIN
  {
    WIN32_FIND_DATAA file;

    if (FindNextFileA(iterator->handle, &file)) {
      _fill_current_item(current, &file);
    } else {
      current->type = DIRTYPE_DONE;
    }
  }
#else
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
#endif

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
