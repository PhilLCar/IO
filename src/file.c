#include <file.h>

/******************************************************************************/
int _lindexof(const char *buffer, char c)
{
  int index = -1;

  for (int i = 0; buffer[i]; i++) {
    if (buffer[i] == c) index = i;
  }

  return index;
}

/******************************************************************************/
int _findexof(const char *buffer, char c)
{
  int index = -1;

  for (int i = 0; buffer[i]; i++) {
    if (buffer[i] == c) {
      index = i;
      break;
    }
  }

  return index;
}

////////////////////////////////////////////////////////////////////////////////
int filenamewoext(const char *filename, char *buffer, int size)
{
  int l   = _lindexof(filename, PATH_MARKER) + 1;
  int len = _findexof(filename + l, '.') + l;

  if (buffer) {
    int min = size > len ? len : size;

    memcpy(buffer, filename, min);
    buffer[min] = 0;
  }
  
  return len;
}

////////////////////////////////////////////////////////////////////////////////
int filepath(const char *filename, char *buffer, int size)
{
  int len = _lindexof(filename, PATH_MARKER) + 1;

  if (buffer) {
    int min = size > len ? len : size;

    memcpy(buffer, filename, min);
    buffer[min] = 0;
  }
  
  return len;
}

////////////////////////////////////////////////////////////////////////////////
int filepathcombine(const char *path, const char *file, char *buffer, int size)
{
  int path_len = strlen(path);
  int file_len = strlen(file);
  int add      = path[path_len - 1] != PATH_MARKER && file[0] != PATH_MARKER;

  int total = path_len + file_len + add;

  if (total < size)
  {
    if (add) {
      sprintf(buffer, "%s%c%s", path, PATH_MARKER, file);
    } else {
      sprintf(buffer, "%s%s", path, file);
    }
  } else {
    total = 0;
  }

  return total;
}

////////////////////////////////////////////////////////////////////////////////
int fileext(const char *filename, char *buffer, int size)
{
  int l   = _lindexof(filename, PATH_MARKER) + 1;
  int len = _findexof(filename + l, '.') + l;
  int ext = strlen(filename + len);

  if (buffer) {
    int min = size > ext ? ext : size;

    memcpy(buffer, filename + len, min);
    buffer[min] = 0;
  }
  
  return ext;
}

////////////////////////////////////////////////////////////////////////////////
int filenamewopath(const char *filename, char *buffer, int size)
{
  int len  = _lindexof(filename, PATH_MARKER) + 1;
  int name = strlen(filename + len);

  if (buffer) {
    int min = size > name ? name : size;

    memcpy(buffer, filename + len, min);
    buffer[min] = 0;
  }
  
  return name;
}

#ifdef WIN
////////////////////////////////////////////////////////////////////////////////
int fileexists(const char *filename, FilePermission permission) {
   WIN32_FIND_DATA file;
   HANDLE handle = FindFirstFile(filename, &file) ;
   int found = handle != INVALID_HANDLE_VALUE;
   if(found) 
   {
       FindClose(handle);
   }
   // For now ignore permission on windows
   permission;
   return found;
}
#else
////////////////////////////////////////////////////////////////////////////////
int fileexists(const char *filename, FilePermission permission) {
  int a = 0;

  switch (permission) {
  case FILE_EXISTS:
    a = F_OK;
    break;
  case FILE_READ:
    a = R_OK;
    break;
  case FILE_WRITE:
    a = W_OK;
    break;
  case FILE_EXECUTE:
    a = X_OK;
    break;
  }

  return !access(filename, a);
}
#endif
