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

typedef struct DirThreadArgs {
	char** inputDirPath;
	int threadNo;
} DirThreadArgs;

//Global variables
Node* front; //Linked List to count rows of entries
Listing** data;
int* indexArray;
char* columnName;
char* TIDs;
pthread_mutex_t LLMutex;
pthread_mutex_t TIDMutex;

int traverseDir(DIR* inputDir);

int sortCSV(char* inputName);

int endsWith(char* str, char* suffix);

void insertNode(Listing* input);

int findHeader(char* headerString);

int populateListing(int index, char* line, Listing* listing);

char* readLine(int fd);

int numChars(char* string, char character);

char* removeWhitespace(char* string);

int comparator(char* str1, char* str2);

int merge(int* indexes, int first, int middle, int last);

int mergeSort(int* indexes, int first, int last);

void printData(int fd, char* headerRow, int numRows);

void freeLL(Node* front);

void freeArray(Listing** data, int numRows);

void freeListing(Listing* data);
