#include <directory.h>

int main(void)
{

  CHECK_MEMORY

  for (DirectoryIterator *di = dopen("."); di; dnext(&di))
  {
    printf("%s\n", di->current.name);
  }

  STOP_WATCHING
}