#include <stdio.h>
#include <stdlib.h>

typedef struct Listing{
	char[20] color;
	char[50] director_name;
	int num_critic_for_reviews;
	int duration;
	int director_facebook_likes;
	int actor_3_facebook_likes;
	char[50] actor_2_name;
	int actor_1_facebook_likes;
	int gross;
	char[200] genres;
	char[50] actor_1_name;
	char[100] movie_title;
	int num_voted_users;
	int cast_total_facebook_likes;
	char[50] actor_3_name;
	int facenumber_in_poster;
	char[300] plot_keywords;
	char[60] movie_imdb_link;
	int num_user_for_reviews;
	char[15] language;
	char[20] country;
	char[10] content_rating;
	int budget;
	int title_year;
	int actor_2_facebook_likes;
	double imdb_score;
	double aspect_ratio;
	int movie_facebook_likes;
} Listing;

typedef struct Node{
	Listing* element;
	struct Node* next;
} Node;

Node* front = NULL;

void insertNode(Listing* input);

char* removeWhitespace(char* string);


//Suggestion: prototype a mergesort function
//multisort function, mergesort that takes an array, and has a configurable sorting element
