#include <path.h>

#ifdef WIN
#define MAX_MARKERS 2
#else
#define MAX_MARKERS 1
#endif

/******************************************************************************/
int _pisabs(const char *path)
{
  for (int i = 0; i < MAX_MARKERS; i++) {
    if (path[i] != PATH_MARKER) {
      return i;
    }
  }
  
  return MAX_MARKERS;
}


/******************************************************************************/
int _plen(const char *buffer, char marker)
{
  int i = 0;

  while (buffer[i] != marker && buffer[i] != 0) i++;

  return i;
}

/******************************************************************************/
char **_psplit(const char *buffer)
{
  char   marker = PATH_MARKER;
  int    prot   = pprotocol(buffer, 0, NULL);
  int    abs    = _pisabs(buffer + prot);
  int    size   = 1 + (prot > 0) + (abs > 0);
  char **split  = malloc(size * sizeof(const char*));

  // Special initialization if protocol is present (will work with window's drive letter as well)
  if (prot) {
    char *protocol = malloc(prot + 1);

    memcpy(protocol, buffer, prot);
    buffer += prot;

    protocol[prot] = 0;
    split[0]       = protocol;
    marker         = '/';
  }

  // Special initialization if absolute path indicator
  if (abs) {
    char *root = malloc(abs + 1);

    memcpy(root, buffer, abs);
    buffer += abs;

    root[abs]         = 0;
    split[(prot > 0)] = root;
  }

  split[size - 1] = NULL;

  for (int i = size - 1, len; buffer[0]; i++) {
    while (1) {
      len = _plen(buffer, marker);

      if (!len && buffer[0]) {
        buffer++;
      } else break;
    }

    if (len) {
      split = realloc(split, (i + 2) * sizeof(const char*));

      if (!split) {
        FATAL("Ran out of memory!");
      }

      split[i]     = malloc(len + 1);
      split[i + 1] = NULL;

      memcpy(split[i], buffer, len);
      
      split[i][len] = 0;

      buffer += len;
    }
  }

  return split;
}

/******************************************************************************/
void _pfree(char **split)
{
  for (int i = 0; split[i]; i++) {
    free(split[i]);
  }

  free(split);
}

/******************************************************************************/
int _pclean(char **split, int size, char buffer[size]) {
  int marker = PATH_MARKER;
  int copied = 0;

  for (int i = 0; split[i]; i++) {
    if (!i) {
      char *prot = strchr(split[0], ':');

      if (prot) marker = prot[1];
    }

    int len = strlen(split[i]);
    int mrk = split[i][len - 1] == marker;

    if (copied + len + mrk > size) {
      FATAL("Buffer overflow!");
    }

    strcpy(buffer + copied, split[i]);
    copied += len;

    if (!mrk && split[i + 1]) {
      buffer[copied++] = marker;
    }
  }

  _pfree(split);

  return copied;
}

////////////////////////////////////////////////////////////////////////////////
int pisabs(const char *path)
{
  int prot = pprotocol(path, 0, NULL);

  printf("%d\n", prot);

  #ifdef WIN
    if (prot == 0) {
      int plen = _plen(path, PATH_MARKER);

      for (int i = 0; i < plen; i++) {
        if (path[i] == ':') return 1;
      }
    }
  #endif

  return _pisabs(path + prot);
}

////////////////////////////////////////////////////////////////////////////////
int pprotocol(const char *path, int size, char buffer[size])
{
  int plen = _plen(path, '/');

  for (int i = 0; i < plen; i++) {
    if (path[i] == ':' && path[i + 1] == '/' && path[i + 2] == '/') {
      i += 3;

      if (buffer) {
        if (i > size) {
          FATAL("Buffer overflow!");
        }

        memcpy(buffer, path, i);
        buffer[i] = 0;
      }

      return i;
    }
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
int pclean(const char *path, int size, char buffer[size])
{
  return _pclean(_psplit(path), size, buffer);
}

////////////////////////////////////////////////////////////////////////////////
int prel(const char *path, int size, char buffer[size])
{
  //char **split = _psplit(path);

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
int pabs(const char *path, int size, char buffer[size])
{
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
int pcombine(const char *path, const char *file, int size, char buffer[size])
{
  char **psplit = _psplit(path);
  char **fsplit = _psplit(file);

  int plen = 0, flen = 0;

  for (; psplit[plen]; plen++);
  for (; fsplit[flen]; flen++);

  int ignore = flen > 0 && fsplit[0][0] == PATH_MARKER;

  psplit = realloc(psplit, (plen + flen + 1 - ignore) * sizeof(const char*));
  
  memcpy(psplit + plen, fsplit + ignore, (flen + 1 - ignore) * sizeof(const char*));

  free(fsplit);

  return _pclean(psplit, size, buffer);
}