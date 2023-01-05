/*
        Standard symbols
*/

#include <unistd.h>

/*
        Sockets
*/
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

/*
        Strings
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
        Error
*/
#include <error.h>

/*
        Definitions
*/
#define SERV_HOST_ADDR "127.0.0.1"
#define BUF_SIZE 100
#define BACKLOG 5

int main(int argc, char *argv[]) {

  // port
  int port = strtol(argv[1], NULL, 10);
  printf("Port %i\n", port);

  int sockfd, connfd;
  unsigned int len;
  struct sockaddr_in servaddr, client;

  int len_rx, len_tx = 0;
  char buff_tx[BUF_SIZE] = "Server succes 👋";
  char buff_rx[BUF_SIZE];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd == -1) {
    perror("Could not create socket");
    return -1;
  }
  puts("Socket created");

  memset(&servaddr, 0, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
  servaddr.sin_port = htons(port);

  /*
          Bind socket
  */

  if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) {
    perror("Server Biding Error");
    return -1;
  }
  puts("Server bided");

  if ((listen(sockfd, BACKLOG)) != 0) {
    perror("Server listen failed");
    return -1;
  }
  puts("Server listening");

  len = sizeof(client);

  while (1) {
    connfd = accept(sockfd, (struct sockaddr *)&client, &len);
    if (connfd < 0) {
      perror("Server accept failed");
      return -1;
    } else {
      while (1) {
        len_rx = read(connfd, buff_rx, sizeof(buff_rx));
        if (len_rx == -1) {
          perror("Server read failed");
        } else if (len_rx == 0) {
          puts("Server client closed");
          close(connfd);
          break;
        } else {
          write(connfd, buff_tx, strlen(buff_tx));
          printf("Server %s", buff_rx);
        }
      }
    }
  }
  return 1;
}