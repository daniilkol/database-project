#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int read_fd  (int fd, char *buf);
int write_fd (int fd, const char *buf);

void itoa (int n, char s[]);

#endif
