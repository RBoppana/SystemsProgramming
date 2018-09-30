#include <stdio.h>
#include <stdlib.h>

typedef struct Listing {
  char* row;
  char* COI;
} Listing; 

typedef struct Node {
	Listing* element;
	struct Node* next;
} Node;

Node* front; //Linked List to count rows of entries

Listing* data;
int* indexArray;
enum types{doubles, strings};
enum types columnType;

void insertNode(Listing* input);

int findHeader(char* headerString, char* columnName);

int populateListing(int index, char* line, Listing* listing);

char* readLine(int fd);

int numChars(char* string, char character);

//char* removeWhitespace(char* string);

//int comparator(char* str1, char* str2);

//int* merge(int* input1, int size1, int* input2, int size2);

//int* mergeSort(int* indexes, int size);

void printData(int fd, Listing* data, int size);

void printLL(Node* front);

void printListing(Listing* data);

//Suggestion: prototype a mergesort function
//multisort function, mergesort that takes an array, and has a configurable sorting element
