#ifndef PATH_H
#define PATH_H

#include <diagnostic.h>
#include "osal.h"
#include "io.export.h"

IO_EXPORT int pisabs(const char *path);
IO_EXPORT int pprotocol(const char *path, int size, char buffer[size]);
IO_EXPORT int pdrive(const char *path, int size, char buffer[size]);

IO_EXPORT int pclean(const char *path, int size, char buffer[size]);
IO_EXPORT int prel  (const char *path, int size, char buffer[size]);
IO_EXPORT int pabs  (const char *path, int size, char buffer[size]);

IO_EXPORT int pcombine(const char *path, const char *filename, int size, char buffer[size]);

#endif