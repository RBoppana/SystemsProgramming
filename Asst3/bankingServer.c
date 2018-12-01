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

}

int printBankAccns(){

}

int main(int argc, char** argv){

	return 0;
}
