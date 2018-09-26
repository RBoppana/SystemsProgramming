#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "simpleCSVsorter.h"

int main(int argc, char** argv){
	if(argc != 3){
	  fprintf(stderr, "Please specify the correct number of arguments.");
		return -1;
	} else if(strcmp(argv[1], "-c") != 0){
	  fprintf(stderr, "Unrecognized parameter.");
		return -1;
	}

	char* headerRow = readHeader();

	//go character by character, when characters match, see if all characters in string match, then nthcomma as comma before desired column
	//later on david will set COI value with string after nthcomma in each row
	int nthComma = 0;
	char* headerRow = data[0]->row;
	i = 0;
	while(){
		if(headerRow[i] == \0)
			break;
		while(headerRow[i] == argv[2][0]){
			i++;
		}
		if(headerRow[i] == ',')
			nthComma++;
		i++;
	}

	int numRows = 0;
	while(!feof(stdin)){
		Listing* temp = (Listing*)malloc(sizeof(Listing));
		readLine(,,nthComma,temp);
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

	/*int firstRowDone = 0;
	char* columnHeads;
	while(!firstRowDone){
	  columns++;
		if(readString(stdin, columnHeads, 1000, ',')){
			firstRowDone = 1;
		}
	}

	dataTypes = (int*)malloc(columns*sizeof(int));

	int numRows = 0;
	while(!feof(stdin)){
	  Value* temp = (Value*)malloc(colums*sizeof(Value));
		for(int i = 0; i < columns; i++){
			readString(stdin, temp[i], 1000, ',');
		}
		front = insertNode(temp);
		numRows++;
		//free(temp);?
	}

	data = (Value**)malloc(numRows*sizeof(Value*));
	int ptr = 0;
	while(front != NULL){
		data[ptr++] = front;
		front = front.next; 
	}*/

	//mergesort data

}