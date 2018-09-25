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

	size_t columns = 0;
	while(!feof(stdin)){
	  char* string = readString(stdin, 500, ','); 
	}
}