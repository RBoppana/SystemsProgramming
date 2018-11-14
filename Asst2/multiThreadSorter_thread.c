#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //read, write, etc.
#include <ctype.h> //isdigit, etc.
#include <dirent.h>
#include <fcntl.h> //O_CREAT, etc.
#include <sys/types.h> //pthread_t, size_t, etc.
#include <pthread.h>
#include "scannerCSVsorter.h"

#define USAGE "Usage: ./scannerCSVsorter -c <column-name> [-d <input-directory>]\n                          [-o <output-directory>]\n"

int main(int argc, char** argv){

    //Handle flags
    int columnSet = 0, inputSet = 0, outputSet = 0;
    char option;
    opterr = 0;
    while ((option = getopt(argc, argv, "c:d:o:")) != -1){
      switch (option){
      case 'c':
        columnName = optarg;
        columnSet = 1;
        break;
      case 'd':
        inputDirPath = optarg;
        inputSet = 1;
        break;
      case 'o':
        outputDirPath = optarg;
        outputSet = 1;
        break;
      case '?':
        if (optopt == 'c' || optopt == 'd' || optopt == 'o'){
          fprintf(stderr, "Option -%c requires an argument.\n", optopt);
          return -1;
        } else {
          fprintf(stderr, "Unrecognized option -%c.\n", optopt);
          fprintf(stderr, USAGE);
          return -1;
        }
      default:
        fprintf(stderr, "Error reading options.\n");
        return -1;
      }
    }
    if (optind < argc){
      fprintf(stderr, "Unrecognized argument %s.\n", argv[optind]);
      fprintf(stderr, USAGE);
      return -1;
    }
    if (columnSet != 1){
      fprintf(stderr, "Please specify the column name.\n");
      fprintf(stderr, USAGE);
      return -1;
    }
    if (inputSet != 1) inputDirPath = "."; 
    if (outputSet != 1) outputDirPath = "."; 
    
    DIR* inputDir, *outputDir;
    inputDir = opendir(inputDirPath);
    outputDir = opendir(outputDirPath);
    if (!inputDir || !outputDir){
      fprintf(stderr, "Directory not found.\n");
      return -1;
    }

    //Print metadata
    fprintf(stdout, "Initial PID: %d\nTIDS of all spawned threads: ", getpid());
    fflush(stdout);

    firstProc = 1;
    int procs = traverseDir(inputDir);
    if (procs < 0){
      fprintf(stderr, "Error while traversing input directory.\n");
      return -1;
    }
    
    fprintf(stdout, "\nTotal number of processes: %d\n", procs);

    return 0;
}

//Function for threads that process directories
void* directoryThread(void* args){
    int totalThreads = 0;
    DIR* inputDir = opendir(args->inputDirPath);
    struct direct* de;
    while((de = readdir(inputDir))){
        if (de->d_type == DT_DIR){ //Encounter a directory
            if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0){
                continue;
            }

            pthread_t thread;
            DirThreadArgs* argument = (DirThreadArgs*) malloc(sizeof(DirThreadArgs));
            argument->inputDirPath = 
            pthread_create(&thread, NULL, directoryThread, );
        }
    }
}

//Function for threads that process files
void* fileThread(){

}

void* traverseDir(DIR* inputDir, int firstThread){
  int totalThreads = 0;
  struct dirent* de;
  while((de = readdir(inputDir))){
    if (de->d_type == DT_DIR){ //Encounter a directory
      if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0){
    continue;
      }

      
      pid_t pid = fork();
      if (pid == -1){
        fprintf(stderr, "Error forking.\n");
        return -1;
      } 
      if (pid == 0){ //child
    if (firstProc == 1) fprintf(stdout, "%d", getpid());
    else fprintf(stdout, ", %d", getpid());
    fflush(stdout);
    firstProc = 0;

    //Change input path
    char temp[strlen(inputDirPath) + 1 + strlen(de->d_name) + 1];
    snprintf(temp, sizeof(temp), "%s/%s", inputDirPath, de->d_name);
    inputDirPath = temp;

    int recursiveProcs = traverseDir(opendir(inputDirPath));
    exit(recursiveProcs); //Set exit status to number of procs
      } else { //parent
    firstProc = 0;
    int status;
    wait(&status);
    if (WIFEXITED(status)){
      totalProcs += WEXITSTATUS(status) + 1;
    } else {
      return -1;
    }
      }
      
    } else { //Not a directory
      pid_t pid = fork();
      if (pid == -1) {
    fprintf(stderr, "Error forking.\n");
    return -1;
      }
      if (pid == 0){ //child
    if (firstProc == 1) fprintf(stdout, "%d", getpid());
    else fprintf(stdout, ", %d", getpid());
    fflush(stdout);

    if (de->d_type == DT_REG && endsWith(de->d_name, ".csv") == 1){ //Encountered a CSV file
      //Check if already sorted
      char sorted[8 + strlen(columnName) + 4 + 1];
      snprintf(sorted, sizeof(sorted), "-sorted-%s.csv", columnName);
      if (endsWith(de->d_name, sorted) == 1){
        fprintf(stderr, "Already sorted. (%s/%s)\n", inputDirPath, de->d_name);
        exit(-1);
      }

      int sortResult = sortCSV(de->d_name);
      exit(sortResult);
    } else {
      fprintf(stderr, "Not a CSV file. (%s/%s)\n", inputDirPath, de->d_name);
      exit(-1);
    }
      } else { //parent
    firstProc = 0;
    int status;
    wait(&status);
    if (WIFEXITED(status)){
      totalProcs++;
    } //Ignore incorrect csv files
      }
    }
  }
  return totalProcs;
}

int sortCSV(char* inputName){
  //Set up input file
  char inputFile[strlen(inputDirPath) + 1 + strlen(inputName) + 1];
  snprintf(inputFile, sizeof(inputFile), "%s/%s", inputDirPath, inputName);
  int inputFD = open(inputFile, O_RDONLY);
  if (inputFD < 0){
    fprintf(stderr, "Unable to open input file. (%s/%s)\n", inputDirPath, inputName);
    return -1;
  }

  //Header line processing
    char* headerString = readLine(inputFD);
    if (!headerString){
      fprintf(stderr, "Header row missing. (%s/%s)\n", inputDirPath, inputName);
      return -1;
    }
    char* headerRow = (char*) malloc((strlen(headerString) + 1) * sizeof(char)); //Keeps header string intact
    if (!headerRow){
      free(headerString);
      fprintf(stderr, "Out of memory.\n");
      return -1;
    }
    strcpy(headerRow, headerString);
    int index = findHeader(headerString);
    if (index < 0){
      fprintf(stderr, "Column name not found. (%s/%s)\n", inputDirPath, inputName);
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
        fprintf(stderr, "Error parsing rows. (%s/%s)\n", inputDirPath, inputName);
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
      fprintf(stderr, "Error sorting. (%s)\n", inputName);
      free(headerString);
      free(indexArray);
      freeArray(data, numRows);
      return -1;
    }

    //Set up output file
    char outputFile[strlen(outputDirPath) + 1 + strlen(inputName) + 8 + strlen(columnName) + 1];
    snprintf(outputFile, sizeof(outputFile), "%s/%.*s-sorted-%s.csv", outputDirPath, (int)(strlen(inputName)-4), inputName, columnName);
    int outputFD = open(outputFile, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (outputFD < 0){
      fprintf(stderr, "Unable to open/create output file. (%s/%s)\n", inputDirPath, inputFile);
      free(headerString);
      free(indexArray);
      freeArray(data, numRows);
      return -1;
    }
    
    //Output data
    printData(outputFD, headerRow, numRows);
    
    //Freedom at last
    free(headerString);
    free(indexArray);
    freeArray(data, numRows);

    return 0;
}
