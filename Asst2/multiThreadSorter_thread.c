#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //read, write, etc.
#include <ctype.h> //isdigit, etc.
#include <dirent.h>
#include <fcntl.h> //O_CREAT, etc.
#include <sys/types.h> //pthread_t, size_t, etc.
#include <pthread.h>
#include "multiThreadSorter_thread.h"

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
    
    //Check column name exists
    if (findI(columnName) < 0){
        fprintf(stderr, "Column name invalid.\n");
        return -1;
    }

    DIR* inputDir, *outputDir;
    inputDir = opendir(inputDirPath);
    outputDir = opendir(outputDirPath);
    if (!inputDir || !outputDir){
        fprintf(stderr, "Directory not found.\n");
        return -1;
    }

    //Create thread for top level input directory
    TID = 1;
    ThreadArgs* argument = (ThreadArgs*) malloc(sizeof(ThreadArgs));
    argument->inputDirPath = inputDirPath;
    pthread_t thread;
    int result = pthread_create(&thread, NULL, directoryThread, argument);
    if (result != 0){
        fprintf(stderr, "Error creating main thread\n");
        return -1;
    }

    //Wait for all input reading to finish
    void* status;
    pthread_join(thread, &status);
    if ((int)(intptr_t) status < 0) return -1;

    //Moving link list to array of listings
    Listing* data[numRows];
    int i = numRows-1;
    while(front != NULL){
        data[i--] = front->element;
        Node* temp = front;
        front = front->next;
        free(temp);
    }

    //Determine datatype of COI
    int columnType = 0;
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
    
    //Create int array of indexes for sorting
    indexArray = (int*)malloc(numRows*sizeof(int));
    for (i = 0; i < numRows; i++){
        indexArray[i] = i;
    }

    //Sort!
    if (mergeSort(indexArray, 0, numRows - 1) != 0){
        fprintf(stderr, "Error sorting. \n");
        free(indexArray);
        freeArray(data, numRows);
        return -1;
    }

    //Set up output file
    char outputFile[strlen(outputDirPath)+ 17 + strlen(columnName) + 5];
    snprintf(outputFile, sizeof(outputFile), "%s/AllFiles-sorted-%s.csv", outputDirPath, columnName);
    int outputFD = open(outputFile, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (outputFD < 0){
        fprintf(stderr, "Unable to open/create output file.\n");
        free(indexArray);
        freeArray(data, numRows);
        return -1;
    }
    
    //Output data
    printData(outputFD, numRows);
    
    //Freedom at last
    free(headerString);
    free(indexArray);
    freeArray(data, numRows);

    return 0;
}

//Function for threads that process subdirectories
void* directoryThread(void* argument){
    ThreadArgs* args = (ThreadArgs*) argument;
    int totalThreads = 0;
    DIR* inputDir = opendir(args->inputDirPath);
    if (!inputDir){
        fprintf(stderr, "Directory not found.\n");
        return (void*) -1;
    }
    struct dirent* de;
    int fileObjects = 0;
    
    //Count number of file objects
    while((de = readdir(inputDir))){
        fileObjects++;
    }
    rewinddir(inputDir);

    pthread_t threads[fileObjects]; 
    int i;
    for (i = 0; i < fileObjects; i++){
        de = readdir(inputDir);
        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0){
            continue;
        }

        //Set up arguments
        ThreadArgs* argument = (ThreadArgs*) malloc(sizeof(ThreadArgs));
        char temp[strlen(args->inputDirPath) + 1 + strlen(de->d_name) + 1];
        snprintf(temp, sizeof(temp), "%s/%s", args->inputDirPath, de->d_name);
        argument->inputDirPath = temp;

        //Create thread
        int result;
        if (de->d_type == DT_DIR){
            result = pthread_create(&threads[i], NULL, directoryThread, argument);
        } else {
            result = pthread_create(&threads[i], NULL, fileThread, argument);
        }
        if (result != 0){
            fprintf(stderr, "Error creating thread\n");
            return (void*) -1;
        }
    }

    //Wait for all threads to finish
    for (i = 0; i < fileObjects; i++){
        void* status;
        pthread_join(threads[i], &status);
        if ((int)(intptr_t) status < 0) return (void*) -1;    
        else totalThreads += (int)(intptr_t) status + 1;
    }

    //Print metadata
    pthread_mutex_lock(&stdoutMutex);
    fprintf(stdout, "Initial PID: %d\nTIDS of all spawned threads: %d", getpid(), TID);
    int tid;
    for (tid = TID + 1; tid < TID + fileObjects; tid++){
        fprintf(stdout, ", %d", tid);

    }
    fprintf(stdout, "\nTotal number of threads: %d\n", totalThreads);
    pthread_mutex_unlock(&stdoutMutex);
    
    return (void*) totalThreads;
}

//Function for threads that process files
void* fileThread(void* argument){
  ThreadArgs* args = (ThreadArgs*) argument;  

  if (endsWith(args->inputDirPath, ".csv") != 1){
        fprintf(stderr, "Not a CSV file. (%s)\n", args->inputDirPath);
    }

    int inputFD = open(args->inputDirPath, O_RDONLY);
    if (inputFD < 0){
        fprintf(stderr, "Unable to open input file. (%s)\n", args->inputDirPath);
        return (void*) -1;
    }

    //Header line processing
    int headerIndexes[28];
    char* headerString = readLine(inputFD);
    if (!headerString){
        fprintf(stderr, "Header row missing. (%s)\n", args->inputDirPath);
        return (void*) -1;
    }
    int k = 0;
    char* savePtr;
    char* token = strtok_r(headerString, ",", &savePtr);
    while (token != NULL){
        int x = findI(token);
        if(x > -1){
            k++;
            if (k > 27){
                fprintf(stderr, "Too many columns. (%s)\n", args->inputDirPath);
                return (void*) -1;
            } 
            headerIndexes[k++] = x;
        } else {
            fprintf(stderr, "Improper column name. (%s)\n", args->inputDirPath);
            return (void*) -1;
        }
        token = strtok_r(NULL, ",", &savePtr);
    }

    //Create temporary linked list of rows
    Node* tempFront;
    char* line;
    while((line = readLine(inputFD))){
        Listing* temp = (Listing*) malloc(sizeof(Listing));
        if (!temp) {
            fprintf(stderr, "Out of memory.\n");
            free(line);
            free(headerString);
            freeLL(tempFront);
            return (void*) -1;
        }
        int i;
        if (populateListing(headerIndexes, k, findI(columnName), line, temp) < 0){
            fprintf(stderr, "Error parsing rows. (%s)\n", args->inputDirPath);
            free(temp);
            free(line);
            free(headerString);
            freeLL(tempFront);
            return (void*) -1;
        }
        tempFront = insertNode(tempFront, temp);
        numRows++;
        free(line);
    }

    //Append list to global list
    pthread_mutex_lock(&LLMutex);
    if(!front){
        front = tempFront;
    } else {
        Node* ptr = tempFront;
        while(ptr->next) ptr = ptr->next;
        ptr->next = front;
        front = tempFront;
    }
    pthread_mutex_unlock(&LLMutex);

    return (void*) 0;
}
