#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "bankingServer.h"
#include <signal.h>
#include <sys/time.h>

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
int seconds = 0;

int createAccount(char* name){
	//mutex
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
	//unlock mutex
	return 1; //success
}

int serve(char* name, int option, float amount, int inputServiceID){
	//mutex
	Node* ptr = Bank;
	while(ptr){
		if(strcmp(ptr->accn->accName, name) == 0){
			currentAccount = ptr->accn;
			if(currentAccount->inSessionFlag == 0){
				currentAccount->inSessionFlag = inputServiceID; //account wasn't being serviced
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
	//unlock mutex
	return -1; //account doesnt exist
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

int clientCommandWrapper(){

}

void timer(int i){
  struct itimerval store;
  signal(SIGALRM, timer);
  seconds += 15;
  printBankAccnsList();
  store.it_interval.tv_sec = 0;
  store.it_interval.tv_usec = 0;
  store.it_value.tv_sec = 15; 
  store.it_value.tv_usec = 0;
  setitimer(ITIMER_REAL, &store,0);
}

void quit(int i){
  signal(SIGINT, quit);
	//close all threads etc
  exit(0);
}

int main(int argc, char** argv){
	serviceID = 1;

	//put server bind listen port shit here

	struct itimerval store;
  store.it_interval.tv_sec = 0;
  store.it_interval.tv_usec = 0;
  store.it_value.tv_sec = 15; 
  store.it_value.tv_usec = 0;
  setitimer(ITIMER_REAL, &store,0);

  signal(SIGALRM, timer);
  signal(SIGINT, quit);

  while(1){
    //server takes connections/launches threads
  }

  return 0;
}

//everytime any client asks for a serviceID, send mutexed serviceID++ to give a new unique value

//when client connects, launch a thread with clientCommandWrapper: takes in an option and runs either create or serve or quit as shown above, and passes in same arguements

//need session acceptor thread and method

//need to figure out 15 sec printing w/ sigint, server shutdown w/ ctrl c

//need to put in mutexes for above methods when relevant