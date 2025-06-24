/*****************************************************
 * @file   Movie.h                                    *
 * @author Paterakis Giorgos <geopat@csd.uoc.gr>     *
 *                                                   *
 * @brief Header File for Data Structures (CS240b)   *
 * Project: Winter 2023						         *
 *****************************************************/
#ifndef MOVIE_FILE
#define MOVIE_FILE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>

/*You can use this enum or you can choose to represent each category as an Integer. Its the same*/
typedef enum {
	HORROR,
	SCIFI,
	DRAMA,
	ROMANCE,
	DOCUMENTARY,
	COMEDY
} Category_t;

/**
* Structure defining a node of a new_realeases binary tree (dendro newvn kykloforiwn)
*/
typedef struct new_movie{
	int category;				/* The category of the movie*/
	int movieID; 				/* The movie identifier*/
	int year;					/* The year movie released*/
	int watchedCounter;	 		/* How many users rate the movie*/
	int sumScore;				/* The sum of the ratings of the movie*/
	struct new_movie *lc;	/* Pointer to the node's left child*/
	struct new_movie *rc;	/* Pointer to the node's right child*/
}new_movie_t;


/**
 *Structure defining a node of movie binary tree (dendro tainiwn kathgorias)
*/
typedef struct movie{
	int movieID; 				/* The movie identifier*/
	int year;					/* The year movie released*/
	int watchedCounter;	 		/* How many users rate the movie*/
	int sumScore;				/* The sum of the ratings of the movie*/
	struct movie *lc;	/* Pointer to the node's left child*/
	struct movie *rc;	/* Pointer to the node's right child*/
}movie_t;

/**
* Structure defining movie_category
*/

typedef struct movie_category{
	movie_t *movie;
	movie_t *sentinel;
}movieCategory_t;

/**
* Structure defining a node of user_movie for history doubly linked binary 
* tree (dentro istorikou) 
*/
typedef struct user_movie{
	int movieID; 					/* The movie identifier*/
	int category;					/* The category of the movie*/
	int score;						/* The score of the movie*/
	struct user_movie *parent;		/* Pointer to the node's parent*/
	struct user_movie *lc;	/* Pointer to the node's left child*/
	struct user_movie *rc;	/* Pointer to the node's right child*/
}userMovie_t;

/**
* Structure defining a node of users' hashtable (pinakas katakermatismou
* xrhstwn)
*/
typedef struct user {
	int userID; 			/* The user's identifier*/
	userMovie_t *history;	/* A doubly linked binary tree with the movies watched by the user*/
	struct user *next;		/* Pointer to the next node of the chain*/	
}user_t;

extern movieCategory_t *categoryArray[6];  /* The categories array (pinakas kathgoriwn)*/
extern new_movie_t *new_releases;    		   /* New releases simply-linked binary tree*/
extern user_t **user_hashtable_p;		   /* The users hashtable. This is an array of chains (pinakas katakermatismoy xrhstwn)*/
extern int hashtable_size;				   /* The size of the users hashtable, parsed from the command line (>0)*/
extern int max_users;					   /* The maximum number of registrations (users)*/			
extern int max_id;   			   /* The maximum account ID */
extern int primes_g[170];

extern new_movie_t **HorrorArray;
extern new_movie_t **ScifiArray;
extern new_movie_t **DramaArray;
extern new_movie_t **RomanceArray;
extern new_movie_t **DocumentaryArray;
extern new_movie_t **ComedyArray;

extern int a,b;

int prime_num(void);

/**
 * @brief Creates a new user.
 * Creates a new user with userID as its identification.
 *
 * @param userID The new user's identification
 *
 * @return 1 on success
 *         0 on failure
 */

 int register_user(int userID);
 
/**
 * @brief Deletes a user.
 * Deletes a user with userID from the system, along with users' history tree.
 *
 * @param userID The new user's identification
 *
 * @return 1 on success
 *         0 on failure
 */

 int unregister_user(int userID);
 
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

 int add_new_movie(int movieID, int category, int year);
 
 /**
 * @brief Distribute the movies from new release binary tree to the array of categories.
 *
 * @return 1 on success
 *         0 on failure
 */

 int distribute_movies(void);
 
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

 int watch_movie(int userID, int category,int movieID, int score);
 
/**
 * @brief Identify the best rating score movie and cluster all the movies of a category.
 *
 * @param userID The identifier of the user
 * @param score The minimum score of a movie
 *
 * @return 1 on success
 *         0 on failure
 */

 int filter_movies(int userID, int score);
 
/**
 * @brief Find the median score that user rates movies.
 *
 * @param userID The identifier of the user
 *
 * @return 1 on success
 *         0 on failure
 */

 int user_stats(int userID);
 
/**
 * @brief Search for a movie with identification movieID in a specific category.
 *
 * @param movieID The identifier of the movie
 * @param category The category of the movie
 *
 * @return 1 on success
 *         0 on failure
 */

 int search_movie(int movieID, int category);
 
 /**
 * @brief Prints the movies in movies categories array.
 * @return 1 on success
 *         0 on failure
 */

 int print_movies(void);
 
  /**
 * @brief Prints the users hashtable.
 * @return 1 on success
 *         0 on failure
 */

 int print_users(void);
 
 /* hash function formula = ((a*userID + b) mod prime_number) mod size_of_hashtable*/
 int hash_function(int userID);

 /* Function that inserts a node in new releases tree */
 int NR_TreeInsert(int movieID, int category, int year);

 /* Function that prints the movies of the movie category tree after D event */
 void D_inOrderPrint(movie_t *ptr, int category);
  
 /* Function that prints the movies of the new releases tree after A event */
 void A_inOrderPrint(new_movie_t *new_releases);

 /* Function that inserts a node into a category tree of movies */
 int catArray_TreeInsert(int movieID, new_movie_t *ptr);

 /* Function that deletes a node in new releases tree */
 int NR_TreeDelete(int movieID);

 /* Funtion that initializes the arrays that contains the movies of each category seperately, in order to insert them into category trees */
 int D_inOrderIteration(new_movie_t *new_releases); 
 
 /* Function that inserts node into a leaf oriented tree */
 int LeafOrientedTree_Insert(userMovie_t *newNode, int movieID, int index, int userID);

 /* Print function for event W*/
 void W_inOrderPrint(userMovie_t *ptr);

 /* Funtion that initializes arrays that contain the movies of each category separately */
 void D_Arrays_Init(void);

 /* Function that help to create the category trees in event D*/
 void create_new_tree(int category, int low, int high);

 /* Funtion that returns the number of movies of a certain category that exist in new releases tree */
 int D_inOrderCounter(new_movie_t *new_releases, int category);
 
 /* Function that returns the number of movies that have average score > given score */
 int catTree_MovieCounter(movie_t *ptr, int category, int score);
 
 /* Function that stores the movies that have average score > given score */
 movie_t **SaveMovies(movie_t *ptr, movie_t **Array, int category, int score);
 
 /* Initialize the heap*/
 movie_t **InitializeHeap(movie_t **Array, int numMovies);
 
 /* Sorts array Array[i...j] */
 movie_t **Heapify(movie_t **Array, int i, int j, int n);

 /* Swap function */
 void swap(movie_t **a, movie_t **b);

 /* Sorts the whole given array*/
 movie_t **HeapSort(movie_t **Array, int numMovies);

 /* Returns the right leaf of leaf v*/
 userMovie_t *FindNextLeaf(userMovie_t *v);

 /* Deletes the watch history of user with id = userID*/
 void delete_watchHistory(int userID);

 /* Functions that helps with the traverse of watch history tree */
 void watchHistory_inOrder_Iterate(userMovie_t *ptr, int index, int userID);

 /* Deletes a node from a leaf oriented tree */
 int LeafOrientedTree_Delete(int movieID, int index, int userID);

 /* Function that returns the left child of element with index m */
 int leftchild(int m, int n);

 /* Function that returns the right child of element with index m */
 int rightchild(int m, int n);

 /* Calculates the height of node */
 int height(new_movie_t *ptr);

 /* Calculates the balance of node */
 int balance(new_movie_t *ptr);

 /* Performs left rotation */
 new_movie_t *leftRotate(new_movie_t* ptr);

 /* Performs right rotation */
 new_movie_t *rightRotate(new_movie_t* ptr);
 
 /* Inserts a node into an AVL tree */
 int NR_AVL_TreeInsert(int movieID, int category, int year);

 /* Performs the rotations after the addition of a node on the tree */
 new_movie_t *doRotations(new_movie_t *ptr, int movieID);

 #endif
