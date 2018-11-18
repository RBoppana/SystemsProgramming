#include <stdio.h>
#include <stdlib.h>

/*stypedef struct Listing {
  char* row;
  char* COI;
} Listing; */

typedef struct Listing{
	char* color;
	char* director_name;
	char* num_critic_for_reviews;
	char* duration;
	char* director_facebook_likes;
	char* actor_3_facebook_likes;
	char* actor_2_name;
	char* actor_1_facebook_likes;
	char* gross;
	char* genres;
	char* actor_1_name;
	char* movie_title;
	char* num_voted_users;
	char* cast_total_facebook_likes;
	char* actor_3_name;
	char* facenumber_in_poster;
	char* plot_keywords;
	char* movie_imdb_link;
	char* num_user_for_reviews;
	char* language;
	char* country;
	char* content_rating;
	char* budget;
	char* title_year;
	char* actor_2_facebook_likes;
	char* imdb_score;
	char* aspect_ratio;
	char* movie_facebook_likes;
    char* COI;
} Listing;

typedef struct Node {
	Listing* element;
	struct Node* next;
} Node;

typedef struct ThreadArgs {
	char* inputDirPath;
	int threadNo;
} ThreadArgs;

//Global variables
Node* front; //Linked List to count rows of entries
int numRows = 0;
int* indexArray;
char* columnName;
char* inputDirPath;
char* outputDirPath;
int firstProc; 
char* reference[28] = {"color","director_name","num_critic_for_reviews","duration","director_facebook_likes","actor_3_facebook_likes","actor_2_name","actor_1_facebook_likes","gross","genres","actor_1_name","movie_title","num_voted_users","cast_total_facebook_likes","actor_3_name","facenumber_in_poster","plot_keywords","movie_imdb_link","num_user_for_reviews","language","country","content_rating","budget","title_year","actor_2_facebook_likes","imdb_score","aspect_ratio","movie_facebook_likes"}; 
char* TIDs;
pthread_mutex_t LLMutex;
pthread_mutex_t TIDMutex;

void* directoryThread(void* args);

void* fileThread(void* args);

int findI(char* str);

int endsWith(char* str, char* suffix);

void appendList(Listing* input);

Listing* insertNode(Listing* list, Listing* input);

//int findHeader(char* headerString);

int populateListing(int* indexArr, int indexArrL, char* line, Listing* listing);

char* getListingField(Listing* listing, int index);

void setListingField(Listing* listing, int index, char* str);

char* readLine(int fd);

int numChars(char* string, char character);

char* removeWhitespace(char* string);

int comparator(char* str1, char* str2);

int merge(int* indexes, int first, int middle, int last);

int mergeSort(int* indexes, int first, int last);

void printData(int fd, char* headerRow, int numRows);

void freeLL(Node* front);

void freeArray(Listing** data, int numRows);

void freeListing(Listing* data);
