#include <file.h>

/******************************************************************************/
int _lindexof(const char *buffer, char c)
{
  int index = -1;

  for (int i = 0;; i++) {
    if (!buffer[i]) {
      if (index < 0) index = i;
      break;
    } else if (buffer[i] == c) index = i;
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
int filenamewoext(char *buffer, const char *filename)
{
  int l   = _lindexof(filename, PATH_MARKER) + 1;
  int len = _findexof(filename + l, '.') + l;

  if (buffer) {
    memcpy(buffer, filename, len);
    buffer[len] = 0;
  }
  
  return len;
}

////////////////////////////////////////////////////////////////////////////////
int filepath(char *buffer, const char *filename)
{
  int len = _lindexof(filename, PATH_MARKER) + 1;

  if (buffer) {
    memcpy(buffer, filename, len);
    buffer[len] = 0;
  }
  
  return len;
}

////////////////////////////////////////////////////////////////////////////////
int fileext(char *buffer, const char *filename)
{
  int l   = _lindexof(filename, PATH_MARKER) + 1;
  int len = _findexof(filename + l, '.') + l;

  if (buffer) strcpy(buffer, filename + len);
  
  return len;
}

////////////////////////////////////////////////////////////////////////////////
int filenamewopath(char *buffer, const char *filename)
{
  int len = _lindexof(filename, PATH_MARKER) + 1;

  if (buffer) strcpy(buffer, filename + len);
  
  return len;
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
