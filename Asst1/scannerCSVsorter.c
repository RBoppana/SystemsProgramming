#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include "scannerCSVsorter.h"

int main(int argc, char** argv){
        if (!(argc == 3 || argc == 5 || argc == 7)){
	  fprintf(stderr, "Please specify the correct number of arguments.\n");
	  fprintf(stderr, "Usage: ./scannerCSVsorter -c <column-name> [-d <input-directory>]\n                          [-o <output-directory>]\n");
		return -1;
	} else if (strcmp(argv[1], "-c") != 0){
	  fprintf(stderr, "Unrecognized flag.\n");
		return -1;
	}
	
	//Handle flags
	DIR* inputDir;
	int inputSet = 0;
	DIR* outputDir;
	int outputSet = 0;
	int i = 0;
	for (i = 0; i < (argc-3)/2; i++){
	  int flag = 3 + (2 * i);
	  if (strcmp(argv[flag], "-d") == 0){
	    inputDir = opendir(argv[flag + 1]);
	    inputSet = 1;
	  } else if (strcmp(argv[flag], "-o") == 0){
	    outputDir = opendir(argv[flag + 1]);
	    outputSet = 1;
	  } else {
	    fprintf(stderr, "Unrecognized flag.\n");
	    return -1;
	  }
	}
	if (inputSet != 1) inputDir = opendir(".");
	if (outputSet != 1) outputDir = opendir(".");
	if (!inputDir || !outputDir){
	  fprintf(stderr, "Directory not found");
	  return -1;
	}

	//char* fileName = traverseDir(inputDir);

	//if (strcmp(fileName, "done") == 0) {
		//print process data to sdout and return
	//}

	//Header line processing
	char* headerString = readLine(0);
	if (!headerString){
	  fprintf(stderr, "Header row missing.\n");
	  return -1;
	}
	char* headerRow = (char*) malloc((strlen(headerString) + 1) * sizeof(char)); //Keeps header string intact
	if (!headerRow){
	  free(headerString);
	  fprintf(stderr, "Out of memory.\n");
	  return -1;
	}
	strcpy(headerRow, headerString);
	int index = findHeader(headerString, argv[2]);
	if (index < 0){
	  fprintf(stderr, "Column name not found.\n");
	  free(headerString);
	  free(headerRow);
	  return -1;
	}
	
	//Create linked list of rows
	int numRows = 0;
	char* line;
	while((line = readLine(0))){
	  Listing* temp = (Listing*)malloc(sizeof(Listing));
	  if (!temp) {
	    fprintf(stderr, "Out of memory.\n");
	    free(line);
	    freeLL(front);
	    free(headerString);
	    free(headerRow);
	    return -1;
	  }
	  if (populateListing(index, line, temp) < 0){
	    fprintf(stderr, "Error parsing rows.\n");
	    free(temp);
	    free(line);
	    free(headerString);
	    free(headerRow);
	    freeLL(front);
	    return -1;
	  }
	  insertNode(temp);
	  numRows++;
	  free(line);
	}

	//printLL(front);
	
	//Put Linked List into array of pointers
	data = (Listing**)malloc(numRows*sizeof(Listing*));
	if (!data){
	  fprintf(stderr, "Out of memory.\n");
	  free(headerString);
	  free(headerRow);
	  freeLL(front);
	  return -1;
	}
	i = numRows-1;
	while(front != NULL){
	  data[i--] = front->element;
		Node* temp = front;
		front = front->next;
		free(temp);
	}

	//printArray(data, numRows);
	
	//Determine datatype of column
	columnType = 0;
	for (i = 0; i < numRows; i++) {
		char* COItemp = data[i]->COI;
		if (!COItemp) continue;
		int j, done = 0;
		for (j = 0; j < strlen(COItemp); j++) {
			if (isdigit(COItemp[j]) == 0 && COItemp[j] != '.' && COItemp[j] != '-') {
				columnType = 1;
				done = 1;
				break;
			}
		}
		if (done == 1) break;
	}
	
	//Create int array for sorting
	indexArray = (int*)malloc(numRows*sizeof(int));
	for (i = 0; i < numRows; i++){
		indexArray[i] = i;
	}
	
	//Sort!
	if (mergeSort(indexArray, 0, numRows - 1) != 0){
	  fprintf(stderr, "Error sorting.\n");
	  free(headerString);
	  free(indexArray);
	  freeArray(data, numRows);
	  return -1;
	}
	
	//Output data
	printData(stdout, headerRow, numRows);
	
	//Freedom at last
	free(headerString);
	free(indexArray);
	freeArray(data, numRows);

	closedir(inputDir);
	closedir(outputDir);

	return 0;
}
