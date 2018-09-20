#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <simpleCSVsorter.h>

int main(int argc, char** argv){
	
	//Column Names
	scanf("%s,", temp->color);
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
	temp->double imdb_score);
	temp->double aspect_ratio); //idk the letter for double
	scanf("%d,", temp->int movie_facebook_likes);
	scanf("/n");

	while(/*there is more lines*/){
		Listing* temp = (Listing*)malloc(sizeof(Listing)); //should i reallocate memory instead of mallocing
		scanf("%s,", temp->color);
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
		temp->double imdb_score);
		temp->double aspect_ratio); //idk the letter for double
		scanf("%d/n", temp->movie_facebook_likes); //might cause error if last characteris not newline
		insertNode(temp);
	}//LinkList is complete with all elements

	//*all stored strings need to be trimmed when actually sorting*

	int counter = 0;

	Node* ptr = front;
	while(ptr != NULL){
		counter++;
		ptr = ptr->next;
	}//# of LinkList elements set

	Listing[counter] collection; //not sure if this needs to be malloced

	ptr = front;

	for(int i = 0; i < counter; i++){
		collection[i] = /*&*/ptr->element; //not sure if it needs & symbol
	}

	collection = MergeSort(collection);

	//curtains close
}
