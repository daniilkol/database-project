#include <unistd.h>
#include "network.h"

int read_fd (int fd, char *buf)
{
  int len = strlen (buf) + 1;
  int bytes = 0, idx = 0;
  
  if (read (fd, &len, sizeof(int)) != (int) sizeof (int))
  {
    return -1;
  }
  
  for (idx = 0; len > 0; idx += bytes, len -= bytes)
  {
    bytes = read (fd, buf + idx, len);

    if (bytes <= 0)
      {
        return -2;
      }
  }

  return idx;
}

int write_fd (int fd, const char *buf)
{
  int len = strlen (buf) + 1;
  int bytes, idx = 0;

  if (write (fd, &len, sizeof(int)) != (int) sizeof (int))
  {
    return -1;
  }

  for (idx = 0; len > 0; idx += bytes, len -= bytes)
  {
    bytes = write (fd, buf + idx, len);

    if (bytes <= 0)
      {
        return -2;
      }
  }

  return idx;
}

void itoa (int n, char s[])
{
  char end = '\0';
  sprintf (s, "%d%c", n, end);
}
