#include <directory.h>

#define TYPENAME DirItem

// Check if pattern is present
/******************************************************************************/
int _is_pattern(const char *pattern)
{
  int asterisk = 0;

  for (int i = 0; pattern[i]; i++) if (pattern[i] == '*') {
    asterisk  = 1;
    break;
  }

  return asterisk;
}

// Makes sure the path is terminated properly
/******************************************************************************/
void _slash_terminate(char *buffer, char *path)
{
  int len = strlen(path);

  strcpy(buffer, path);

  if (path[len - 1] != PATH_MARKER) {
    buffer[len] = PATH_MARKER;
    buffer[len + 1] = 0;
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
  int i, j;

  for (i = 0, j = 0; pattern[i] && string[j]; j++) {
    if (pattern[i] == '*') {
      if (pattern[i + 1] == string[j + 1]) i++;
    } 
    else if (pattern[i] != string[j]) return 0;
    else i++;
  }

  return !pattern[i];
}

////////////////////////////////////////////////////////////////////////////////
int dirlastmod(const char *dirname)
{
  int   modif = 0;
  char  buffer[256];
  FILE *result;

  sprintf(buffer, "stat -c %%Y %s", dirname);

  result = popen(buffer, "r");
  memset(buffer, 0, sizeof(buffer));

  for (int c = fgetc(result), i = 0; c != EOF; c = fgetc(result), i++) buffer[i] = c;

  modif = atoi(buffer);

  fclose(result);
  
  return modif;
}

////////////////////////////////////////////////////////////////////////////////
DirectoryIterator *dir(const char *dirname)
{
  DirectoryIterator *di       = malloc(sizeof(DirectoryIterator));
  int                pattern  = _is_pattern(dirname);

  if (pattern) {
    filenamewopath(di->name, dirname);
    filepath(di->path, dirname);
  } else {
    di->name[0] = 0;
    _slash_terminate(di->path, dirname);
  }

  di->dir = opendir(di->path);

  if (!di->dir) {
    free(di);
    di = NULL;
  }
  
  return di;
}

DirectoryInformation *dirnext(DirectoryIterator *iterator)
{
  DirectoryInformation *info = &iterator->info;
  struct dirent        *d;

  while ((d = readdir(iterator->dir))) {
    if (!strcmp(d->d_name, ".") || !strcmp(d->d_name, "..")) continue;
    if (!iterator->name[0] || _match(iterator->name, d->d_name)) {

      sprintf(info->name, "%s", d->d_name);
      info->type = d->d_type == DT_DIR ? DIRTYPE_DIRECTORY : (d->d_type == DT_REG ? DIRTYPE_FILE : DIRTYPE_OTHER);
      break;
    }
  }
  if (!d) {
    closedir(iterator->dir);
    free(iterator);
    info = NULL;
  }

  return info;
}

#endif
