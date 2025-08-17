#include <path.h>

#ifdef WIN
#define MAX_MARKERS 2
#else
#define MAX_MARKERS 1
#endif

enum _step {
  _PATH_PROTOCOL,
  _PATH_DRIVE,
  _PATH_ABSOLUTE,
  _PATH_COMPONENTS
};

struct _path {
  char   marker;
  char  *protocol;
  char  *drive;
  int    absolute;
  int    size;
  char **components;
};

/******************************************************************************/
int _plen(const char *buffer, char marker)
{
  int i;

  for (i = 0; buffer[i] && buffer[i] != marker; i++);

  return i;
}

/******************************************************************************/
int _pprotocol(struct _path *path, const char *buffer)
{
  // URI always use forward slash
  int plen = _plen(buffer, '/');

  for (int i = 0; i < plen; i++) {
    if (buffer[i] == ':' && buffer[i + 1] == '/' && buffer[i + 2] == '/') {
      i += 3;

      path->marker = '/';
      path->protocol = malloc((i + 1) * sizeof(char));

      strncpy(path->protocol, buffer, i);

      return i;
    }
  }

  return 0;
}

/******************************************************************************/
int _pdrive(struct _path *path, const char *buffer)
{
  int plen = _plen(buffer, path->marker);

  // Windows network drive
  if (plen == 0 && path->marker == '\\' && buffer[0] == '\\' && buffer[1] == '\\') {
    plen = _plen(buffer + 2, '\\');
  } else if (plen && buffer[plen - 1] != ':') {
    plen = 0;
  }

  if (plen) {
    path->drive = malloc((plen + 1) * sizeof(char));

    strncpy(path->drive, buffer, plen);
  }

  return plen;
}

/******************************************************************************/
void _pinit(struct _path *path)
{
  memset(path, 0, sizeof(struct _path));

  path->marker = PATH_MARKER;
}

/******************************************************************************/
void _pbuild(struct _path *path, const char *buffer, enum _step step)
{
  int index = 0;
  
  _pinit(path);

  index += _pprotocol(path, buffer);

  if (step == _PATH_PROTOCOL) return;
  
  index += _pdrive(path, buffer + index);

  if (step == _PATH_DRIVE) return;

  path->absolute = buffer[index] == path->marker;

  if (step == _PATH_ABSOLUTE) return;

  {
    int cap = 2;

    path->components = (char**)malloc(cap * sizeof(char*));
    path->size       = 0;

    for (int i = index, s = index - 1; ; i++) {
      if (!buffer[i] || buffer[i] == path->marker) {
        int len = i - ++s;

        if (len) {
          path->components[path->size] = malloc((len + 1) * sizeof(char));
          
          if (!path->components[path->size]) {
            FATAL("Memory allocation error!");
          }

          strncpy(path->components[path->size++], &buffer[s], len);

          if (path->size == cap && buffer[i]) {
            path->components = realloc(path->components, (cap <<= 1) * sizeof(char*));

            if (!path->components) {
              FATAL("Memory reallocation error!");
            }
          }
        }

        if (!buffer[i]) break;

        s = i;
      }
    }
  }
}

/******************************************************************************/
void _pfree(struct _path *path)
{
  if (path->protocol) free(path->protocol);
  if (path->drive)    free(path->drive);

  for (int i = 0; i < path->size; i++) {
    if (path->components[i]) free(path->components[i]);
  }

  if (path->components) free(path->components);
}

/******************************************************************************/
int _pclean(struct _path *path, int size, char destination[size]) {
  int copied  = 0;
  int nothing = 1;

  if (path->protocol) {
    int len = strlen(path->protocol);

    if ((copied + len) >= size) {
      FATAL("Buffer overflow!");
    }

    strncpy(&destination[copied], path->protocol, len);

    copied += len;
  }

  if (path->drive) {
    int len = strlen(path->drive);

    if ((copied + len) >= size) {
      FATAL("Buffer overflow!");
    }

    strncpy(&destination[copied], path->drive, len);

    copied += len;
  }

  if (path->absolute) {
    if (copied + 2 >= size) {
      FATAL("Buffer overflow!");
    }

    destination[copied++] = path->marker;
    destination[copied]   = 0;
  }

  for (int i = 0; i < path->size; i++) {
    if (!strcmp(".", path->components[i])) {
      free(path->components[i]);
      path->components[i] = NULL;
    } else if (!strcmp("..", path->components[i])) {
      for (int j = i - 1; j >= 0; j--) {
        if (path->components[j] && strcmp("..", path->components[j])) {
          // only free if parent is present;
          free(path->components[i]);
          free(path->components[j]);
          path->components[i] = NULL;
          path->components[j] = NULL;
          break;
        }
      }
    }
  }

  for (int i = 0, j = 0; i < path->size; i++) {
    if (path->components[i]) {
      int len = strlen(path->components[i]);

      if ((copied + len + j) >= size) {
        FATAL("Buffer overflow!");
      }

      if (j) {
        destination[copied] = path->marker;
      }

      strncpy(&destination[copied + j], path->components[i], len);

      copied += len + j;
      j       = 1;
      nothing = 0;
    }
  }

  if (nothing) {
    if (copied + 2 >= size) {
      FATAL("Buffer overflow!");
    }

    destination[copied++] = '.';
    destination[copied]   = 0;
  }

  _pfree(path);

  return copied;
}

/******************************************************************************/
int _pcombine(struct _path *fpath, struct _path *spath, int size, char destination[size])
{
  fpath->components = realloc(fpath->components, (fpath->size + spath->size) * sizeof(char*));

  if (!fpath->components) {
    FATAL("Memory reallocation error!");
  }

  for (int i = fpath->size, j = 0; i < fpath->size + spath->size; i++, j++) {
    fpath->components[i] = spath->components[j];
    spath->components[j] = NULL;
  }

  fpath->size += spath->size;

  _pfree(spath);

  return _pclean(fpath, size, destination);
}

////////////////////////////////////////////////////////////////////////////////
int pisabs(const char *buffer)
{
  struct _path path;

  _pbuild(&path, buffer, _PATH_ABSOLUTE);

  _pfree(&path);

  return path.absolute;
}

////////////////////////////////////////////////////////////////////////////////
int pprotocol(const char *buffer, int size, char destination[size])
{
  int          len;
  struct _path path;

  _pbuild(&path, buffer, _PATH_PROTOCOL);

  len = path.protocol && strlen(path.protocol);

  if (destination) {
    if (len + 1 < size) {
      memcpy(destination, path.protocol, len);
      destination[len + 1] = 0;
    } else {
      FATAL("Buffer overflow!");
    }
  }

  _pfree(&path);

  return len;
}

////////////////////////////////////////////////////////////////////////////////
int pdrive(const char *buffer, int size, char destination[size])
{
  int          len;
  struct _path path;

  _pbuild(&path, buffer, _PATH_DRIVE);

  len = path.drive && strlen(path.drive);

  if (destination) {
    if (len + 1 < size) {
      memcpy(destination, path.drive, len);
      destination[len + 1] = 0;
    } else {
      FATAL("Buffer overflow!");
    }
  }

  _pfree(&path);

  return len;
}

////////////////////////////////////////////////////////////////////////////////
int pclean(const char *buffer, int size, char destination[size])
{
  struct _path path;

  _pbuild(&path, buffer, _PATH_COMPONENTS);

  return _pclean(&path, size, destination);
}

////////////////////////////////////////////////////////////////////////////////
int prel(const char *buffer, int size, char destination[size])
{
  struct _path path;

  _pbuild(&path, buffer, _PATH_COMPONENTS);

  if (!path.absolute) {
    return _pclean(&path, size, destination);
  } else {
    char         wd[PATH_MAX_LENGTH];
    struct _path abs;
    int          match = 0;

    workdir(sizeof(wd), wd);
    _pbuild(&abs, wd, _PATH_COMPONENTS);
    
    if (abs.drive && path.drive && strcmp(abs.drive, path.drive)) {
      _pfree(&abs);

      return _pclean(&path, size, destination);
    }

    for (int i = 0, m = 1; i < abs.size; i++) {
      if (m && !strcmp(path.components[i], abs.components[i])) {
        free(abs.components[i]);
        free(path.components[i]);
        abs.components[i]  = NULL;
        path.components[i] = NULL;
      } else {
        if (m) {
          match = i;
          m = 0;
        }

        free(abs.components[i]);
        abs.components[i] = malloc(3 * sizeof(char));
        strcpy(abs.components[i], "..");
      }
    }

    for (int i = 0; i < abs.size - match; i++) {
      abs.components[i] = abs.components[i + match];
    }

    for (int i = 0; i < path.size - match; i++) {
      path.components[i] = path.components[i + match];
    }

    abs.size     -= match;
    path.size    -= match;
    abs.absolute  = 0;

    free(abs.drive);
    abs.drive = NULL;

    return _pcombine(&abs, &path, size, destination);
  }
}

////////////////////////////////////////////////////////////////////////////////
int pabs(const char *buffer, int size, char destination[size])
{
  struct _path path;

  _pbuild(&path, buffer, _PATH_COMPONENTS);

  if (path.absolute) {
    return _pclean(&path, size, destination);
  } else {
    char         wd[PATH_MAX_LENGTH];
    struct _path rel;

    workdir(sizeof(wd), wd);
    _pbuild(&rel, wd, _PATH_COMPONENTS);

    return _pcombine(&rel, &path, size, destination);
  }
}

////////////////////////////////////////////////////////////////////////////////
int pcombine(const char *first, const char *second, int size, char destination[size])
{
  struct _path fpath;
  struct _path spath;

  _pbuild(&fpath, first,  _PATH_COMPONENTS);
  _pbuild(&spath, second, _PATH_COMPONENTS);

  return _pcombine(&fpath, &spath, size, destination);
}