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

char* readLine(FILE* fp, size_t size, char stop, Listing* storageLocation);

//char* removeWhitespace(char* string);

//void printData(FILE* fp, Listing* data);

Value** mergeSort(Value** input, int** inputTypes);

//Suggestion: prototype a mergesort function
//multisort function, mergesort that takes an array, and has a configurable sorting element
