#include <log.h>

void _debug(const char *filename, int line, const char *message)
{
  printf(FONT_BOLD TEXT_CYAN "debug:" FONT_RESET FONT_BOLD " %s(%d): " FONT_RESET "%s\n", filename, line, message);
}

void _info(const char *filename, int line, const char *message)
{
  printf(FONT_BOLD TEXT_GREEN "info:" FONT_RESET FONT_BOLD "  %s(%d): " FONT_RESET "%s\n", filename, line, message);
}

void _warn(const char *filename, int line, const char *message)
{
  fprintf(stderr, FONT_BOLD TEXT_YELLOW "warn:" FONT_RESET FONT_BOLD "  %s(%d): " FONT_RESET "%s\n", filename, line, message);
}

void _error(const char *filename, int line, const char *message)
{
  fprintf(stderr, FONT_BOLD TEXT_RED "error:" FONT_RESET FONT_BOLD " %s(%d): " FONT_RESET "%s\n", filename, line, message);
}

void _fatal(const char *filename, int line, const char *message)
{
  fprintf(stderr, FONT_BOLD TEXT_YELLOW BG_RED "fatal:" FONT_RESET FONT_BOLD " %s(%d): " FONT_RESET "%s\n", filename, line, message);
  exit(-1);
}


void loglvl(const char *filename, int line, int lvl, const char *message, ...)
{
  char buffer[LOG_MSG_MAX];

  va_list argptr;
  va_start(argptr, message);
  vsnprintf(buffer, sizeof(buffer), message, argptr);
  va_end(argptr);

  switch (lvl) {
    case LOG_DEBUG:
      _debug(filename, line, buffer);
      break;
    case LOG_INFO:
      _info(filename, line, buffer);
      break;
    case LOG_WARN:
      _warn(filename, line, buffer);
      break;
    case LOG_ERROR:
      _error(filename, line, buffer);
      break;
    case LOG_FATAL:
      _fatal(filename, line, buffer);
      break;
    default:
      _error(filename, line, "Unknown log level!");
  }
}