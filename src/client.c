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
        Wait
*/

#include <sys/wait.h>

/*
        Define
*/

#define SERV_HOST_ADDR "127.0.0.1"

char buf_tx[100];
char buf_rx[100];

int main(int argc, char *argv[]) {

  // port
  int port = strtol(argv[1], NULL, 10);
  printf("Port %i\n", port);

  int sockfd, len_rx;
  struct sockaddr_in servaddr;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  memset(&servaddr, 0, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
  servaddr.sin_port = htons(port);

  if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
    perror("[CLIENT] connection failed");
    return -1;
  }
  puts("[CLIENT] connection establish");

  while (1) {

    memset(buf_tx, 0, 100);

    if (fgets(buf_tx, 100, stdin) == NULL) {
      close(sockfd);
      return 0;
    }

    write(sockfd, buf_tx, sizeof(buf_tx));
    len_rx = read(sockfd, buf_rx, sizeof(buf_rx));
    printf("%s\n", buf_rx);

//     if (len_rx == -1) {
//       perror("[CLIENT] Error");
//       return -1;
//     } else if (len_rx == 0) {
//       puts("[CLIENT] No Server");
//       close(sockfd);
//       return -1;
//     }

    int i;
    for (i = 0; i < 100; i++) {
      wait(NULL);
    }
  }

  // read(sockfd, buf_rx, sizeof(buf_rx));
  // printf("Client received %s\n", buf_rx);

  close(sockfd);
  return 0;
}
