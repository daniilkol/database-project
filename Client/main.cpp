#include "network.h"

int main()
{
  int sock;
  struct sockaddr_in server_addr;
  struct hostent *hostinfo;
  char buf[BUF_LEN];
  
  hostinfo = gethostbyname (SERVER_NAME);
  if (!hostinfo)
    {
      fprintf (stdout, "Cannot get host\n");
      return 1;
    }
  
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons (SERVER_PORT);
  server_addr.sin_addr = *(struct in_addr *) hostinfo->h_addr;
  
  sock = socket (PF_INET, SOCK_STREAM, 0);
  if (sock < 0)
    {
      fprintf (stdout, "Cannot open socket\n");
      return -1;
    }
  
  if (connect (sock, (struct sockaddr *) &server_addr, sizeof (server_addr)) < 0)
    {
      fprintf (stdout, "Cannot connect\n");
      return -2;
    }

  while (fgets (buf, BUF_LEN, stdin))
    {
      for (int idx = 0; buf[idx]; idx++)
        {
          if (buf[idx] == '\n')
            {
              buf[idx] = 0;
              break;
            }
        }
      if (!strcmp (buf, "quit;") || !strcmp (buf, "stop;"))
        {
          write_fd (sock, buf);
          read_fd  (sock, buf);
          close (sock);
          return 0;
        }
      write_fd (sock, buf);
      while (1)
        {
          read_fd (sock, buf);
          if (!strcmp (buf, "done"))
            break;
          printf ("%s\n", buf);
        }
    }
  close (sock);
  return 1;
}
