#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>

pthread_t inputThread, outputThread;
int inService = 0;
pthread_mutex_t serviceMutex;

void* userPrompt(void* arg){
  int socketfd = *(int*)arg;
  char input[267]; //Max length of input string
  char message[267];
  char command[11], argument[257];

  while(1){
    fprintf(stdout, "\nEnter a command: ");
    fgets(input, 266, stdin);
    if(!strchr(input, '\n')){ //if newline does not exist
      while(fgetc(stdin) != '\n'); //discard until newline
    }
    command[0] = '\0';
    argument[0] = '\0';
    sscanf(input, "%10s %256[^\n]s", command, argument);
    //fprintf(stdout, "command: %s, argument: %s\n", command, argument);

    int tempService;
    pthread_mutex_lock(&serviceMutex);
    tempService = inService;
    pthread_mutex_unlock(&serviceMutex);

    if(strcmp(command, "create") == 0 || strcmp(command, "serve") == 0){
      if (tempService == 1){
        fprintf(stdout, "Please end the current session first.\n");
        continue;
      }
      if (strlen(argument) == 0){
        fprintf(stdout, "Please provide an account name.\n");
        continue;
      }
      snprintf(message, sizeof(message), "%s\n%s", command, argument);
      write(socketfd, message, strlen(message) + 1);
    }
    else if (strcmp(command, "end") == 0){
      if (tempService == 0){
        fprintf(stdout, "No account is currently being served.\n");
        continue;
      }
      pthread_mutex_lock(&serviceMutex);
      inService = 0;
      pthread_mutex_unlock(&serviceMutex);
      write(socketfd, "end", 4);
    }
    else if (strcmp(command, "deposit") == 0 || strcmp(command, "withdraw") == 0){
      if (tempService == 0){
        fprintf(stdout, "No account is currently being served.\n");
        continue;
      }
      errno = 0;
      double amount = strtod(argument, NULL);
      if (errno != 0 || amount <= 0){
        fprintf(stdout, "Please provide a positive value.\n");
        continue;
      }
      snprintf(message, sizeof(message), "%s\n%lf", command, amount);
      write(socketfd, message, strlen(message) + 1);
    }
    else if (strcmp(command, "query") == 0){
      if (tempService == 0){
        fprintf(stdout, "No account is currently being served.\n");
        continue;
      }
      write(socketfd, "query", 6);
    }
    else if (strcmp(command, "quit") == 0){
      if (tempService == 1){
        fprintf(stdout, "Please end the current session first.\n");
        continue;
      }
      fprintf(stdout, "Disconnecting from the server...");
      close(socketfd);
      pthread_cancel(outputThread);
      break;
    } 
    else {
      fprintf(stdout, "Not a valid command.\n");
      continue;
    }
    sleep(2);
  }

  return NULL;
}

void* serverResponse(void* arg){
  int socketfd = *(int*)arg;
  char response[267];
  char command[11], argument[257];

  while(1){
    if (read(socketfd, response, 267) == 0){
      fprintf(stdout, "Server disconnected.\n");
      close(socketfd);
      pthread_cancel(inputThread);
      break;
    }
    
    command[0] = '\0';
    argument[0] = '\0';
    sscanf(response, "%10s\n%256s", command, argument);
    if (strcmp(command, "create") == 0){
      if (strcmp(argument, "1") == 0){
        fprintf(stdout, "Account created successfully.\n");
      } else if (strcmp(argument, "-1") == 0){
        fprintf(stdout, "Unable to create account.\n");
      } else if (strcmp(argument, "-2") == 0){
        fprintf(stdout, "Account name already exists.\n");
      } else {
        fprintf(stderr, "Error parsing server message.\n");
      }
    } 
    else if(strcmp(command, "serve") == 0){
      if (strcmp(argument, "1") == 0){
        pthread_mutex_lock(&serviceMutex);
        inService = 1;
        pthread_mutex_unlock(&serviceMutex);
        fprintf(stdout, "Successfully serving.\n");
      }
      if (strcmp(argument, "-1") == 0){
        fprintf(stdout, "Account does not exist.\n");
      } else if(strcmp(argument, "-2") == 0){
        fprintf(stdout, "Account is already in service. Please try again later.\n");
      } else {
        fprintf(stderr, "Error parsing server message.\n");
      }
    } 
    else if (strcmp(command, "end") == 0){
      if (strcmp(argument, "1")){
        fprintf(stdout, "Service session ended.\n");
      } else {
        fprintf(stderr, "Error parsing server message.\n");
      }
    }
    else if (strcmp(command, "deposit") == 0){
      if (strcmp(argument, "1")){
        fprintf(stdout, "Successfully deposited.\n");
      } else {
        fprintf(stderr, "Error parsing server message.\n");
      }
    } 
    else if (strcmp(command, "withdraw") == 0){
      if (strcmp(argument, "1")){
        fprintf(stdout, "Successfully withdrawn.\n");
      } else if (strcmp(argument, "-3")){
        fprintf(stdout, "Insufficient funds.\n");
      } else {
        fprintf(stderr, "Error parsing server message.\n");
      }
    }
    else if (strcmp(command, "query") == 0){
      errno = 0;
      double amount = strtod(argument, NULL);
      if (errno == 0){
        fprintf(stdout, "Current account balance: $%.2lf\n", amount);
      } else {
        fprintf(stderr, "Error parsing server message.\n");
      }
    }
    else {
      fprintf(stderr, "Error parsing server message.\n");
    }
  }

  return NULL;
}

int main(int argc, char** argv){
  if (argc != 3){
    fprintf(stderr, "Please specify the hostname and port of the server.\n");
    return -1;
  }

  //Socket setup
  int port = strtol(argv[2], NULL, 10);
  if (port <= 8192){
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
