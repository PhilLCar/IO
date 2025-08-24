#ifndef OSAL_UTILS
#define OSAL_UTILS

// C
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// CUT
#include <diagnostic.h>
#include "log.h"
#include "io.export.h"

#ifdef WIN
// Windows
#define long long long
#define PATH_MARKER '\\'
#define PATH_MAX_LENGTH 260 /* This is insanely small?? */
#undef max
#undef min
#define fopen(PATH, PERM) ({ FILE *stream; fopen_s(&stream, PATH, PERM); stream;})
#define popen  _popen
#define pclose _pclose
#define memcpy(D, S, L) memcpy_s(D, L, S, L)
#define strcpy(D, S)    strcpy_s(D, strlen(S) + 1, S)
#define strncpy(D, S, L) strncpy_s(D, L + 1, S, L)
#else
// Linux
#define PATH_MARKER '/'
#define PATH_MAX_LENGTH 4096
#endif

static inline __attribute__((unused)) int max(int a, int b) {
  return a > b ? a : b;
}

static inline __attribute__((unused)) int min(int a, int b) {
  return a < b ? a : b;
}

IO_EXPORT int  nstrcpy(char *dst, const char *src, unsigned int n);
IO_EXPORT void sysargs(const char *command, int argc, const char *argv[argc]);
IO_EXPORT void *run(const char *command);
IO_EXPORT void *runargs(const char *command, int argc, const char *argv[argc]);

IO_EXPORT void newdir(const char *dirname);
IO_EXPORT void deldir(const char *dirname);
IO_EXPORT void newfile(const char *filename);
IO_EXPORT void delfile(const char *filename);
IO_EXPORT long lastmod(const char *filename);
IO_EXPORT void workdir(int size, char buffer[size]);

#endif