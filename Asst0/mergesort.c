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

char* readString(FILE* fp, size_t size, char stop){
  char* string = (char*) malloc(size * sizeof(char));
  size_t length = 0;
  char current;
  fscanf(fp, "%c", &current);
  while (current != EOF && current != stop){
    string[length] = current;
    length++;
    if (length == size){
      size *= 2;
      string = realloc(string, size * sizeof(char));
    }
    fscanf(fp, "%c", &current);
  }
  string[length] = '\0';
  length++;
  return realloc(string, length * sizeof(char));
}

Listing* mergeSort(Listing* input, int** inputTypes){
  
}