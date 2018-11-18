#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
#include "scannerCSVsorter.h"

int endsWith(char* str, char* suffix){
  int index = strlen(str) - strlen(suffix);
  if (strcmp(str + index, suffix) == 0){
    return 1;
  }
  return 0;
}

int findI(char* str){
  int i;
  for(i = 0; i < 28, i++){
    if(strcmp(reference[i], removeWhitespace(str)) == 0) return i;
  }
  return -1;
}

Node* insertNode(Node* list, Listing* input){ //Inserts a node into a temporary list
	Node* newNode = (Node*) malloc(sizeof(Node));
  if(!newNode) return NULL;
	newNode->element = input;
	newNode->next = list;
	return newNode;
}

/*int findHeader(char* headerString){
  char* trimmedHeader = removeWhitespace(headerString);
  char temp[strlen(trimmedHeader) + 1]; //For storing the current column value
  temp[0] = '\0';
  int numQuotes = 0;
  int currentIndex = 0;
  int targetIndex = -1;
  char* token;
  while ((token = strsep(&trimmedHeader, ","))){
    strcat(temp, token);
    numQuotes += numChars(token, '"');
    if (numQuotes % 2 == 1){ //Check if comma is part of string
      strcat(temp, ",");
    } else if (strcmp(temp, columnName) == 0) { //If not, check the name
      targetIndex = currentIndex;
      currentIndex++;
    } else {
      temp[0] = '\0'; //Set string to empty
      currentIndex++;
    }
  }
  //numColumns = currentIndex;
  free(trimmedHeader);
  return targetIndex;
}*/

char* getListingField(Listing* listing, int index){
  switch(index){
    case 0:
      return listing->color;
    case 1:
      return listing->director_name;
    case 2:
      return listing->num_critic_for_reviews;
    case 3:
      return listing->duration;
    case 4:
      return listing->director_facebook_likes;
    case 5:
      return listing->actor_3_facebook_likes;
    case 6:
      return listing->actor_2_name;
    case 7:
      return listing->actor_1_facebook_likes;
    case 8:
      return listing->gross;
    case 9:
      return listing->genres;
    case 10:
      return listing->actor_1_name;
    case 11:
      return listing->movie_title;
    case 12:
      return listing->num_voted_users;
    case 13:
      return listing->cast_total_facebook_likes;
    case 14:
      return listing->actor_3_name;
    case 15:
      return listing->facenumber_in_poster;
    case 16:
      return listing->plot_keywords;
    case 17:
      return listing->movie_imdb_link;
    case 18:
      return listing->num_user_for_reviews;
    case 19:
      return listing->language;
    case 20:
      return listing->country;
    case 21:
      return listing->content_rating;
    case 22:
      return listing->budget;
    case 23:
      return listing->title_year;
    case 24:
      return listing->actor_2_facebook_likes;
    case 25:
      return listing->imdb_score;
    case 26:
      return listing->aspect_ratio;    
    case 27:
      return listing->movie_facebook_likes;    
    default:
      return NULL; //random bs case
  }
}

void setListingField(Listing* listing, int index, char* str){
  switch(index){
    case 0:
      listing->color = str;
    case 1:
      listing->director_name = str;
    case 2:
      listing->num_critic_for_reviews = str;
    case 3:
      listing->duration = str;
    case 4:
      listing->director_facebook_likes = str;
    case 5:
      listing->actor_3_facebook_likes = str;
    case 6:
      listing->actor_2_name = str;
    case 7:
      listing->actor_1_facebook_likes = str;
    case 8:
      listing->gross = str;
    case 9:
      listing->genres = str;
    case 10:
      listing->actor_1_name = str;
    case 11:
      listing->movie_title = str;
    case 12:
      listing->num_voted_users = str;
    case 13:
      listing->cast_total_facebook_likes = str;
    case 14:
      listing->actor_3_name = str;
    case 15:
      listing->facenumber_in_poster = str;
    case 16:
      listing->plot_keywords = str;
    case 17:
      listing->movie_imdb_link = str;
    case 18:
      listing->num_user_for_reviews = str;
    case 19:
      listing->language = str;
    case 20:
      listing->country = str;
    case 21:
      listing->content_rating = str;
    case 22:
      listing->budget = str;
    case 23:
      listing->title_year = str;
    case 24:
      listing->actor_2_facebook_likes = str;
    case 25:
      listing->imdb_score = str;
    case 26:
      listing->aspect_ratio = str;    
    case 27:
      listing->movie_facebook_likes = str;    
    default:
      listing->COI = NULL; //random bs case
  }
}

int populateListing(int* indexArr, int indexArrL, char* line, Listing* listing){
  //Copy entire string to listing
  char* temp = (char*) malloc((strlen(line) + 1) * sizeof(char));
  if (!temp) return -1;
  strcpy(temp, line);
  listing->row = temp;
  
  //Get string value of column of interest
  char* trimmedLine = removeWhitespace(line);
  char col[strlen(trimmedLine) + 1];
  col[0] = '\0';
  int numQuotes = 0;
  int currentIndex = 0;
  char* token;
  while ((token = strsep(&trimmedLine, ","))){ //Parse through commas
    strcat(col, token);
    setListingField(listing, indexArr[currentIndex], col);
    numQuotes += numChars(token, '"');
    if (numQuotes % 2 == 1){ //Check if comma is part of string
      strcat(col, ",");
      setListingField(listing, indexArr[currentIndex], col);
    } else {
      currentIndex++;
    }
  }
  if (currentIndex != indexArrL){ //Check number of columns match
    free(temp);
    free(trimmedLine);
    return -1;
  }
  /*if (col[0] == '\0'){
    listing->COI = NULL;
  } else {
    char* new = (char*) malloc((strlen(col) + 1) * sizeof(char));
    if (!new) {
      free(temp);
      free(trimmedLine);
      return -1;
    }  
    strcpy(new, col);
    char* trimmed = removeWhitespace(new); //Remove whitespace outside quotes
    free(new);

    if (trimmed[0] == '"'){ //Crop off quotes
      char* noQuotes = (char*) malloc((strlen(trimmed) - 1) * sizeof(char));
      if (!noQuotes){
        free(temp);
        free(trimmedLine);
        free(trimmed);
        return -1;
      }
      trimmed[strlen(trimmed) - 1] = '\0';
      strcpy(noQuotes, &(trimmed[1]));
      free(trimmed);
      trimmed = removeWhitespace(noQuotes); //Remove whitespace inside quotes
      free(noQuotes);
    } 
    listing->COI = trimmed;
  }*/
  free(trimmedLine);
  return 0;
}

char* readLine(int fd){
  size_t capacity = 100;
  char* string = (char*) malloc(capacity * sizeof(char));
  if (!string) return string;
  size_t length = 0;
  char current;
  while (read(fd, &current, sizeof(char)) != 0 && current != '\n'){
    string[length] = current;
    length++;
    if (length == capacity){
      capacity *= 2;
      string = (char*) realloc(string, capacity * sizeof(char));
      if (!string) return string;
    }
  }
  if (length == 0 && current != '\n'){
    free(string);
    return NULL;
  } 
  string[length] = '\0';
  length++;
  return (char*) realloc(string, length * sizeof(char));
}

int numChars(char* string, char character){
  if (!string){
    return 0;
  }
  int count = 0, i;
  for (i = 0; i < strlen(string); i++){
    if (string[i] == character) count++;
  }
  return count;
}

char* removeWhitespace(char* string){
  int start, end;
  int i;
  for (i = 0; i < strlen(string); i++){ //Preceding whitespace
    if (isspace(string[i])) continue;
    start = i;
    break; 
  }
  for (i = strlen(string) - 1; i >= 0; i--){ //Proceding whitespace
    if (isspace(string[i])) continue;
    end = i;
    break;
  }
  char* result = (char*) malloc((end - start + 2)* sizeof(char));
  if (!result) return result;
  string[end + 1] = '\0';
  strcpy(result, &(string[start]));
  return result;
}

int comparator(char* str1, char* str2) {
  if (!str1 && !str2){
    return 0;
  }
  if (!str1 || !str2){
    return (!str1) ? -1 : 1;
  }
  if (columnType == 0){ //double
    double num1 = atof(str1);
    double num2 = atof(str2);
    if (num1 < num2) return -1;
    if (num1 > num2) return 1;
    return 0;
  } else { //string
    return strcmp(str1, str2);
  }
}

int merge(int* indexes, int first, int middle, int last){
  int i = first, j = middle + 1, k;
  int* temp = (int*) malloc((last - first + 1) * sizeof(int));
  if(!temp) return -1;
  for (k = 0; k <= (last - first); k++){
    if (j > last || (i <= middle && comparator(data[indexes[i]]->COI, data[indexes[j]]->COI) <= 0)){
      temp[k] = indexes[i];
      i++;
    } else { //value in j is greater or i has run out
      temp[k] = indexes[j];
      j++;
    }
  }
  
  for (k = 0; k <= (last - first); k++){ //Copy temp back into array
    indexes[k + first] = temp[k];
  }
  free(temp);
  return 0;
}

int mergeSort(int* indexes, int first, int last){
  if (first < last){
    int middle = (first + last) / 2;
    if (mergeSort(indexes, first, middle) != 0) return -1;
    if (mergeSort(indexes, middle + 1, last) != 0) return -1;
    if (merge(indexes, first, middle, last) != 0) return -1;
    return 0;
  }
  return 0;
}

void printData(int fd, char* headerRow, int numRows){
  write(fd, headerRow, strlen(headerRow));
  int i;
  for(i = 0; i < numRows; i++){
    write(fd, "\n", 1);
    write(fd, data[indexArray[i]]->row, strlen(data[indexArray[i]]->row));
  }
}

void freeLL(Node* front){
  while (front != NULL){
    Node* temp = front;
    front = front->next;
    freeListing(temp->element);
    free(temp);
  }
}

void freeArray(Listing** data, int numRows){
  int i;
  for (i = 0; i < numRows; i++){
    freeListing(data[i]);
  }
}

void freeListing(Listing* data){
  free(data->COI);
  free(data->row);
  free(data);
}
