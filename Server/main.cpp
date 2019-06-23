#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cstring>

#include "read.h"
#include "recognize.h"
#include "red_black_tree.h"
#include "global_list.h"
#include "group_group.h"

#include "network.h"
#define PORT 1234
#define LEN 256

int main (int argc, char **argv)
{
  const char *file_name = "a.txt";
  int result;
  double counter_of_time;
  global_list *head;
  tree_name *root_name;
  tree_phone *root_phone;
  group_group hash_group[MAX_HASH];

  if (argc != 1)
    {
      fprintf (stdout, "Usage: %s\n", argv[0]);
      return -1000;
    }

  //Reading structures
  head = new global_list ();
  root_name = new tree_name ();
  root_phone = new tree_phone ();

  counter_of_time = clock();
  fprintf (stdout, "Creating structures...\n");
  result = read (file_name, head, root_name, root_phone, hash_group);
  fprintf (stdout, "\n");
  if (result < 0)
    {
      return_error (result, file_name);
      delete head;
      delete root_phone;
      delete root_name;
      return 1;
    }
  print_list (head, result, &stdout);
  fprintf (stdout, "TREE [NAME]:\n");
  print_tree (root_name->get_root (), 0, 0, &stdout);
  fprintf (stdout, "TREE [PHONE]:\n");
  print_tree (root_phone->get_root (), 0, 0, &stdout);
  fprintf (stdout, "HASH TABLE [GROUP]:\n");
  print_hash_table (hash_group, &stdout);
  counter_of_time = (clock () - counter_of_time) / CLOCKS_PER_SEC;
  fprintf (stdout, "Elapsed = %.3f\n\n", counter_of_time);
  //End reading


  //Network
  int sock, opt = 1, idx = 0;
  struct sockaddr_in addr;

  sock = socket (PF_INET, SOCK_STREAM, 0);
  if (sock < 0)
    {
      fprintf (stdout, "Cannot open socket!\n");
      return -1;
    }

  setsockopt (sock, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof (opt));
  addr.sin_family = AF_INET;
  addr.sin_port = htons (PORT);
  addr.sin_addr.s_addr = htonl (INADDR_ANY);

  if (bind (sock, (struct sockaddr *) &addr, sizeof (addr)) < 0)
    {
      fprintf (stdout, "Cannot bind socket!\n");
      return -2;
    }
  if (listen (sock, 3) < 0)
    {
      fprintf (stdout, "Cannot listen socket!\n");
      return -3;
    }

  fd_set active_set, read_set;
  FD_ZERO (&active_set);
  FD_SET (sock, &active_set);

  for (;;)
    {
      read_set = active_set;
      if (select (FD_SETSIZE, &read_set, 0, 0, 0) < 0)
        {
          fprintf (stdout, "Cannot select from set!\n");
          return -11;
        }
      for (idx = 0; idx < FD_SETSIZE; idx++)
        {
          if (FD_ISSET (idx, &read_set))
            {
              if (idx == sock)
                {
                  //new ask
                  struct sockaddr_in client;
                  int new_sock;
                  socklen_t size;
                  new_sock = accept (sock, (struct sockaddr *) &client, &size);
                  if (new_sock < 0)
                    {
                      fprintf (stdout, "Cannot accept!\n");
                      return -12;
                    }
                  FD_SET (new_sock, &active_set);
                }
              else
                {
                  int answer = read_command (head,
                                             root_name,
                                             root_phone,
                                             hash_group,
                                             &result,

                                             idx,
                                             active_set);

                  if (answer < 0)
                    {
                      close (sock);

                      fprintf (stdout, "Deleting structures...\n");
                      counter_of_time = clock ();
                      delete head;
                      delete root_phone;
                      delete root_name;
                      counter_of_time = (clock () - counter_of_time) / CLOCKS_PER_SEC;
                      fprintf (stdout, "Structures has been deleted for %.3f\n", counter_of_time);
                      return 0;
                    }
                }
            }
        }
      idx = FD_SETSIZE;
    }

  //End network

  return 0;
}
