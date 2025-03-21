#include <log.h>
#include <directory.h>

// TODO: remove . and .. from paths in clean
// TODO: to abs and to rel path methods
// TODO: update all other packages with these changes (especially except)
// TODO: make debug logs accept parameters

int main(void)
{
  const char *path = "file:///This/Is/A/Test/Path.txt";

  char buffer[4096];

  fext(path, sizeof(buffer), buffer);
  printf("%s\n", buffer);

  fname(path, sizeof(buffer), buffer);
  printf("%s\n", buffer);

  fnamext(path, sizeof(buffer), buffer);
  printf("%s\n", buffer);

  fpath(path, sizeof(buffer), buffer);
  printf("%s\n", buffer);

  CHECK_MEMORY

  DEBUG("This is a debug log!");
  INFO("This is an info log!");
  WARN("This is a warn log!");
  ERROR("This is an error log!");

  CHECK_MEMORY

  printf("Test path is absolute: %s\n", pisabs(path) ? "true" : "false");

  pprotocol(path, sizeof(buffer), buffer);

  printf("Protocol: %s\n", buffer);

  pclean(path, sizeof(buffer), buffer);

  printf("Cleaned path: %s\n", buffer);

  pcombine("test/path/", "/rest/of/path.txt", sizeof(buffer), buffer);

  printf("Combined path: %s\n", buffer);

  CHECK_MEMORY

  for (DirectoryIterator *di = dopen("."); !ddone(di); dnext(di))
  {
    dname(di, sizeof(buffer), buffer);

    printf("%s\n", buffer);
  }

  STOP_WATCHING
}