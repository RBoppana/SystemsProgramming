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
int columnType; //0 for double, 1 for string
int numColumns;

void insertNode(Listing* input);

int findHeader(char* headerString, char* columnName);

int populateListing(int index, char* line, Listing* listing);

char* readLine(int fd);

int numChars(char* string, char character);

char* removeWhitespace(char* string);

int comparator(char* str1, char* str2);

int merge(int* indexes, int first, int middle, int last);

int mergeSort(int* indexes, int first, int last);

void printData(FILE* fp, char* headerRow, int numRows);

void printLL(Node* front);

void printArray(Listing** data, int numRows);

void printListing(Listing* data);

void freeLL(Node* front);

void freeArray(Listing** data, int numRows);

void freeListing(Listing* data);
