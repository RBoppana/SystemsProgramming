#include <stdio.h>
#include <stdlib.h>
#include "simpleCSVsorter.h"

void insertNode(Listing* input){
	Node* newNode = (Node*) malloc(sizeof(Node));
	newNode->element = input;
	newNode->next = front;
	front = newNode;
	return;
}

char* removeWhitespace(char* string){
}
