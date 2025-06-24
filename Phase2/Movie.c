/*****************************************************
 * @file   Movie.c                                   *
 * @author Paterakis Giorgos <geopat@csd.uoc.gr>     *
 *                                                   *
 * @brief Implementation for Movie.h 				 *
 * Project: Winter 2023						         *
 *****************************************************/
#include "Movie.h"

/**
 * @brief Creates a new user.
 * Creates a new user with userID as its identification.
 *
 * @param userID The new user's identification
 *
 * @return 1 on success
 *         0 on failure
 */

 int register_user(int userID){
	int index = hash_function(userID);
	user_t *ptr = user_hashtable_p[index];
	int i;
	int counter = 1; 
	int flag = 0;

	while(ptr!=NULL && ptr->userID!=userID){
		ptr = ptr->next;
	}
	if(ptr != NULL){
		flag = 1;
	}
	else{
		ptr = malloc(sizeof(user_t*));
		ptr->userID = userID;
		ptr->history = NULL;
		if(user_hashtable_p[index] == NULL){
			user_hashtable_p[index] = ptr;
		}
		else{
			ptr->next = user_hashtable_p[index];
			user_hashtable_p[index] = ptr;
		}
	}
	printf("\nChain %d of Users:",index);
	ptr = user_hashtable_p[index];
	if(ptr!=NULL){
		while(ptr){
			printf("\n    %d", ptr->userID);
			ptr = ptr->next;
		}
	}
	counter++;
	printf("\n");
	printf("DONE\n\n");
	if(flag){
		return 0;
	}
	return 1;
 }
 
/**
 * @brief Deletes a user.
 * Deletes a user with userID from the system, along with users' history tree.
 *
 * @param userID The new user's identification
 *
 * @return 1 on success
 *         0 on failure
 */

 int unregister_user(int userID){
	int index = hash_function(userID);
	int flag = 0;
	int i;
	int counter = 0;
	user_t* ptr = user_hashtable_p[index];
	user_t *prev_ptr;

	while(ptr!=NULL && ptr->userID!=userID){
		prev_ptr = ptr;
		ptr = ptr->next;
	}
	if(ptr==NULL){
		flag = 1;
	}
	else{
		delete_watchHistory(userID);
		if(ptr == user_hashtable_p[index]){
			if(ptr->next!=NULL){
				user_hashtable_p[index] = user_hashtable_p[index]->next;
			}
			else{
				user_hashtable_p[index] = NULL;
			}
		}
		else if(ptr->next==NULL){
			prev_ptr->next = NULL;
		}
		else{
			prev_ptr->next = ptr->next;
			ptr->next = NULL;
		}
		free(ptr);
	}
	for(i=0; i < hashtable_size; i++){
		printf("Chain %d of Users:",counter);
		ptr = user_hashtable_p[i];
		if(ptr!=NULL){
			while(ptr){
				printf("\n    %d", ptr->userID);
				ptr = ptr->next;
			}
		}
		counter++;
		printf("\n");
	}
	if(flag){
		return 0;
	}
	return 1;
 }
 
 /**
 * @brief Add new movie to new release binary tree.
 * Create a node movie and insert it in 'new release' binary tree.
 *
 * @param movieID The new movie identifier
 * @param category The category of the movie
 * @param year The year movie released
 *
 * @return 1 on success
 *         0 on failure
 */

 int add_new_movie(int movieID, int category, int year){
	/* Below there are 2 implementations. One is with the use 
	of binary search tree and the other with the use of AVL tree.
	To use one method, add comments to the other*/

	/* Create new releases tree by using binary search tree */
	if(!NR_TreeInsert(movieID, category, year)){
		return 0;
	}

	/* Create new releases tree by using AVL tree */
	// if(!NR_AVL_TreeInsert(movieID, category, year)){
	// 	return 0;
	// }

	printf("New releases:\n    New releases tree:");
	A_inOrderPrint(new_releases);
	printf("\nDONE\n\n");
	return 1;
 }

  /**
 * @brief Distribute the movies from new release binary tree to the array of categories.
 *
 * @return 0 on success
 *         1 on failure
 */

int counter0 = 0;
int counter1 = 0;
int counter2 = 0;
int counter3 = 0;
int counter4 = 0;
int counter5 = 0;

 int distribute_movies(void){
	 int i;
	 D_Arrays_Init();
	 if(!D_inOrderIteration(new_releases)){
		return 0;
	 }
	 printf("Movie Category Array:");
	 for(i=0;i<6;i++){
		if(i==0){
			create_new_tree(0, 0, counter0-1);
			printf("\n    HORROR: ");
			D_inOrderPrint(categoryArray[0]->movie, 0);
		}
		else if(i==1){
			create_new_tree(1, 0, counter1-1);
			printf("\n    SCIFI: ");
			D_inOrderPrint(categoryArray[1]->movie, 1);
		}
		else if(i==2){
			create_new_tree(2, 0, counter2-1);
			printf("\n    DRAMA: ");
			D_inOrderPrint(categoryArray[2]->movie, 2);
		}
		else if(i==3){
			create_new_tree(3, 0, counter3-1);
			printf("\n    ROMANCE: ");
			D_inOrderPrint(categoryArray[3]->movie, 3);
		}
		else if(i==4){
			create_new_tree(4, 0, counter4-1);
			printf("\n    DOCUMENTARY: ");
			D_inOrderPrint(categoryArray[4]->movie, 4);
		}
		else{
			create_new_tree(5, 0, counter5-1);
			printf("\n    COMEDY: ");
			D_inOrderPrint(categoryArray[5]->movie, 5);
		}
	 }
	 printf("\nDONE\n\n");
	 return 1;
 }
 
 /**
 * @brief User rates the movie with identification movieID with score
 *
 * @param userID The identifier of the user
 * @param category The Category of the movie
 * @param movieID The identifier of the movie
 * @param score The score that user rates the movie with id movieID
 *
 * @return 1 on success
 *         0 on failure
 */

 int watch_movie(int userID, int category, int movieID, int score){
	 int index = hash_function(userID);
	 movie_t *ptr = categoryArray[category]->movie;
     userMovie_t *userMovie;
	 user_t *user_ptr = user_hashtable_p[index];

	 while(user_ptr!=NULL && user_ptr->userID!=userID){
		user_ptr = user_ptr->next;
	 }

	 while(ptr!=categoryArray[category]->sentinel && ptr->movieID!=movieID){
		if(ptr->movieID > movieID){
			ptr = ptr->lc;
		}
		else{
			ptr = ptr->rc;
		}
	 }
	 if(ptr == categoryArray[category]->sentinel){
		return 0;
	 }
	 ptr->watchedCounter++;
	 ptr->sumScore = ptr->sumScore + score;
	 userMovie = malloc(sizeof(userMovie_t));
	 userMovie->movieID = movieID;
	 userMovie->category = category;
	 userMovie->score = score;
	 if(!LeafOrientedTree_Insert(userMovie, movieID, index, userID)){
		return 0;
	 }
	 printf("History tree of user %d:\n",userID);
	 W_inOrderPrint(user_ptr->history);
	 printf("\nDONE\n\n");
	 return 1;
 }
 
/**
 * @brief Identify the best rating score movie and cluster all the movies of a category.
 *
 * @param userID The identifier of the user
 * @param score The minimum score of a movie
 *
 * @return 1 on success
 *         0 on failure
 */

 int gcounter;

 int filter_movies(int userID, int score){
	 int numMovies = 0;
	 int i;
	 movie_t **MovieArray;
	 for(i=0; i<6; i++){
		numMovies = numMovies + catTree_MovieCounter(categoryArray[i]->movie, i, score);
	 }
	 MovieArray = malloc(sizeof(movie_t*)*numMovies);
	 gcounter = 0;

	 for(i=0; i<6; i++){
		MovieArray = SaveMovies(categoryArray[i]->movie, MovieArray, i, score);
	 }

	 MovieArray = HeapSort(MovieArray, numMovies);
	 gcounter = 0;
	 for(i=0;i<numMovies;i++){
		printf("	%d-%d ", MovieArray[i]->movieID, (MovieArray[i]->sumScore/MovieArray[i]->watchedCounter));
	 }
	 printf("\n\nDONE\n");
	 free(MovieArray);
	 return 1;
 }
 
/**
 * @brief Find movies from categories withn median_score >= score t
 *
 * @param userID The identifier of the user
 * @param category Array with the categories to search.
 * @param score The minimum score the movies we want to have
 *
 * @return 1 on success
 *         0 on failure
 */

 int user_stats(int userID){
	 int index = hash_function(userID);
	 int counter = 0;
	 int sumScore = 0;
	 int MScore = 0;
	 user_t *user_ptr = user_hashtable_p[index];
	 userMovie_t *user_history = user_ptr->history;
	 userMovie_t *prev = NULL;
	 userMovie_t *ptr = NULL;
	 while(user_ptr!=NULL && user_ptr->userID!=userID){
		user_ptr = user_ptr->next;
	 }
	 if(user_ptr == NULL){
		return 0;
	 }
     while(user_history!=NULL){
		prev = user_history;
		user_history = user_history->lc;
	 }

	 if(prev!=NULL){
		sumScore = sumScore + prev->score;
	 }
	 else{
		sumScore = sumScore + user_history->score;
	 }

	 counter++;

	 ptr = FindNextLeaf(prev);
	 while(ptr!=NULL){
		sumScore = sumScore + ptr->score;
		counter++;
		ptr = FindNextLeaf(ptr);
	 }
	 
	 if(counter!=0){
	 	MScore = sumScore/counter;
	 }

	 printf("\nQ %d %d", userID, MScore);
     printf("\nDONE\n\n");
	 return 1;
 }
 
/**
 * @brief Search for a movie with identification movieID in a specific category.
 *
 * @param movieID The identifier of the movie
 * @param category The category of the movie
 *
 * @return 1 on success
 *         0 on failure
 */

 int search_movie(int movieID, int category){
	 int i;
	 int flag = 0;
	 movie_t *ptr = categoryArray[category]->movie;
	 while(ptr!=categoryArray[category]->sentinel && ptr->movieID!=movieID){
		if(ptr->movieID > movieID){
			ptr = ptr->lc;
		}
		else{
			ptr = ptr->rc;
		}
	 }
		if(ptr!=categoryArray[category]->sentinel){
			if(category==0){
				printf("I %d HORROR %d : ",ptr->movieID, ptr->year);
			}
			else if(category==1){
				printf("I %d SCIFI %d : ",ptr->movieID, ptr->year);
			}
			else if(category==2){
				printf("I %d DRAMA %d : ",ptr->movieID, ptr->year);
			}
			else if(category==3){
				printf("I %d ROMANCE %d : ",ptr->movieID, ptr->year);
			}
			else if(category==4){
				printf("I %d DOCUMENTARY %d : ",ptr->movieID, ptr->year);
			}
			else{
				printf("I %d COMEDY %d : ",ptr->movieID, ptr->year);
			}
			printf("\nDONE\n\n");
			return 1;
	 }
	 printf("\nMovie with id %d does not exist", movieID);
	 printf("\nDONE\n\n");
	 return 0;
 }
 
 /**
 * @brief Prints the movies in movies categories array.
 * @return 1 on success
 *         0 on failure
 */

 int print_movies(void){
	 int i=0;
	 printf("Movie Category Array:");
	 for(i=0;i<6;i++){
		if(i==0){
			create_new_tree(0, 0, counter0-1);
			printf("\n    HORROR: ");
			D_inOrderPrint(categoryArray[0]->movie, 0);
		}
		else if(i==1){
			create_new_tree(1, 0, counter1-1);
			printf("\n    SCIFI: ");
			D_inOrderPrint(categoryArray[1]->movie, 1);
		}
		else if(i==2){
			create_new_tree(2, 0, counter2-1);
			printf("\n    DRAMA: ");
			D_inOrderPrint(categoryArray[2]->movie, 2);
		}
		else if(i==3){
			create_new_tree(3, 0, counter3-1);
			printf("\n    ROMANCE: ");
			D_inOrderPrint(categoryArray[3]->movie, 3);
		}
		else if(i==4){
			create_new_tree(4, 0, counter4-1);
			printf("\n    DOCUMENTARY: ");
			D_inOrderPrint(categoryArray[4]->movie, 4);
		}
		else{
			create_new_tree(5, 0, counter5-1);
			printf("\n    COMEDY: ");
			D_inOrderPrint(categoryArray[5]->movie, 5);
		}
	 }
	 printf("\nDONE\n\n");
	 return 1;
 }
 
  /**
 * @brief Prints the users hashtable.
 * @return 1 on success
 *         0 on failure
 */

int print_users(void){
	user_t *user_ptr;
	userMovie_t *user_history = NULL;
	userMovie_t *prev = NULL;
	userMovie_t *ptr = NULL;
	int i;

	for(i=0;i<hashtable_size;i++){
		if(user_hashtable_p[i] == NULL){
			continue;
		}
		printf("Chain %d of Users:\n",i);
		user_ptr = user_hashtable_p[i];
		user_history = user_ptr->history;
		while(user_ptr!=NULL){
			printf("	%d\n",user_ptr->userID);
			printf("	History Tree:\n");
			while(user_history!=NULL){
				prev = user_history;
				user_history = user_history->lc;
	 		}
			if(prev!=NULL){
				printf("	   %d %d\n",prev->movieID, prev->score);
				ptr = FindNextLeaf(prev);
				while(ptr!=NULL){
					printf("	   %d %d\n",ptr->movieID, ptr->score);
					ptr = FindNextLeaf(ptr);
				}
			}
			user_ptr = user_ptr->next;
		}
	}
	printf("\nDONE\n\n");
	return 1;
}
 
 /* hash function formula = ((a*userID + b) mod prime_number) mod size_of_hashtable*/
int hash_function(int userID){
	return ((a*userID+b) % prime_num()) % hashtable_size;
}

/* Function that inserts a node in new releases tree */
int NR_TreeInsert(int movieID, int category, int year){
	new_movie_t *ptr = new_releases;
	new_movie_t *prev = NULL;
	new_movie_t *new_node;

	while(ptr!=NULL){
		if(ptr->movieID == movieID){
			return 0;
		}
		prev = ptr;
		if(movieID < ptr->movieID){
			ptr = ptr->lc;
		}
		else{
			ptr = ptr->rc;
		}
	}

	new_node = malloc(sizeof(new_movie_t));
	new_node->category = category;
	new_node->movieID = movieID;
	new_node->year = year;
	new_node->sumScore = 0;
	new_node->watchedCounter = 0;
	new_node->rc = NULL;
	new_node->lc = NULL;

	if(prev == NULL){
		new_releases = new_node;
	}

	else if (movieID < prev->movieID) {
		prev->lc = new_node;
	}
	else{
		prev->rc = new_node;
	}
	return 1;
}

/* Function that prints the movies of the movie category tree after D event */
void D_inOrderPrint(movie_t *ptr, int category){
	if(ptr == categoryArray[category]->sentinel){
		return;
	}
	D_inOrderPrint(ptr->lc, category);
	printf("%d ", ptr->movieID);
	D_inOrderPrint(ptr->rc, category);
}

/* Function that prints the movies of the new releases tree after A event */
void A_inOrderPrint(new_movie_t *new_releases){
	if(new_releases == NULL){
		return;
	}
	A_inOrderPrint(new_releases->lc);
	printf("%d ", new_releases->movieID);
	A_inOrderPrint(new_releases->rc);
}

/* Function that inserts a node into a category tree of movies */
int catArray_TreeInsert(int category, new_movie_t *nm_ptr){
	movie_t *ptr = categoryArray[category]->movie;
	movie_t *prev = NULL;
	movie_t *new_node;

	while(ptr!=categoryArray[category]->sentinel){
		if(ptr->movieID == nm_ptr->movieID){
			return 0;
		}
		prev = ptr;
		if(nm_ptr->movieID < ptr->movieID){
			ptr = ptr->lc;
		}
		else{
			ptr = ptr->rc;
		}
	}

	new_node = malloc(sizeof(movie_t));
	new_node->movieID = nm_ptr->movieID;
	new_node->year = nm_ptr->year;
	new_node->sumScore = 0;
	new_node->watchedCounter = 0;
	new_node->lc = categoryArray[category]->sentinel;
	new_node->rc = categoryArray[category]->sentinel;

	if(prev == NULL){
		categoryArray[category]->movie = new_node;
	}
	else if (nm_ptr->movieID < prev->movieID) {
		prev->lc = new_node;
	}
	else{
		prev->rc = new_node;
	}
	return 1;
}

/* Function that deletes a node in new releases tree */
int NR_TreeDelete(int movieID){
	new_movie_t *ptr = new_releases;
	new_movie_t *prev = NULL;
	new_movie_t *successor;
	new_movie_t *successor_prev;

	while(ptr!=NULL && ptr->movieID!=movieID){
		prev = ptr;
		if(ptr->movieID > movieID){
			ptr = ptr->lc;
		}
		else{
			ptr = ptr->rc;
		}
	}
	
	if(ptr == NULL){
		return 0;
	}

    /* If the node that will be deleted has no childs */
	if (ptr->lc == NULL && ptr->rc == NULL) {
        if(prev!=NULL && prev->lc == ptr){
            prev->lc = NULL;
        }
        else if(prev!=NULL && prev->rc == ptr){
            prev->rc = NULL;
        }
        free(ptr);
    }

	/* If the node that will be deleted has one child */
	else if(ptr->lc == NULL){
		if(prev == NULL){
        	new_releases = ptr->rc;
        }
		else if(prev->rc == ptr){
            prev->rc = ptr->rc;
        }
        else{
            prev->lc = ptr->rc;
        }
		free(ptr);
		ptr = NULL;
	}
	else if(ptr->rc == NULL){
		if(prev == NULL){
            new_releases = ptr->lc;
        }
		else if(prev->lc == ptr){
            prev->lc = ptr->lc;
        }
        else{
            prev->rc = ptr->lc;
        }
		free(ptr);
		ptr = NULL;
	}

	/* If the node that will be deleted has two childs */
	else{
		successor_prev = ptr;
		successor = ptr->rc;
		while(successor->lc!=NULL){
			successor_prev = successor;
			successor = successor->lc;
		}

		if(successor_prev!=ptr){
			successor_prev->lc = successor->rc;
		}
		else{
			successor_prev->rc = successor->rc;
		}

		ptr->movieID = successor->movieID;
		free(successor);
	}
	return 1;
}

/* Funtion that initializes the arrays that contains the movies of each category seperately, in order to insert them into category trees */
int D_inOrderIteration(new_movie_t *new_releases){
	if(new_releases == NULL){
		return 1;
	}

	D_inOrderIteration(new_releases->lc);
	if(new_releases->category == 0){
		HorrorArray[counter0]->category = new_releases->category;
		HorrorArray[counter0]->movieID = new_releases->movieID;
		HorrorArray[counter0]->sumScore = new_releases->sumScore;
		HorrorArray[counter0]->watchedCounter = new_releases->watchedCounter;
		HorrorArray[counter0]->year = new_releases->year;
		counter0++;
	}
	else if(new_releases->category == 1){
		ScifiArray[counter1]->category = new_releases->category;
		ScifiArray[counter1]->movieID = new_releases->movieID;
		ScifiArray[counter1]->sumScore = new_releases->sumScore;
		ScifiArray[counter1]->watchedCounter = new_releases->watchedCounter;
		ScifiArray[counter1]->year = new_releases->year;
		counter1++;
	}
	else if(new_releases->category == 2){
		DramaArray[counter2]->category = new_releases->category;
		DramaArray[counter2]->movieID = new_releases->movieID;
		DramaArray[counter2]->sumScore = new_releases->sumScore;
		DramaArray[counter2]->watchedCounter = new_releases->watchedCounter;
		DramaArray[counter2]->year = new_releases->year;
		counter2++;
	}
	else if(new_releases->category == 3){
		RomanceArray[counter3]->category = new_releases->category;
		RomanceArray[counter3]->movieID = new_releases->movieID;
		RomanceArray[counter3]->sumScore = new_releases->sumScore;
		RomanceArray[counter3]->watchedCounter = new_releases->watchedCounter;
		RomanceArray[counter3]->year = new_releases->year;
		counter3++;
	}
	else if(new_releases->category == 4){
		DocumentaryArray[counter4]->category = new_releases->category;
		DocumentaryArray[counter4]->movieID = new_releases->movieID;
		DocumentaryArray[counter4]->sumScore = new_releases->sumScore;
		DocumentaryArray[counter4]->watchedCounter = new_releases->watchedCounter;
		DocumentaryArray[counter4]->year = new_releases->year;
		counter4++;
	}
	else if(new_releases->category == 5){
		ComedyArray[counter5]->category = new_releases->category;
		ComedyArray[counter5]->movieID = new_releases->movieID;
		ComedyArray[counter5]->sumScore = new_releases->sumScore;
		ComedyArray[counter5]->watchedCounter = new_releases->watchedCounter;
		ComedyArray[counter5]->year = new_releases->year;
		counter5++;
	}
	if(!NR_TreeDelete(new_releases->movieID)){
		return 0;
	}
	D_inOrderIteration(new_releases->rc);
}

/* Function that inserts node into a leaf oriented tree */
int LeafOrientedTree_Insert(userMovie_t *newNode, int movieID, int index, int userID){
	user_t *user_ptr = user_hashtable_p[index];
	userMovie_t *ptr = NULL;
	userMovie_t *prev = NULL;
	userMovie_t *filling = NULL;

	while(user_ptr!=NULL && user_ptr->userID!=userID){
		user_ptr = user_ptr->next;
	}

	if(user_ptr==NULL){
		return 0;
	}
	ptr = user_ptr->history;
	if(ptr == NULL){
		user_ptr->history = newNode;
		return 1;
	}

	while(ptr!=NULL){
		if(ptr->movieID > movieID){
			prev = ptr;
			ptr = ptr->lc;
		}
		else if(ptr->movieID < movieID){
			prev = ptr;
			ptr = ptr->rc;
		}
		else{
			ptr->movieID = movieID;
			return 0;
		}
	}

	filling = malloc(sizeof(userMovie_t));
	filling->movieID = movieID;
	filling->category = newNode->category;
	filling->score = newNode->score;
	filling->lc = NULL;
	filling->rc = NULL;

	if(prev->movieID < movieID){
		prev->rc = newNode;
		prev->lc = filling;
		newNode->parent = prev;
		filling->movieID = prev->movieID;
		filling->category = prev->category;
		filling->score = prev->score;
		filling->parent = prev;
	}
	else if(prev->movieID > movieID){
		newNode->movieID = prev->movieID;
		newNode->category = prev->category;
		newNode->score = prev->score;
		prev->rc = newNode;
		prev->lc = filling;
		prev->movieID = movieID;
		newNode->parent = prev;
		filling->parent = prev;
	}
	return 1;
}

/* Print function for event W*/
void W_inOrderPrint(userMovie_t *ptr){
	if(ptr == NULL){
		return;
	}
	W_inOrderPrint(ptr->lc);
	if(ptr->lc == NULL && ptr->rc == NULL){
		printf("		%d, %d\n", ptr->movieID, ptr->score);
	}
	W_inOrderPrint(ptr->rc);
}

/* Funtion that returns the number of movies of a certain category that exist in new releases tree */
int D_inOrderCounter(new_movie_t *new_releases, int category){
	if(new_releases == NULL){
		return 0;
	}

	if(new_releases->category == category){
		return 1 + D_inOrderCounter(new_releases->lc, category) + D_inOrderCounter(new_releases->rc, category);
	}

	return D_inOrderCounter(new_releases->lc, category) + D_inOrderCounter(new_releases->rc, category);
}

/* Funtion that initializes arrays that contain the movies of each category separately */
void D_Arrays_Init(void){
	int size;
	int i;
	size = D_inOrderCounter(new_releases, 0);
	if(size>0){
		HorrorArray = malloc(size * sizeof(new_movie_t*));
		for(i=0; i<size; i++){
			HorrorArray[i] = malloc(sizeof(new_movie_t));
		}
	}

	size = D_inOrderCounter(new_releases, 1);
	if(size>0){
		ScifiArray = malloc(size * sizeof(new_movie_t*));
		for(i=0; i<size; i++){
			ScifiArray[i] = malloc(sizeof(new_movie_t));
		}
	}

	size = D_inOrderCounter(new_releases, 2);
	if(size>0){
		DramaArray = malloc(size * sizeof(new_movie_t*));
		for(i=0; i<size; i++){
			DramaArray[i] = malloc(sizeof(new_movie_t));
		}
	}

	size = D_inOrderCounter(new_releases, 3);
	if(size>0){
		RomanceArray = malloc(size * sizeof(new_movie_t*));
		for(i=0; i<size; i++){
			RomanceArray[i] = malloc(sizeof(new_movie_t));
		}
	}

	size = D_inOrderCounter(new_releases, 4);
	if(size>0){
		DocumentaryArray = malloc(size * sizeof(new_movie_t*));
		for(i=0; i<size; i++){
			DocumentaryArray[i] = malloc(sizeof(new_movie_t));
		}
	}

	size = D_inOrderCounter(new_releases, 5);
	if(size>0){
		ComedyArray = malloc(size * sizeof(new_movie_t*));
		for(i=0; i<size; i++){
			ComedyArray[i] = malloc(sizeof(new_movie_t));
		}
	}
}

/* Function that help to create the category trees in event D*/
void create_new_tree(int category, int low, int high){
	int mid = 0;
	if(high < low){
		return;
	}
	mid = low + ((high - low)/2);
	if(category == 0){
		catArray_TreeInsert(category, HorrorArray[mid]);
	}
	if(category == 1){
		catArray_TreeInsert(category, ScifiArray[mid]);
	}
	if(category == 2){
		catArray_TreeInsert(category, DramaArray[mid]);
	}
	if(category == 3){
		catArray_TreeInsert(category, RomanceArray[mid]);
	}
	if(category == 4){
		catArray_TreeInsert(category, DocumentaryArray[mid]);
	}
	if(category == 5){
		catArray_TreeInsert(category, ComedyArray[mid]);
	}
	create_new_tree(category, low, mid-1);
	create_new_tree(category, mid+1, high);
}

/* Function that returns the number of movies that have average score > given score */
int catTree_MovieCounter(movie_t *ptr, int category, int score){
	if(ptr == categoryArray[category]->sentinel){
		return 0;
	}

	if(ptr->watchedCounter!=0 && (ptr->sumScore/ptr->watchedCounter) > score){
		return 1 + catTree_MovieCounter(ptr->lc, category, score) + catTree_MovieCounter(ptr->rc, category, score);
	}

	return catTree_MovieCounter(ptr->lc, category, score) + catTree_MovieCounter(ptr->rc, category, score);
}

/* Function that stores the movies that have average score > given score */
movie_t **SaveMovies(movie_t *ptr, movie_t **Array, int category, int score){
	if(ptr == categoryArray[category]->sentinel){
		return Array;
	}

	if(ptr->watchedCounter!=0 && (ptr->sumScore/ptr->watchedCounter) > score){
		Array[gcounter] = ptr;
		gcounter++;
	}
	Array = SaveMovies(ptr->lc, Array, category, score);
	Array = SaveMovies(ptr->rc, Array, category, score);
}

/* Initialize the heap*/
movie_t **InitializeHeap(movie_t **Array, int numMovies){
	int i;
	for(i=1; i<numMovies; i++){
		Array = Heapify(Array, 0, i, numMovies);
	}

	return Array;
}

/* Function that returns the left child of element with index m */
int leftchild(int m, int n){
	return 2*m-n;
}

/* Function that returns the right child of element with index m */
int rightchild(int m, int n){
	return 2*m-n-1;
}

/* Sorts array Array[i...j] */
movie_t **Heapify(movie_t **Array, int i, int j, int n){
	int m = j;
	int p;
	while((leftchild(m,n)>=i && (Array[leftchild(m,n)]->sumScore/Array[leftchild(m,n)]->watchedCounter)<(Array[m]->sumScore/Array[m]->watchedCounter)) || (rightchild(m,n)>=i && (Array[rightchild(m,n)]->sumScore/Array[rightchild(m,n)]->watchedCounter)<(Array[m]->sumScore/Array[m]->watchedCounter))){
		if(rightchild(m,n) >= i){
			if((Array[leftchild(m,n)]->sumScore/Array[leftchild(m,n)]->watchedCounter) < (Array[rightchild(m,n)]->sumScore/Array[rightchild(m,n)]->watchedCounter)){
				p = leftchild(m,n);
			}
			else{
				p = rightchild(m,n);
			}
		}
		else{
			p = i;
		}
		swap(&Array[m], &Array[p]);
		m = p;
	}
	return Array;
}

/* Swap function */
void swap(movie_t **a, movie_t **b){
	movie_t *tmp = *a;
	*a = *b;
	*b = tmp;
}

/* Sorts the whole given array*/
movie_t **HeapSort(movie_t **Array, int numMovies){
	int i;
	Array = InitializeHeap(Array, numMovies);

	for(i=0;i<numMovies-1;i++){
		swap(&Array[i], &Array[numMovies-1]);
		Array = Heapify(Array, i+1, numMovies-1, numMovies);
	}

	return Array;
}

/* Returns the right leaf of leaf v*/
userMovie_t *FindNextLeaf(userMovie_t *v){
	userMovie_t *tmp;
	if(v==NULL){
		return NULL;
	}
	tmp = v->parent;
	while(tmp!=NULL){
		if(tmp->movieID >= v->movieID){
			break;
		}
		tmp = tmp->parent;
	}
	if(tmp==NULL){
		return NULL;
	}
	tmp=tmp->rc;
	while(tmp!=NULL){
		if(tmp->lc==NULL && tmp->rc==NULL){
			return tmp;
		}
		tmp=tmp->lc;
	}
	return NULL;
}

/* Deletes the watch history of user with id = userID*/
void delete_watchHistory(int userID){
	int index = hash_function(userID);
	user_t *ptr = user_hashtable_p[index];
	
	while(ptr!=NULL){
		ptr = ptr->next;
	}
	if(ptr == NULL){
		return;
	}
	watchHistory_inOrder_Iterate(ptr->history, index, userID);
}

/* Functions that helps with the traverse of watch history tree */
void watchHistory_inOrder_Iterate(userMovie_t *ptr, int index, int userID){
	if(ptr==NULL){
		return;
	}
	watchHistory_inOrder_Iterate(ptr->lc, index, userID);
	LeafOrientedTree_Delete(ptr->movieID, index, userID);
	watchHistory_inOrder_Iterate(ptr->rc, index, userID);
}

/* Deletes a node from a leaf oriented tree */
int LeafOrientedTree_Delete(int movieID, int index, int userID){
	user_t *ptr = user_hashtable_p[index];
	userMovie_t *m_ptr = NULL;
	userMovie_t *prev_ptr = NULL;

	while(ptr!=NULL){
		ptr = ptr->next;
	}
	if(ptr == NULL){
		return 0;
	}

	m_ptr = ptr->history;
	while(ptr!=NULL){
		if(m_ptr->movieID==movieID && m_ptr->lc==NULL && m_ptr->rc==NULL){
			prev_ptr->lc = NULL;
			prev_ptr->rc = NULL;
			free(m_ptr);
			return 1;
		}
		else if(m_ptr->lc!=NULL && m_ptr->lc->movieID == movieID && m_ptr->lc->lc == NULL && m_ptr->lc->lc==NULL){
			if(prev_ptr->lc->movieID==movieID){
				if(m_ptr->lc->movieID!=movieID){
					prev_ptr->lc=m_ptr->lc;
				}
				else if(m_ptr->rc->movieID!=movieID){
					prev_ptr->lc=m_ptr->rc;
				}
			}
		}
	}
}

/* Calculates the height of node */
int height(new_movie_t *ptr){
	int leftheight = 0;
	int rightheight = 0;

	if(ptr == NULL){
		return -1;
	}

	leftheight = height(ptr->lc);
	rightheight = height(ptr->rc);

	if(leftheight>rightheight){
		return 1+leftheight;
	}

	return 1+rightheight;
}

/* Calculates the balance of node */
int balance(new_movie_t *ptr){
	if(ptr == NULL){
		return 0;
	}

	return height(ptr->rc) - height(ptr->lc);
}

 /* Performs left rotation */
 new_movie_t *leftRotate(new_movie_t* ptr){
	new_movie_t *ptr_rc;
	new_movie_t *ptr_lc;

	if (ptr == NULL || ptr->lc == NULL) {
        return ptr;
    }

    ptr_rc = ptr->rc;
    ptr_lc = ptr_rc->lc;

    if (ptr_rc == NULL) {
        return ptr;
    }

	ptr_rc->lc = ptr;
	ptr->rc = ptr_lc;

	return ptr_rc;
 }

 /* Performs right rotation */
new_movie_t *rightRotate(new_movie_t* ptr){
	new_movie_t *ptr_lc;
	new_movie_t *ptr_lc_rc;

    if (ptr == NULL || ptr->lc == NULL) {
        return ptr;
    }

    ptr_lc = ptr->lc;
    ptr_lc_rc = ptr_lc->rc;

    if (ptr_lc == NULL) {
        return ptr;
    }

    ptr_lc->rc = ptr;
    ptr->lc = ptr_lc_rc;

    return ptr_lc;
}

/* Inserts a node into an AVL tree */
int NR_AVL_TreeInsert(int movieID, int category, int year){
	if(!NR_TreeInsert(movieID, category, year)){
		return 1;
	}

	new_releases = doRotations(new_releases, movieID);

	return 1; 
}

/* Performs the rotations after the addition of a node on the tree */
new_movie_t *doRotations(new_movie_t *ptr, int movieID) {
    int ptr_balance;

    if(ptr == NULL) {
        return NULL;
    }

    if(ptr->lc != NULL) {
        ptr->lc = doRotations(ptr->lc, movieID);
    }

    if(ptr->rc != NULL) {
        ptr->rc = doRotations(ptr->rc, movieID);
    }

    ptr_balance = balance(ptr);

    if (ptr_balance > 1 && ptr->lc != NULL && movieID < ptr->lc->movieID) {
        ptr = rightRotate(ptr);
    }

    else if (ptr_balance < -1 && ptr->rc != NULL && movieID > ptr->rc->movieID) {
        ptr = leftRotate(ptr);
    }

    else if (ptr_balance > 1 && ptr->lc != NULL && movieID > ptr->lc->movieID) {
        if (ptr->lc != NULL) {
            ptr->lc = leftRotate(ptr->lc);
        }
        ptr = rightRotate(ptr);
    }

    else if (ptr_balance < -1 && ptr->rc != NULL && movieID < ptr->rc->movieID) {
        if (ptr->rc != NULL) {
            ptr->rc = rightRotate(ptr->rc);
        }
        ptr = leftRotate(ptr);
    }

    return ptr;
}