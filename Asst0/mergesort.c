#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
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
  temp[0] = '\0';
  int numQuotes = 0;
  int currentIndex = 0;
  char* token;
  while ((token = strsep(&headerString, ","))){
    strcat(temp, token);
    numQuotes += numChars(token, '"');
    if (numQuotes % 2 == 1){ //Check if comma is part of string
      strcat(temp, ",");
    } else if (strcmp(temp, columnName) == 0) { //If not, check the name
      return currentIndex;
    } else {
      temp[0] = '\0'; //Set string to empty
      currentIndex++;
    }
  }
  return -1;
}

int populateListing(int index, char* line, Listing* listing){
  //Copy entire string to listing
  char* temp = (char*) malloc((strlen(line) + 1) * sizeof(char));
  if (!temp) return -1;
  strcpy(temp, line);
  listing->row = temp;
  
  //Get string value of column of interest
  char col[strlen(line) + 1];
  col[0] = '\0';
  int numQuotes = 0;
  int currentIndex = 0;
  char* token;
  while ((token = strsep(&line, ","))){
    if (currentIndex == index){
      strcat(col, token);
    }
    numQuotes += numChars(token, '"');
    if (numQuotes % 2 == 1){
      if (currentIndex == index){
	strcat(col, ",");
      }
    } else {
      currentIndex++;
    }
  }
  if (col[0] == '\0'){
    listing->COI = NULL;
  } else {
    char* new = (char*) malloc((strlen(col) + 1) * sizeof(char));
    if (!new) return -1;  
    strcpy(new, col);
    char* trimmed = removeWhitespace(new);
    listing->COI = trimmed;
  }
  
  return 0;
}

char* readLine(int fd){
  size_t capacity = 100;
  char* string = (char*) malloc(capacity * sizeof(char));
  if (!string) return string;
  size_t length = 0;
  char current;
  while (read(fd, &current, sizeof(char)) != 0 && current != '\n'){
    string[length] = current;
    length++;
    if (length == capacity){
      capacity *= 2;
      string = (char*) realloc(string, capacity * sizeof(char));
      if (!string) return string;
    }
  }
  if (length == 0 && current != '\n'){
    free(string);
    return NULL;
  } 
  string[length] = '\0';
  length++;
  return (char*) realloc(string, length * sizeof(char));
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

char* removeWhitespace(char* string){
  int start, end;
  int i;
  for (i = 0; i < strlen(string); i++){ //Preceding whitespace
    if (isspace(string[i])) continue;
    start = i;
    break;
    
  }
  for (i = strlen(string) - 1; i >= 0; i--){ //Proceding whitespace
    if (isspace(string[i])) continue;
    end = i;
    break;
  }
  char* result = (char*) malloc((end - start + 2)* sizeof(char));
  if (!result) return result;
  strcpy(result, &string[start]);
  result[end - start + 1] = '\0';
  free(string);
  return result;
}

int comparator(char* str1, char* str2) {
  if (!str1 && !str2){
    return 0;
  }
  if (!str1 || !str2){
    return !str1 ? -1 : 1;
  }
  if (columnType == 0) {
		double num1 = atol(str1);
		double num2 = atol(str2);
		if (num1 < num2) {
			return -1;
		}else if (num1 > num2) {
			return 1;
		}else {
			return 0;
		}
	}else {
		if (strcmp(str1, str2) < 0) {
			return -1;
		}
		else if (strcmp(str1, str2) > 0) {
			return 1;
		}
		else {
			return 0;
		}
	}
}

int* merge(int* input1, int size1, int* input2, int size2){
  AA = (int*) realloc(AA, (size1 + size2) * sizeof(int));
  int i = 0;
  int j = 0;

  while(i < size1 && j < size2){
	char* str1 = data[input1[i]]->COI;
	char* str2 = data[input2[j]]->COI;
	if(comparator(str1, str2) < 0){
      AA[i+j] = input1[i];
      i++;
    }else if(comparator(str1, str2) > 0){
      AA[i+j] = input2[j];
      j++;
    }else if(comparator(str1, str2) == 0){
      AA[i+j] = input1[i];
      i++;
      AA[i+j] = input2[j];
      j++;
    }
  }

  while (i < size1) { //copy remaining elements in i
	  AA[i + j] = input1[i];
	  i++;
  }

  while (j < size2) { //copy remaining elements in j
	  AA[i + j] = input2[j];
	  j++;
  }

  return AA;
}

int* mergeSort(int* indexes, int size){
  if(size == 1){
    return indexes;
  }

  int A1[size/2]; 
  int A2[size-(size/2)];

  int i;
  for(i = 0; i < size/2; i++){
    A1[i] = indexes[i];
  }
  for(i = 0; i < (size-(size/2)); i++){
    A2[i] = indexes[(size/2)+i];
  }
  
  int* SortedA1 = mergeSort(A1, size/2);
  int* SortedA2 = mergeSort(A2, (size-(size/2)));

  return merge(SortedA1, size/2, SortedA2, (size-(size/2)));
}

void printData(int fd, char* headerRow, int numRows){
  write(fd, headerRow, strlen(headerRow));
  int i;
  for(i = 0; i < numRows; i++){
    write(fd, "\n", 1);
    write(fd, data[indexArray[i]]->row, strlen(data[indexArray[i]]->row));
  }
}

void printLL(Node* front){
  while (front != NULL){
    printListing(front->element);
    front = front->next;
  }
}

void printListing(Listing* data){
  fprintf(stdout, "Column Value: |%s|, Row string: |%s|\n", data->COI, data->row);
}
