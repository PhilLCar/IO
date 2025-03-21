#ifndef OSAL_UTILS
#define OSAL_UTILS

#include <stdio.h>
#include <string.h>
#include <log.h>
#include <stdlib.h>

#ifdef WIN
// Windows
#define long long long
#define PATH_MARKER '\\'
#define PATH_MAX_LENGTH 260 /* This is insanely small?? */
#else
// Linux
#define PATH_MARKER '/'
#define PATH_MAX_LENGTH 4096
#endif

static inline int max(int a, int b) {
  return a > b ? a : b;
}

static inline int min(int a, int b) {
  return a < b ? a : b;
}

void  sysargs(const char *command, int argc, const char *argv[argc]);
void *run(const char *command);
void *runargs(const char *command, int argc, const char *argv[argc]);

void newdir(const char *dirname);
void deldir(const char *dirname);
void newfile(const char *filename);
void delfile(const char *filename);
long lastmod(const char *filename);
void workdir(int size, char buffer[size]);

#endif