#include <log.h>
#include <directory.h>
#include <path.h>

int main(void)
{
#ifdef WIN
  const char *path = "C:\\This\\Is\\A\\Test\\Path.txt";
#else
  const char *path = "file:///This/Is/A/Test/Path.txt";
#endif

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

  DEBUG("This is a %s log!", "debug");
  INFO ("This is a %s log!", "info");
  WARN ("This is a %s log!", "warn");
  ERROR("This is an error log!");

  CHECK_MEMORY

  printf("Test path is absolute: %s\n", pisabs(path) ? "true" : "false");

  pprotocol(path, sizeof(buffer), buffer);

#ifdef WIN
  printf("Drive: %s\n", buffer);
#else
  printf("Protocol: %s\n", buffer);
#endif

  pclean(path, sizeof(buffer), buffer);

  printf("Cleaned path: %s\n", buffer);

#ifdef WIN
  pcombine("test\\path\\", "\\rest\\of\\path.txt", sizeof(buffer), buffer);
#else
  pcombine("test/path/", "/rest/of/path.txt", sizeof(buffer), buffer);
#endif

  printf("Combined path: %s\n", buffer);

#ifdef WIN
  pabs("..\\.\\Collection\\wrong\\..\\right\\ok", sizeof(buffer), buffer);
#else
  pabs(".././Collection/wrong/../right/ok", sizeof(buffer), buffer);
#endif

  printf("To absolute: %s\n", buffer);

  prel(buffer, sizeof(buffer), buffer);

  printf("To relative: %s\n", buffer);

  CHECK_MEMORY

  for (DirectoryIterator *di = dopen("."); !ddone(di); dnext(di))
  {
    dname(di, sizeof(buffer), buffer);

    printf("%s\n", buffer);
  }

  STOP_WATCHING
}