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
    if(strcmp(reference[i], str) == 0) return i;
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
      listing->color = str; break;
    case 1:
      listing->director_name = str; break;
    case 2:
      listing->num_critic_for_reviews = str; break;
    case 3:
      listing->duration = str; break;
    case 4:
      listing->director_facebook_likes = str; break;
    case 5:
      listing->actor_3_facebook_likes = str; break;
    case 6:
      listing->actor_2_name = str; break;
    case 7:
      listing->actor_1_facebook_likes = str; break;
    case 8:
      listing->gross = str; break;
    case 9:
      listing->genres = str; break;
    case 10:
      listing->actor_1_name = str; break;
    case 11:
      listing->movie_title = str; break;
    case 12:
      listing->num_voted_users = str; break;
    case 13:
      listing->cast_total_facebook_likes = str; break;
    case 14:
      listing->actor_3_name = str; break;
    case 15:
      listing->facenumber_in_poster = str; break;
    case 16:
      listing->plot_keywords = str; break;
    case 17:
      listing->movie_imdb_link = str; break;
    case 18:
      listing->num_user_for_reviews = str; break;
    case 19:
      listing->language = str; break;
    case 20:
      listing->country = str; break;
    case 21:
      listing->content_rating = str; break;
    case 22:
      listing->budget = str; break;
    case 23:
      listing->title_year = str; break;
    case 24:
      listing->actor_2_facebook_likes = str; break;
    case 25:
      listing->imdb_score = str; break;
    case 26:
      listing->aspect_ratio = str; break;
    case 27:
      listing->movie_facebook_likes = str; break;
    default:
      break; //random bs case
  }
}

int populateListing(int* indexArr, int indexArrL, int targetIndex, char* line, Listing* listing){
  int rowLength = 0;
  
  //Get string value of column of interest
  char col[strlen(line) + 1];
  col[0] = '\0';
  int numQuotes = 0;
  int currentIndex = 0;
  char* token;
  while ((token = strsep(&line, ","))){ //Parse through commas
    strcat(col, token);
    numQuotes += numChars(token, '"');
    if (numQuotes % 2 == 1){ //Comma is part of string
        strcat(col, ",");
    } else { //Comma is delimiting CSV
      if (col[0] == '\0'){
        setListingField(listing, indexArr[currentIndex], NULL);
      } else {
        char* new = (char*) malloc(strlen(col) + 1);
        if (!new) {
          return -1;
        }
        strcpy(new, col);
        setListingField(listing, indexArr[currentIndex], new);
        rowLength += strlen(col);
        col[0] = '\0';
        currentIndex++;
      }
    }
  }
  if (currentIndex != indexArrL){ //Check number of columns match
    return -1;
  }

  //Create row string in correct order
  char* temp = (char*) malloc(rowLength + 28);
  if (!temp) {
    return -1;
  strcat(temp, getListingField(listing, 0));
  int i;
  for (i = 1; i < 28; i++){
    strcat(temp, ",");
    strcat(temp, getListingField(listing, i));
  }
  listing->row = temp;
  free(temp);

  //Set column of interest
  char* trimmed = removeWhitespace(getListingField[targetIndex]); //Remove whitespace and quotes
  listing->COI = trimmed;

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

//Removes whitespace and quotes around a string
char* removeWhitespace(char* string){
  int start, end;
  int i;
  for (i = 0; i < strlen(string); i++){ //Preceding whitespace
    if (isspace(string[i]) || string[i] == '"') continue;
    start = i;
    break; 
  }
  for (i = strlen(string) - 1; i >= 0; i--){ //Proceding whitespace
    if (isspace(string[i]) || string[i] == '"') continue;
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

void printData(int fd, int numRows){
  write(fd, "color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,cast_total_facebook_likes,actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes", 417);
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
