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
    int max = size > len ? len : size;

    memcpy(buffer, filename, max - 1);
    buffer[max] = 0;
  }
  
  return len;
}

////////////////////////////////////////////////////////////////////////////////
int filepath(const char *filename, char *buffer, int size)
{
  int len = _lindexof(filename, PATH_MARKER) + 1;

  if (buffer) {
    int max = size > len ? len : size;

    memcpy(buffer, filename, max - 1);
    buffer[max] = 0;
  }
  
  return len;
}

////////////////////////////////////////////////////////////////////////////////
int fileext(const char *filename, char *buffer, int size)
{
  int l   = _lindexof(filename, PATH_MARKER) + 1;
  int len = _findexof(filename + l, '.') + l;
  int ext = strlen(filename + len) + 1;

  if (buffer) {
    int max = size > ext ? ext : size;

    memcpy(buffer, filename + len, max - 1);
    buffer[max] = 0;
  }
  
  return ext;
}

////////////////////////////////////////////////////////////////////////////////
int filenamewopath(const char *filename, char *buffer, int size)
{
  int len  = _lindexof(filename, PATH_MARKER) + 1;
  int name = strlen(filename + len) + 1;

  if (buffer) {
    int max = size > name ? name : size;

    memcpy(buffer, filename + len, max - 1);
    buffer[max] = 0;
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
