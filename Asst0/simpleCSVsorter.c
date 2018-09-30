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

	char* headerString = readLine(0);
	if (*headerString == '\0'){
	  fprintf(stderr, "Header row missing.\n");
	  return -1;
	}

	int index = findHeader(headerString, argv[2]);
	if (index < 0){
	  fprintf(stderr, "Column name not found.\n");
	  return -1;
	}

	fprintf(stdout, "Index: %d\n", index);
	free(headerString);
	/*
	int numRows = 0;
	char* line;
	while((line = readLine(0))){
	  char* line = readLine(0);
	  Listing* temp = (Listing*)malloc(sizeof(Listing));
		populateListing(nthComma, readLine(0), temp);
		front = insertNode(temp);
		numRows++;
	}

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
