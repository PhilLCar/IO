#include <directory.h>

int main(void)
{
  const char *path = "This/Is/A/Test/Path.txt";

  char buffer[4096];

  fileext(path, buffer, sizeof(buffer));
  printf("%s\n", buffer);

  filenamewoext(path, buffer, sizeof(buffer));
  printf("%s\n", buffer);

  filenamewopath(path, buffer, sizeof(buffer));
  printf("%s\n", buffer);

  filepath(path, buffer, sizeof(buffer));
  printf("%s\n", buffer);

  CHECK_MEMORY

  for (DirectoryIterator *di = dopen("."); di; dnext(&di))
  {
    printf("%s\n", di->current.name);
  }

  STOP_WATCHING
}