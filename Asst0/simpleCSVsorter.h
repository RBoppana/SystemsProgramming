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

Node* front; //Link List to count rows of entries

Listing* data;
int* indexArray;

void insertNode(Listing* input);

char* populateListing(int index, char* line, Listing* listing);

char* readLine(int fd);

//char* removeWhitespace(char* string);

int* merge(int* inputArray1, int* inputArray2);

int* mergeSort(Listing* reference, int* indexes, int size);

void printData(int fd, Listing* data, int size);

//Suggestion: prototype a mergesort function
//multisort function, mergesort that takes an array, and has a configurable sorting element
