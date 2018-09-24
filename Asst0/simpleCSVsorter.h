#include <stdio.h>
#include <stdlib.h>

/*typedef struct Listing{
	char color[20];
	char director_name[50];
	int num_critic_for_reviews;
	int duration;
	int director_facebook_likes;
	int actor_3_facebook_likes;
	char actor_2_name[50];
	int actor_1_facebook_likes;
	int gross;
	char genres[200];
	char actor_1_name[50];
	char movie_title[100];
	int num_voted_users;
	int cast_total_facebook_likes;
	char actor_3_name[50];
	int facenumber_in_poster;
	char plot_keywords[300];
	char movie_imdb_link[60];
	int num_user_for_reviews;
	char language[15];
	char country[20];
	char content_rating[10];
	int budget;
	int title_year;
	int actor_2_facebook_likes;
	double imdb_score;
	double aspect_ratio;
	int movie_facebook_likes;
} Listing;*/

typedef union Value {
  int integer;
  double decimal;
  char* string;
} Value; //Just one element of a Listing

typedef struct Node {
	Listing* element;
	struct Node* next;
} Node;

int columns = 0;

typedef struct Listing {
	Value[columns] columnArray;
} Listing;

Node* front; //Link List to count rows of entries

Value** data;
int** dataTypes; // 0 for int, 1 for double, 2 for string

void insertNode(Listing* input);

char* readString(FILE* fp, size_t size, char stop);

//char* removeWhitespace(char* string);

//void printData(FILE* fp, Listing* data);

Value** mergeSort(Value** input, int** inputTypes);

//Suggestion: prototype a mergesort function
//multisort function, mergesort that takes an array, and has a configurable sorting element
