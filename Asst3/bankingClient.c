#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h> 

int main(int argc, char** argv){
  if (argc != 3){
    fprintf(stderr, "Please specify the hostname and port of the server.\n");
    return -1;
  }

  int port = atoi(argv[2]);
  if (port == 0){
    fprintf(stderr, "Invalid port number.\n");
    return -1;
  }

  //Socket setup
  int socketfd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in server;
  struct hostent *he;
  if (socketfd < 0){
    fprintf(stderr, "Error opening socket.\n");
    return -1;
  }
  he = gethostbyname(argv[1]);
  if (he == NULL) {
    fprintf(stderr,"Host not found.\n");
    exit(0);
  }
  server.sin_family = AF_INET;
  memcpy(&server.sin_addr, he->h_addr_list[0], he->h_length);
  server.sin_port = htons(port);

  //Connect to server
  while (serverfd < 0){
    fprintf(stdout, "Connecting to %s on port %d...\n", argv[1], port);
  }

  return 0;
}
