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

#define SERVER_NAME "127.0.0.1"
#define SERVER_PORT 1234
#define BUF_LEN 256

int read_fd (int fd, char *buf);
int write_fd (int fd, const char *buf);

#endif
