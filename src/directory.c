#include <directory.h>

// Makes sure the path is terminated properly
/******************************************************************************/
void _slash_terminate(char *buffer, const char *path)
{
  int len = 0;

  do {
    buffer[len] = path[len];
  } while (path[len++]);

  if (path[len - 2] != PATH_MARKER) {
    buffer[len - 1] = PATH_MARKER;
    buffer[len] = 0;
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
  char            *dirpath = pattern ? filepath(dirname) : _slash_terminate(dirname);

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
int dlastmod(const char *dirname)
{
  int   modif = 0;
  char  buffer[256];
  FILE *result;

  sprintf(buffer, "stat -c %%Y %s", dirname);

  result = popen(buffer, "r");
  memset(buffer, 0, sizeof(buffer));

  for (int c = fgetc(result), i = 0; c != EOF; c = fgetc(result), i++) buffer[i] = c;

  modif = atoi(buffer);

  pclose(result);
  
  return modif;
}

////////////////////////////////////////////////////////////////////////////////
DirectoryIterator *dopen(const char *dirname)
{
  DirectoryIterator *di = malloc(sizeof(DirectoryIterator));

  // Does it contain a glob?
  if (strchr(dirname, '*') != NULL) {
    filenamewopath(di->filter, dirname);
    filepath(di->path, dirname);
  } else {
    di->filter[0] = 0;
    _slash_terminate(di->path, dirname);
  }

  di->handle = opendir(di->path);

  if (di->handle) {
    dnext(&di);
  } else {
    free(di);
    di = NULL;
  }
  
  return di;
}

DirectoryItem *dnext(DirectoryIterator **iterator)
{
  DirectoryIterator *iter    = *iterator;
  DirectoryItem     *current = &iter->current;

  struct dirent *d;

  while ((d = readdir(iter->handle))) {
    // Skip relative directories
    if (!strcmp(d->d_name, ".") || !strcmp(d->d_name, "..")) continue;

    if (!iter->filter[0] || _match(iter->filter, d->d_name)) {

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
    dclose(iterator);
  }

  return current;
}

void dclose(DirectoryIterator **iterator)
{
  if (*iterator)
  {
    closedir((*iterator)->handle);

    free(*iterator);
    *iterator = NULL;
  }
}

#endif
