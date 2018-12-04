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

/*
int userPrompt(server port stuff){
  while(connected and every 2 seconds){
    printf("What can we help you with?\n");
    char command[10];
    scanf("%s", command);
    if(strcmp(command, "create") == 0){
      char name[256];
      scanf("%s", name);
      send create to server
    }else if(strcmp(command, "serve") == 0){
      int serviceID = get unique serviceID from server -- server always sends new incremented value
      char name[256];
      scanf("%s", name);
      send data to server -- serve(name, -1, -1, serviceID) 
      if(serviceAcceptance == -2){
        printf("Account already in service!\n");
      }else if(serviceAcceptance == -1){
        printf("Account does not exist.\n")
      }else{
        scanf("%s", command);
        while(strcmp(command, "end") != 0){
          if(strcmp(command, "deposit") == 0){
            float amount;
            scanf("%f", amount);
            send data to server -- serve(name, 1, amount, serviceID) 
          }else if(strcmp(command, "withdraw") == 0){
            float amount;
            scanf("%f", amount);
            send data to server -- serve(name, 2, amount, serviceID) 
          }else if(strcmp(command, "query") == 0){
            send query to server -- serve(name, 3, -1, serviceID)
          }else if(strcmp(command, "quit") == 0){
            send quit to server and do all quit stuff
          }else if(strcmp(command, "create") == 0){
            printf("Please exist the service session to create an account.\n");
          }else{
            printf("Please enter a valid command, you are currently in service of account: %s.\n", name);
          }
        }
        send end to server -- serve(name, 4, -1, serviceID)
      }      
    }else if(strcmp(command, "quit") == 0){
      send quit to server and do all quit stuff
    }else{
      printf("Please enter a valid command.");
    }
  }
}
*/

/*
int serverResponses(){

}
*/

int serviceAcceptance; //marked by server response when serve is called

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

  //Connection Confirmation Message

  //Launch User Prompt Thread  

  //Launch Server Response Thread

  return 0;
}
