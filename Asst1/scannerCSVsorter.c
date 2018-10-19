#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/types.h>
#include "scannerCSVsorter.h"

int traverseDir(DIR* directory);
int sortCSV(int inputFD, int outputFD, char* columnName);

int main(int argc, char** argv){
        if (!(argc == 3 || argc == 5 || argc == 7)){
	  fprintf(stderr, "Please specify the correct number of arguments.\n");
	  fprintf(stderr, "Usage: ./scannerCSVsorter -c <column-name> [-d <input-directory>]\n                          [-o <output-directory>]\n");
		return -1;
	}
	
	//Handle flags
	DIR* inputDir, outputDir;
	char* columnName;
	int inputSet = 0, outputSet = 0, columnSet = 0;
	int flag;
	for (flag = 1; flag < argc; flag += 2){
	  if (strcmp(argv[flag], "-c") == 0){
	    columnName = argv[flag + 1];
	    columnSet = 1;
	  } else if (strcmp(argv[flag], "-d") == 0){
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
	if (columnSet != 1){
	  fprintf(stderr, "Please specify the column name.\m");
	  return -1;
	}
	if (inputSet != 1) inputDir = opendir(".");
	if (outputSet != 1) outputDir = opendir(".");
	if (!inputDir || !outputDir){
	  fprintf(stderr, "Directory not found");
	  return -1;
	}

	char num[10];
	sprintf(num, "%d", getpid());
	write(0, "Initial PID: ", 13);
	write(0, num, strlen(num));
	write(0, "\nPIDS of all child processes: ", 30);

	int procs = traverse(inputDir, outputDir);

	sprintf(num, "%d", procs);
	write(0, "\b\nTotal number of processes: ", 29); //Backspace last comma and flush with newline
	write(0, num, strlen);
	write(0, "\n", 1);

	return 0;
}

int traverse(DIR* inputDir, DIR* outputDir){
  int totalProcs = 0;
  struct dirent* de;
  while((de = readdir(inputDir))){
    if (de->d_type == DT_DIR){ //Encounter a directory
      if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0){
	continue;
      }
      pid_t pid = fork();
      if (pid == -1) return -1;
      if (pid == 0){ //child
	char num[10];
	sprintf(num, "%d", getpid());
	write(0, num, strlen(num));
	write(0, ",", 1);
	int recursiveProcs = traverse(opendir(de->d_name), outputDir);
	exit(recursiveProcs); //Set exit status to number of procs
      } else { //parent
	int status;
	wait(&status);
	if (WIFEXITED(status)){
	  totalProcs += WEXITSTATUS(status);
	} else {
	  return -1;
	}
      }
    } else if (de->d_type == DT_REG && isCSV(de->d_name) == 1){ //Encounter a CSV file
      totalProcs++;
      pid_t pid = fork();
      if (pid == -1) return -1;
      if (pid == 0){ //child
	//sort stuff
      } else {
	int status
	wait(&status);
	if (!WIFEXITED(status)) return -1;
      }
    }
  }
  return totalProcs;
}

int sortCSV(int inputFD, int outputFD, char* columnName){
	//Header line processing
	char* headerString = readLine(inputFD);
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
	int index = findHeader(headerString, columnName);
	if (index < 0){
	  fprintf(stderr, "Column name not found.\n");
	  free(headerString);
	  free(headerRow);
	  return -1;
	}
	
	//Create linked list of rows
	int numRows = 0;
	char* line;
	while((line = readLine(inputFD))){
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
	int i = numRows-1;
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
	//**Change to write()**
	//printData(outputFD, headerRow, numRows);
	
	//Freedom at last
	free(headerString);
	free(indexArray);
	freeArray(data, numRows);

	return 0;
}
