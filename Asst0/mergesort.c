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

int readLine(size_t index, Listing* listing){
  int rowStringCapacity = 100;
  int columnValueCapacity = 100;
  char* rowString = (char*) malloc(rowStringCapacity * sizeof(char));
  char* columnValue = (char*) malloc(columnValueCapacity * sizeof(char));
  if (!rowString || !columnValue) return NULL;
  size_t rowStringLength = 0;
  size_t columnValueLength = 0;
  char current;
  int currentIndex = 0;

  read(0, &current, 1);
  while (current != EOF && current != '\n'){
    if (current == ','){
      currentIndex++; 
    }

    rowString[rowStringLength] = current;
    rowStringLength++;
    if (rowStringLength == rowStringCapacity){
      rowStringCapacity *= 2;
      rowString = realloc(rowString, rowStringCapacity * sizeof(char));
      if (!rowString) return -1;
    }

    if (currentIndex == index && current != ','){
      columnValue[columnValueLength] = current;
      columnValueLength++;
      if (columnValueLength == columnValueCapacity){
	columnValueCapacity *= 2;
	columnValue = realloc(columnValue, columnValueCapacity * sizeof(char));
	if (!columnValue) return -1;
      }
    }

    read(0, &current, 1);
  }

  rowString[rowStringLength] = '\0';
  columnValue[columnValueLength] = '\0';
  rowStringLength++;
  columnValueLength++;
  
  rowString = realloc(rowString, rowStringLength * sizeof(char));
  columnValue = realloc(columnVaue, columnValueLength * sizeof(char));
  if (!rowString || !columnValue) return -1;

  listing->row = rowString;
  listing->COI = columnValue;

  return 0;
}

char* readHeader(){
  size_t capacity = 100;
  char* string = (char*) malloc(capacity * sizeof(char));
  if (!string) return string;
  size_t length = 0;
  char current;
  read(0, &current, 1);
  while (current != EOF && current != '\n'){
    string[length] = current;
    length++;
    if (length == size){
      size *= 2;
      string = realloc(string, size * sizeof(char));
      if (!string) return string;
    }
    read(0, &current, 1);
  }
  string[length] = '\0';
  length++;
  return realloc(string, length * sizeof(char));
}

 mergeSort(Value** input, int** inputTypes, int compareColumn, int size){
  if(size = 1){
    return input;
  }

  
}
