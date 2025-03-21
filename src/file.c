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
int fpath(const char *filename, int size, char buffer[size])
{
  int len = max(_lindexof(filename, PATH_MARKER), 0);

  if (buffer) {
    if (size <= len) {
      FATAL("Buffer overflow!");
    }

    memcpy(buffer, filename, len);
    buffer[len] = 0;
  }
  
  return len;
}

////////////////////////////////////////////////////////////////////////////////
int fname(const char *filename, int size, char buffer[size])
{
  int l   = _lindexof(filename, PATH_MARKER) + 1;
  int len = _findexof(filename + l, '.');

  if (buffer) {
    if (size <= len) {
      FATAL("Buffer overflow!");
    }

    memcpy(buffer, filename + l, len);
    buffer[len] = 0;
  }
  
  return len;
}

////////////////////////////////////////////////////////////////////////////////
int fext(const char *filename, int size, char buffer[size])
{
  int l   = _lindexof(filename, PATH_MARKER) + 1;
  int ext = _findexof(filename + l, '.') + l;
  int len = strlen(filename + ext);

  if (buffer) {
    if (size <= len) {
      FATAL("Buffer overflow!");
    }

    memcpy(buffer, filename + ext, len);
    buffer[len] = 0;
  }
  
  return len;
}

////////////////////////////////////////////////////////////////////////////////
int fnamext(const char *filename, int size, char buffer[size])
{
  int name = _lindexof(filename, PATH_MARKER) + 1;
  int len  = strlen(filename + name);

  if (buffer) {
    if (size <= len) {
      FATAL("Buffer overflow!");
    }

    memcpy(buffer, filename + name, len);
    buffer[len] = 0;
  }
  
  return len;
}

#ifdef WIN

////////////////////////////////////////////////////////////////////////////////
int fexists(const char *filename, FilePermission permission) {
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
int fexists(const char *filename, FilePermission permission) {
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