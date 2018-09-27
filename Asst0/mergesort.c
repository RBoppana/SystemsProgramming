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

int populateListing(int index, char* line, Listing* listing){
  
  return 0;
}

char* readLine(int fd){
  size_t capacity = 100;
  char* string = (char*) malloc(capacity * sizeof(char));
  if (!string) return string;
  size_t length = 0;
  char current;
  read(fd, &current, 1);
  while (current != EOF && current != '\n'){
    string[length] = current;
    length++;
    if (length == size){
      size *= 2;
      string = realloc(string, size * sizeof(char));
      if (!string) return string;
    }
    read(fd, &current, 1);
  }
  string[length] = '\0';
  length++;
  return realloc(string, length * sizeof(char));
}

int* merge(int* inputArray1, int inputArray2){

}

int* mergeSort(Listing* reference, int* indexes, int size){
  if(size = 1){
    return indexes;
  }
  
}

void printData(int fd, Listing* data, int size){
  
}
