#ifndef PATH_H
#define PATH_H

#include <osal.h>

int pisabs(const char *path);
int pprotocol(const char *path, int size, char buffer[size]);

int pclean(const char *path, int size, char buffer[size]);
int prel  (const char *path, int size, char buffer[size]);
int pabs  (const char *path, int size, char buffer[size]);

int pcombine(const char *path, const char *filename, int size, char buffer[size]);

#endif