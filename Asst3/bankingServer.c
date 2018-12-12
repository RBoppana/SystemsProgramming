#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

//Automated Threading Memechine

#define INTERVAL 10

typedef struct Account{
  char* accName;
  float balance;
  int inSessionFlag;
} Account;

typedef struct Node{
  Account* accn;
  struct Node* next;
} Node;

Node* Bank;
Account* currentAccount;
int serviceID;
int seconds = 0;
int keepRunning = 1;
pthread_mutex_t accnMutex;

int createAccount(char* name){
	pthread_mutex_lock(&accnMutex);
	Node* ptr = Bank;
	while(ptr){
		if(strcmp(name, ptr->accn->accName) == 0) return -2; //duplicate account
		ptr = ptr->next;
	}
	free(ptr);

	Account* newAccount = (Account*)malloc(sizeof(Account));
	if(!newAccount) return -1; //out of memory - couldnt create
	newAccount->accName = name;
	newAccount->balance = 0.0;
	newAccount->inSessionFlag = 0;

	Node* newNode = (Node*)malloc(sizeof(Node));
	if(!newNode){
		free(newAccount);
		return -1; //out of memory - couldnt create
	}
  newNode->accn = newAccount;
	newNode->next = Bank;
	Bank = newNode;
	pthread_mutex_unlock(&accnMutex);
	return 1; //success
}

int serve(char* name, int option, float amount, int inputServiceID){
  Node* ptr = Bank;
  while(ptr){
    if(strcmp(ptr->accn->accName, name) == 0){
      currentAccount = ptr->accn;
      if(currentAccount->inSessionFlag == 0){
        //mutex
        currentAccount->inSessionFlag = inputServiceID; //account wasn't being serviced
        //unlock
      }
      if(currentAccount->inSessionFlag != inputServiceID){
        return -2; //account in service by another account
      }
      switch(option){
        case 1: //deposit
          currentAccount->balance += amount;
          return 1;
        case 2: //withdraw
          if((currentAccount->balance - amount) < 0) return -3; //negative balance
          currentAccount->balance -= amount;
          return 1;
        case 3: //query
          return currentAccount->balance;
        case 4: //end session
          currentAccount->inSessionFlag = 0;
          return 1; //successfully ended
        default:
          return 0; //idk
      }
      break;
    }
    ptr = ptr->next;
  }
  return -1; //account doesnt exist
}

int printBankAccnsList(){
  pthread_mutex_lock(&accnMutex);
  Node* ptr = Bank;
  if(!ptr) printf("No account data yet.\n");
  while(ptr){
    printf("%s\t%f\t", ptr->accn->accName, ptr->accn->balance);
    if(ptr->accn->inSessionFlag > 0) printf("IN SERVICE");
    printf("\n");
    ptr = ptr->next;
  }
  free(ptr);
  pthread_mutex_unlock(&accnMutex);
  return 1;
}

void* clientCommandWrapper(void* arg){
  pthread_detach(pthread_self());
  int socketfd = *(int*)arg;
  //int inService = 0;
  char message[264];
  char response[1000];

  while(1){
    if (read(socketfd, message, 264) == 0) break;

    scanf("%10[^\n]s\n%256s");

    fprintf(stdout, "%s\n", message);
    fflush(stdout);

    snprintf(response, sizeof(response), "If you're reading this, you have succeeded!");
    write(socketfd, response, strlen(response) + 1);
  }

  return NULL;
}

void timer(int i){
  struct itimerval store;
  signal(SIGALRM, timer);
  seconds += INTERVAL;
  printBankAccnsList();
  //printf("3 more secs");
  fflush(stdout);
  store.it_interval.tv_sec = 0;
  store.it_interval.tv_usec = 0;
  store.it_value.tv_sec = INTERVAL;
  store.it_value.tv_usec = 0;
  setitimer(ITIMER_REAL, &store,0);
}

void quit(int i){
	keepRunning = 0;
  printf("Server Shutting down!!\n");
  while(Bank){
    Node* temp = Bank;
    Bank = Bank->next;
    free(temp->accn->accName);
    free(temp->accn);
    free(temp);
  }
  exit(0);
}

int main(int argc, char** argv){
  if (argc != 2){
    fprintf(stderr, "Please specify the port of the server.\n");
    return -1;
  }

  serviceID = 1;

  //Socket setup
  int port = atoi(argv[1]);
  if (port <= 0){
    fprintf(stderr, "Invalid port number.\n");
    return -1;
  }
  int socketfd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in server, client;
  if (socketfd < 0){
    fprintf(stderr, "Error opening socket.\n");
    return -1;
  }
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  server.sin_addr.s_addr = INADDR_ANY;
  if (bind(socketfd, (struct sockaddr*) &server, sizeof(server)) < 0){
    fprintf(stderr, "Error binding socket.\n");
    return -1;
  }
  listen(socketfd, 5);
  fprintf(stdout, "Server started.\n");

  //Setup 15 second loop and ctrl c interrupt

  struct itimerval store;
  store.it_interval.tv_sec = 0;
  store.it_interval.tv_usec = 0;
  store.it_value.tv_sec = INTERVAL;
  store.it_value.tv_usec = 0;
  setitimer(ITIMER_REAL, &store, 0);

  signal(SIGALRM, timer);
  signal(SIGINT, quit);

  //Handle new client connections
  while (keepRunning){
    socklen_t clientLen = sizeof(client);
    int newfd = accept(socketfd, (struct sockaddr*) &client, &clientLen);
    if (newfd < 0){
      fprintf(stderr, "Error accepting client connection.\n");
      continue;
    }
    pthread_t thread;
    if (pthread_create(&thread, NULL, clientCommandWrapper, &newfd) != 0){
      fprintf(stderr, "Error creating client thread.\n");
      close(newfd);
      continue;
    }
    fprintf(stderr, "Accepted connection from %s.\n", inet_ntoa(client.sin_addr));
  }

  //disconnect and close all threads etc

  return 0;
}
