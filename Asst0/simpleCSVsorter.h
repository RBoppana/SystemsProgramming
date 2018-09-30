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
Listing** data;
int* indexArray;
int* AA;
int columnType; //0 for double, 1 for string

void insertNode(Listing* input);

int findHeader(char* headerString, char* columnName);

int populateListing(int index, char* line, Listing* listing);

char* readLine(int fd);

int numChars(char* string, char character);

char* removeWhitespace(char* string);

int comparator(char* str1, char* str2);

int* merge(int* input1, int size1, int* input2, int size2);

int* mergeSort(int* indexes, int size);

void printData(int fd, char* headerRow, int numRows);

void printLL(Node* front);

void printListing(Listing* data);
