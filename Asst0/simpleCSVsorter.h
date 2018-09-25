#include <stdio.h>
#include <stdlib.h>

typedef union Value {
  int integer;
  double decimal;
  char* string;
} Value; 

typedef struct Node {
	Value* element;
	struct Node* next;
} Node;

Node* front; //Link List to count rows of entries

Value** data;
int** dataTypes; // 0 for int, 1 for double, 2 for string

void insertNode(Listing* input);

char* readString(FILE* fp, size_t size, char stop);

//char* removeWhitespace(char* string);

//void printData(FILE* fp, Listing* data);

Value** mergeSort(Value** input, int** inputTypes);

//Suggestion: prototype a mergesort function
//multisort function, mergesort that takes an array, and has a configurable sorting element
