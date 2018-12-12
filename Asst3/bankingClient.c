#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h> 

pthread_t inputThread, outputThread;

void* userPrompt(void* arg){
  int socketfd = *(int*)arg;
  int inService = 0;
  char input[264]; //Max length of input string ("create " + 256 characters)
  char message[264];
  char command[11], argument[257];

  while(1){
    fprintf(stdout, "\nEnter a command: ");
    fgets(input, 264, stdin);
    if(!strchr(input, '\n')){ //if newline does not exist
      while(fgetc(stdin) != '\n'); //discard until newline
    }
    command[0] = '\0';
    argument[0] = '\0';
    sscanf(input, "%10s %256[^\n]s", command, argument);
    fprintf(stdout, "command: %s, argument: %s\n", command, argument);

    if(strcmp(command, "create") == 0){
      if (inService == 1){
        fprintf(stdout, "Please end the current session before creating a new account.\n");
        continue;
      }
      if (strlen(argument) == 0){
	fprintf(stdout, "Please provide an account name.\n");
	continue;
      }
      snprintf(message, sizeof(message), "create\n%s", argument);
      write(socketfd, message, strlen(message) + 1);
    }/*else if(strcmp(command, "serve") == 0){      
      if (inService == 1){
        fprintf(stdout, "End the current session before serving a different account.")
      }
      char string[6 + strlen(argument) + 1];
      snprintf(string, sizeof(string), "serve\n%s", argument);
      write(socketfd, string, strlen(string));

      if(serviceAcceptance == -2){
        fprintf(stdout, "Account already in service!\n");
      }else if(serviceAcceptance == -1){
        fprintf(stdout, "Account does not exist.\n")
      }else{
        int serviceID = //get unique serviceID from server -- server always sends new incremented value
        scanf("%s", command);
        while(strcmp(command, "end") != 0){
          if(strcmp(command, "deposit") == 0){
            float amount;
            scanf("%f", amount);
            //send data to server -- serve(name, 1, amount, serviceID) 
          }else if(strcmp(command, "withdraw") == 0){
            float amount;
            scanf("%f", amount);
            //send data to server -- serve(name, 2, amount, serviceID) 
          }else if(strcmp(command, "query") == 0){
            //send query to server -- serve(name, 3, -1, serviceID)
          }else if(strcmp(command, "quit") == 0){
            //send quit to server and do all quit stuff
          }else if(strcmp(command, "create") == 0){
            fprintf(stdout, "Please exist the service session to create an account.\n");
          }else{
            fprintf(stdout, "Please enter a valid command, you are currently in service of account: %s.\n", name);
          }
        }
        //send end to server -- serve(name, 4, -1, serviceID)
      }      
    }else if(strcmp(command, "quit") == 0){
      //send quit to server and do all quit stuff
    }*/else{
      fprintf(stdout, "Not a valid command.\n");
      continue;
    }
    sleep(2);
  }

  return NULL;
}

void* serverResponse(void* arg){
  int socketfd = *(int*)arg;
  char response[1000];

  while(1){
    read(socketfd, response, 1000);
    fprintf(stdout, "%s\n", response);
  }

  return NULL;
}

int main(int argc, char** argv){
  if (argc != 3){
    fprintf(stderr, "Please specify the hostname and port of the server.\n");
    return -1;
  }

  //Socket setup
  int port = atoi(argv[2]);
  if (port <= 0){
    fprintf(stderr, "Invalid port number.\n");
    return -1;
  }
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
  while (1){
    fprintf(stdout, "Connecting to %s on port %d...\n", argv[1], port);
    fflush(stdout);
    if (connect(socketfd, (struct sockaddr*) &server, sizeof(server)) < 0){
      fprintf(stdout, "Unable to connect to server. Retrying in 3 seconds.\n");
      sleep(3);
      continue;
    } else {
      fprintf(stdout, "Successfully connected.\n");
      break;
    }
  }

  if (pthread_create(&inputThread, NULL, userPrompt, &socketfd) != 0){
      fprintf(stderr, "Error creating input thread.\n");
      return -1;
  }
  if (pthread_create(&outputThread, NULL, serverResponse, &socketfd) != 0){
      fprintf(stderr, "Error creating output thread.\n");
      pthread_cancel(inputThread);
      pthread_join(inputThread, NULL);
      return -1;
  }

  pthread_join(inputThread, NULL);
  pthread_join(outputThread, NULL);
  fprintf(stdout, "Client session complete.\n");

  return 0;
}
