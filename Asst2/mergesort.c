#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
#include "scannerCSVsorter.h"

int endsWith(char* str, char* suffix){
  int index = strlen(str) - strlen(suffix);
  if (strcmp(str + index, suffix) == 0){
    return 1;
  }
  return 0;
}

void insertNode(Listing* input){
	Node* newNode = (Node*) malloc(sizeof(Node));
	newNode->element = input;
	newNode->next = front;
	front = newNode;
	return;
}

int findHeader(char* headerString){
  char* trimmedHeader = removeWhitespace(headerString);
  char temp[strlen(trimmedHeader) + 1]; //For storing the current column value
  temp[0] = '\0';
  int numQuotes = 0;
  int currentIndex = 0;
  int targetIndex = -1;
  char* token;
  while ((token = strsep(&trimmedHeader, ","))){
    strcat(temp, token);
    numQuotes += numChars(token, '"');
    if (numQuotes % 2 == 1){ //Check if comma is part of string
      strcat(temp, ",");
    } else if (strcmp(temp, columnName) == 0) { //If not, check the name
      targetIndex = currentIndex;
      currentIndex++;
    } else {
      temp[0] = '\0'; //Set string to empty
      currentIndex++;
    }
  }
  numColumns = currentIndex;
  free(trimmedHeader);
  return targetIndex;
}

int populateListing(int index, char* line, Listing* listing){
  //Copy entire string to listing
  char* temp = (char*) malloc((strlen(line) + 1) * sizeof(char));
  if (!temp) return -1;
  strcpy(temp, line);
  listing->row = temp;
  
  //Get string value of column of interest
  char* trimmedLine = removeWhitespace(line);
  char col[strlen(trimmedLine) + 1];
  col[0] = '\0';
  int numQuotes = 0;
  int currentIndex = 0;
  char* token;
  while ((token = strsep(&trimmedLine, ","))){ //Parse through commas
    if (currentIndex == index){
      strcat(col, token);
    }
    numQuotes += numChars(token, '"');
    if (numQuotes % 2 == 1){ //Check if comma is part of string
      if (currentIndex == index){
	strcat(col, ",");
      }
    } else {
      currentIndex++;
    }
  }
  if (currentIndex != numColumns){ //Check number of columns match
    free(temp);
    free(trimmedLine);
    return -1;
  }
  if (col[0] == '\0'){
    listing->COI = NULL;
  } else {
    char* new = (char*) malloc((strlen(col) + 1) * sizeof(char));
    if (!new) {
      free(temp);
      free(trimmedLine);
      return -1;
    }  
    strcpy(new, col);
    char* trimmed = removeWhitespace(new); //Remove whitespace outside quotes
    free(new);

    if (trimmed[0] == '"'){ //Crop off quotes
      char* noQuotes = (char*) malloc((strlen(trimmed) - 1) * sizeof(char));
      if (!noQuotes){
	free(temp);
	free(trimmedLine);
	free(trimmed);
	return -1;
      }
      trimmed[strlen(trimmed) - 1] = '\0';
      strcpy(noQuotes, &(trimmed[1]));
      free(trimmed);
      trimmed = removeWhitespace(noQuotes); //Remove whitespace inside quotes
      free(noQuotes);
    } 
    listing->COI = trimmed;
  }
  free(trimmedLine);
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
  string[end + 1] = '\0';
  strcpy(result, &(string[start]));
  return result;
}

int comparator(char* str1, char* str2) {
  if (!str1 && !str2){
    return 0;
  }
  if (!str1 || !str2){
    return (!str1) ? -1 : 1;
  }
  if (columnType == 0){ //double
    double num1 = atof(str1);
    double num2 = atof(str2);
    if (num1 < num2) return -1;
    if (num1 > num2) return 1;
    return 0;
  } else { //string
    return strcmp(str1, str2);
  }
}

int merge(int* indexes, int first, int middle, int last){
  int i = first, j = middle + 1, k;
  int* temp = (int*) malloc((last - first + 1) * sizeof(int));
  if(!temp) return -1;
  for (k = 0; k <= (last - first); k++){
    if (j > last || (i <= middle && comparator(data[indexes[i]]->COI, data[indexes[j]]->COI) <= 0)){
      temp[k] = indexes[i];
      i++;
    } else { //value in j is greater or i has run out
      temp[k] = indexes[j];
      j++;
    }
  }
  
  for (k = 0; k <= (last - first); k++){ //Copy temp back into array
    indexes[k + first] = temp[k];
  }
  free(temp);
  return 0;
}

int mergeSort(int* indexes, int first, int last){
  if (first < last){
    int middle = (first + last) / 2;
    if (mergeSort(indexes, first, middle) != 0) return -1;
    if (mergeSort(indexes, middle + 1, last) != 0) return -1;
    if (merge(indexes, first, middle, last) != 0) return -1;
    return 0;
  }
  return 0;
}

void printData(int fd, char* headerRow, int numRows){
  write(fd, headerRow, strlen(headerRow));
  int i;
  for(i = 0; i < numRows; i++){
    write(fd, "\n", 1);
    write(fd, data[indexArray[i]]->row, strlen(data[indexArray[i]]->row));
  }
}

void freeLL(Node* front){
  while (front != NULL){
    Node* temp = front;
    front = front->next;
    freeListing(temp->element);
    free(temp);
  }
}

void freeArray(Listing** data, int numRows){
  int i;
  for (i = 0; i < numRows; i++){
    freeListing(data[i]);
  }
}

void freeListing(Listing* data){
  free(data->COI);
  free(data->row);
  free(data);
}
