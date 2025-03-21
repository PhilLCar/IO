#ifndef LOG_H
#define LOG_H

#include <terminal.h>
#include <stdlib.h>

#define LOG_DEBUG 0
#define LOG_INFO  1
#define LOG_WARN  2
#define LOG_ERROR 3
#define LOG_FATAL 4

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_INFO
#endif

#define LOG(LEVEL, MESSAGE) loglvl(__FILE__, __LINE__, LEVEL, MESSAGE);

#if LOG_LEVEL == LOG_DEBUG
#define DEBUG(MESSAGE) LOG(LOG_DEBUG, MESSAGE)
#else
#define DEBUG(MESSAGE)
#endif

#if LOG_LEVEL == LOG_DEBUG || LOG_LEVEL == LOG_INFO
#define INFO(MESSAGE) LOG(LOG_INFO,  MESSAGE)
#else
#define INFO(MESSAGE)
#endif

#if LOG_LEVEL == LOG_DEBUG || LOG_LEVEL == LOG_INFO || LOG_LEVEL == LOG_WARN
#define WARN(MESSAGE) LOG(LOG_WARN,  MESSAGE)
#else
#define WARN(MESSAGE)
#endif 

#if LOG_LEVEL == LOG_DEBUG || LOG_LEVEL == LOG_INFO || LOG_LEVEL == LOG_WARN || LOG_LEVEL == LOG_ERROR
#define ERROR(MESSAGE) LOG(LOG_ERROR, MESSAGE)
#else
#define ERROR(MESSAGE)
#endif

// Fatal errors are always included
#define FATAL(MESSAGE) LOG(LOG_FATAL, MESSAGE)

void loglvl(const char *filename, int line, int lvl, const char *message);

#endif