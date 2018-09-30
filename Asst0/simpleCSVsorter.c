#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "simpleCSVsorter.h"

int main(int argc, char** argv){
	if(argc != 3){
	  fprintf(stderr, "Please specify the correct number of arguments.\n");
		return -1;
	} else if(strcmp(argv[1], "-c") != 0){
	  fprintf(stderr, "Unrecognized parameter.\n");
		return -1;
	}

	//Header line processing
	char* headerString = readLine(0);
	if (!headerString){
	  fprintf(stderr, "Header row missing.\n");
	  return -1;
	}
	int index = findHeader(headerString, argv[2]);
	if (index < 0){
	  fprintf(stderr, "Column name not found.\n");
	  return -1;
	}
	free(headerString);
	
	//Create linked list of rows
	int numRows = 0;
	char* line;
	while((line = readLine(0))){
	  Listing* temp = (Listing*)malloc(sizeof(Listing));
	  if (populateListing(index, line, temp) < 0){
	      fprintf(stderr, "Error parsing rows.\n");
	      return -1;
	    }
		insertNode(temp);
		numRows++;
		free(line);
	}

	printLL(front);

	/*
	data = (Listing*)malloc(numRows*sizeof(Listing));

	int i = numRows-1;
	while(front != NULL){
		data[i--] = front->element;
		Listing* temp = front;
		front = front->next;
		free(temp);
	}

	indexArray = (int*)malloc(numRows*sizeof(int));

	for(i = 0; i < numRows; i++){
		indexArray[i] = i;
	}

	indexArray = mergeSort(indexArray, numRows);
	
	printf("%s\n", headerRow);
	for(i = 0; i < numRows; i++){
		printf("%s\n", data[indexArray[i]]->row);
	}
	*/

	return 0;
}
