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

int findHeader(char* headerString, char* columnName){
  char temp[strlen(headerString) + 1]; //For storing the current column value
  int numQuotes = 0;
  int currentIndex = 0;
  char* token = strtok(headerString, ",");
  while (token != NULL){
    strcat(temp, token);
    numQuotes += numChars(token, '"');
    if (numQuotes % 2 == 1){ //Check if comma is part of string
      strcat(col, ",");
    } else if (strcmp(temp, columnName) == 0) { //If not, check the name
      return currentIndex;
    } else {
      temp[0] = '\0'; //Set string to empty
      currentIndex++;
    }
    token = strtok(NULL, ",");
  }
  return -1;
}

int populateListing(int index, char* line, Listing* listing){
  char temp[strlen(line) + 1];
  strcpy(temp, line);
  listing->row = temp;
  
  char col[strlen(line) + 1] = "";
  int numQuotes = 0;
  int currentIndex = 0;
  char* token = strtok(line, ",");
  while (token != NULL){
    if (currentIndex = index){
      strcat(col, token);
    }
    numQuotes += numChars(token, '"');
    if (numQuotes % 2 == 1){
      strcat(col, ",");
    } else {
      currentIndex++;
    }
    token = strtok(NULL, ",");
  }
  col = realloc(col, (strlen(col) + 1) * sizeof(char));
  if (!col) return -1;
  listing->COI = col;

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

int numChars(char* string, char character){
  if (!string){
    return 0;
  }
  int count = 0, i;
  for (i = 0; i < strlen(string); i++){
    if (string[i] == character) count++;
  }
  return count;
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
