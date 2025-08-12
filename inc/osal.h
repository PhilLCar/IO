#ifndef OSAL_UTILS
#define OSAL_UTILS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef WIN
// Windows
#define long long long
#define PATH_MARKER '\\'
#define PATH_MAX_LENGTH 260 /* This is insanely small?? */
#undef max
#undef min
#else
// Linux
#define PATH_MARKER '/'
#define PATH_MAX_LENGTH 4096
#endif

#include <diagnostic.h>
#include "log.h"
#include "export.h"

static inline int max(int a, int b) {
  return a > b ? a : b;
}

static inline int min(int a, int b) {
  return a < b ? a : b;
}

PUBLIC void sysargs(const char *command, int argc, const char *argv[argc]);
PUBLIC void *run(const char *command);
PUBLIC void *runargs(const char *command, int argc, const char *argv[argc]);

PUBLIC void newdir(const char *dirname);
PUBLIC void deldir(const char *dirname);
PUBLIC void newfile(const char *filename);
PUBLIC void delfile(const char *filename);
PUBLIC long lastmod(const char *filename);
PUBLIC void workdir(int size, char buffer[size]);

#endif