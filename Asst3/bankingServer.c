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

int main(int argc, char** argv){

	serviceID = 1;
	//everytime any client asks for a serviceID, send mutexed serviceID++ to give a new unique value

	return 0;
}
