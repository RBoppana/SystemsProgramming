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

int serve(char* name, int option, float amount){
	Node* ptr = Bank;
	while(ptr){
		if(strcmp(ptr->accn->accName, name) == 0){
			Account* currentAccn = ptr->accn;
			if(currentAccn->inSessionFlag == 1){
				return -2; //account in service
			}
			currentAccn->inSessionFlag = 1;
			switch(option){
				case 1: //deposit
					currentAccn->balance += amount;
					return 1;
				case 2: //withdraw
					if((currentAccn->balance - amount) < 0) return -3; //negative balance
					currentAccn->balance -= amount;
					return 1;
				case 3: //query
					return currentAccn->balance;
				case 4: //end session
					currentAccn->inSessionFlag = 0;
					return 1; //successfully ended
			}
			break;
		}
	}
	return -1; //account doesnt exist
}

int printBankAccnsList(){
	//mutex bank data
	Node* ptr = Bank;
	while(ptr){
		printf("%s\t%f\t", ptr->accn->accName, ptr->accn->balance);
		if(ptr->accn->inSessionFlag == 1) printf("IN SERVICE");
		prinf("\n");
		ptr = ptr->next;
	}
	free(ptr);
	//unlock mutex
}

int main(int argc, char** argv){

	return 0;
}
