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

	/*while(){
		Listing* temp = (Listing*)malloc(sizeof(Listing));
		scanf("%*[^\n]\n%[^,]s,", temp->color);
		if (temp->color == NULL) {
			break;
			free(temp);
		}
		scanf("%s,", temp->director_name);
		scanf("%d,", temp->num_critic_for_reviews);
		scanf("%d,", temp->duration);
		scanf("%d,", temp->director_facebook_likes);
		scanf("%d,", temp->actor_3_facebook_likes);
		scanf("%s,", temp->actor_2_name);
		scanf("%d,", temp->actor_1_facebook_likes);
		scanf("%d,", temp->gross);
		scanf("%s,", temp->genres);
		scanf("%s,", temp->actor_1_name);
		scanf("%s,", temp->movie_title);
		scanf("%d,", temp->num_voted_users);
		scanf("%d,", temp->cast_total_facebook_likes);
		scanf("%s,", temp->actor_3_name);
		scanf("%d,", temp->facenumber_in_poster);
		scanf("%s,", temp->plot_keywords);
		scanf("%s,", temp->movie_imdb_link);
		scanf("%d,", temp->num_user_for_reviews);
		scanf("%s,", temp->language);
		scanf("%s,", temp->country);
		scanf("%s,", temp->content_rating);
		scanf("%d,", temp->budget);
		scanf("%d,", temp->title_year);
		scanf("%d,", temp->actor_2_facebook_likes);
		scanf("%f,", temp->imdb_score);
		scanf("%f,", temp->aspect_ratio);
		scanf("%d", temp->movie_facebook_likes); 
		insertNode(temp);
	}*/

}