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

//Automated Threading Memechine

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
Node* currentAccount;
int serviceID;

int createAccount(char* name){
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
  newNode->next = Bank;
  Bank = newNode;
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
  }
  return -1; //account doesnt exist
}

int quit(){

}

int printBankAccnsList(){
  //mutex bank list
  Node* ptr = Bank;
  while(ptr){
    printf("%s\t%f\t", ptr->accn->accName, ptr->accn->balance);
    if(ptr->accn->inSessionFlag > 0) printf("IN SERVICE");
    printf("\n");
    ptr = ptr->next;
  }
  free(ptr);
  //unlock mutex
}

void* clientCommandWrapper(void* arg){
  fprintf(stdout, "Connected.");
}

int main(int argc, char** argv){
  if (argc != 2){
    fprintf(stderr, "Please specify the port of the server.\n");
    return -1;
  }

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
  server.sin_port = htons(portno);
  server.sin_addr.s_addr = INADDR_ANY;
  if (bind(socketfd, (struct sockaddr*) &server, sizeof(server)) < 0){
    fprintf(stderr, "Error binding socket.\n");
  }
  listen(socketfd, 5);

  //Handle new client connections
  while (1){
    int clientLen = sizeof(client);
    int newfd = accept(socketfd, (struct sockaddr*) &client, &clientLen);
    if (newfd < 0){
      fprintf(stderr, "Error accepting client connection.\n");
      continue;
    }
    pthread_t thread;
    if (pthread_detach(thread) != 0){
      fprintf(stderr, "Error detaching.\n");
      continue;
    }
    if (pthread_create(&thread, NULL, clientCommandWrapper, &newfd) != 0){
      fprintf(stderr, "Error creating client thread.\n");
      continue;
    }
  }
  

  //when client connects, launch a thread with clientCommandWrapper: takes in an option and runs either create or serve or quit as shown above, and passes in same arguements

  serviceID = 1;
  //everytime any client asks for a serviceID, send mutexed serviceID++ to give a new unique value



  //need session acceptor thread and method

  //need to figure out 15 sec printing w/ sigint, server shutdown w/ ctrl c

  //need to put in mutexes for above methods when relevant

  return 0;
}
