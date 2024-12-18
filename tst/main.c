#include <directory.h>

int main(void)
{
  const char *path = "This/Is/A/Test/Path.txt";

  char buffer[4096];

  fileext(path, sizeof(buffer), buffer);
  printf("%s\n", buffer);

  filenamewoext(path, sizeof(buffer), buffer);
  printf("%s\n", buffer);

  filenamewopath(path, sizeof(buffer), buffer);
  printf("%s\n", buffer);

  filepath(path, sizeof(buffer), buffer);
  printf("%s\n", buffer);

  CHECK_MEMORY

  for (DirectoryIterator *di = dopen("."); di; dnext(&di))
  {
    printf("%s\n", di->current.name);
  }

  STOP_WATCHING
}