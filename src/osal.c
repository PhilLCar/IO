#include <osal.h>

char *_command_buffer(const char *command, int argc, const char *argv[argc]) {
  int start = strlen(command);

  char *cmd = malloc(start + 1);

  strcpy(cmd, command);

  for (int i = 0; i < argc; i++) {
    int len = strlen(argv[i]);

    cmd[start++] = ' ';

    cmd = realloc(cmd, start + len + 1);

    if (!cmd) {
      FATAL("Ran out of memory!");
    }

    strcpy(cmd + start, argv[i]);

    start += len;
  }

  return cmd;
}

void sysargs(const char *command, int argc, const char *argv[argc])
{
  char *buffer = _command_buffer(command, argc, argv);

  system(buffer);
  free(buffer);
}

void* run(const char *command)
{
  size_t  init   = 4096;
  FILE   *result = popen(command, "r");
  char   *text   = malloc(init);

  for (int c = fgetc(result), i = 0; c != EOF; c = fgetc(result), i++) {
    if (i >= init) {
      text = realloc(text, init <<= 1);
      
      if (!text) {
        FATAL("Out of memory!");
      }
    }

    text[i] = c;
  }

  pclose(result);

  return text;
}

void *runargs(const char *command, int argc, const char *argv[argc])
{
  char *buffer = _command_buffer(command, argc, argv);
  char *result = run(buffer);

  free(buffer);

  return result;
}

#ifdef WIN

#else

void newdir(const char *dirname)
{
  sysargs("mkdir", 2, (const char*[]){"-p", dirname});
}

void deldir(const char *dirname)
{
  sysargs("rmdir", 2, (const char*[]){"-p", dirname});
}

void newfile(const char *filename)
{
  sysargs("touch", 1, (const char*[]){filename});
}

void delfile(const char *filename)
{
  sysargs("rm", 1, (const char*[]){filename});
}

long lastmod(const char *filename)
{
  char *result = runargs("state", 3, (const char *[]){"-c", "%%Y", filename});
  long  value  = atol(result);

  free(result);

  return value;
}

void workdir(int size, char buffer[size]) {
  char *result = run("pwd");

  if (strlen(result) > size) {
    FATAL("Buffer overflow!");
  } else {
    strcpy(buffer, result);
  }

  free(result);
}

#endif