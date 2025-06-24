#include <stdio.h>
#include <stdlib.h>
#include "streaming_service.h"

struct movie *CategoryTable[6];
struct new_movie *nm_head = NULL;
struct user *S;
struct user *u_head;

/* Initialize sentinel value and add it to the user list */
void user_list_init(){
    S = malloc(sizeof(struct user));
    S->uid = -1;
    S->next = NULL;
    u_head = S;
}

/*
 * Register User - Event R
 * 
 * Adds user with ID uid to
 * users list, as long as no
 * other user with the same uid
 * already exists.
 *
 * Returns 0 on success, -1 on
 * failure (user ID already exists,
 * malloc or other error)
 */
int register_user(int uid){
    struct user *new_user = malloc(sizeof(struct user));
    struct user *ptr = u_head;
    int flag = 0; /* Is used to checks if user with id = uid already exists */

    /* If list is empty, then allocate memory for a new user*/
    if(u_head==S){
        new_user->uid = uid;
        new_user->suggestedHead = NULL;
        new_user->suggestedTail = NULL;
        new_user->watchHistory = NULL;
        new_user->next = S;
        u_head = new_user;
    }
    else{
        /* Check in user list, if user with id=uid already exists (flag = 1 if exists)*/
        while(ptr!=S){
            if(ptr->uid==uid){
                flag = 1;
                break;
            }
            ptr = ptr->next;
        }
        /* If user does not exists, then allocate memory for the creation of a new user*/
        if(flag == 0){
            new_user->uid = uid;
            new_user->suggestedHead = NULL;
            new_user->suggestedTail = NULL;
            new_user->watchHistory = NULL;
            new_user->next = u_head;
            u_head = new_user;
        }
    }
    /* These lines of code print the list of the users */
    printf("\nR %d\n",uid);
    printf("    Users =  ");
    ptr = u_head;
    while(ptr!=S){
        if(ptr->next==S){
            printf("%d",ptr->uid);
        }
        else{
            printf("%d,",ptr->uid);
        }
        ptr=ptr->next;
    }
    printf("\nDONE\n");
    if(flag == 1){
        free(new_user);
        return -1;
    }
    return 0;
}

/*
 * Unregister User - Event U
 *
 * Removes user with ID uid from
 * users list, as long as such a
 * user exists, after clearing the
 * user's suggested movie list and
 * watch history stack
 */
void unregister_user(int uid){
        struct user *ptr = u_head;
        struct user *tmp_ptr = NULL;
        struct movie *pWh = NULL;
        struct suggested_movie *sm_ptr = NULL;

        while(ptr!=S){
            /* Check if user with id = uid exists */
            if(ptr->uid==uid){
                /* Free all the movies that suggested movies list contains */
                while(ptr->suggestedHead!=NULL){
                    sm_ptr = ptr->suggestedHead;
                    ptr->suggestedHead = ptr->suggestedHead->next;
                    free(sm_ptr);
                }
                /* Pop all the movies that watch history stack contains */
                while(ptr->watchHistory){
                    pWh = ptr->watchHistory;
                    ptr->watchHistory = ptr->watchHistory->next;
                    free(pWh);
                }
                /* Removing the user from the list of users */
                if(ptr == u_head){ /* If the user is at the head of the list */
                    if(ptr->next!=NULL){ /* If there are no other users */
                        u_head = u_head->next;
                    }
                }
                else if(ptr->next == NULL){ /* If the user is at the last node of the list */
                    tmp_ptr->next = NULL;
                }
                else{
                    tmp_ptr->next = ptr->next;
                    ptr->next = NULL;
                }
                free(ptr);
                break;
            }
            tmp_ptr=ptr;
            ptr=ptr->next;
        }
        /* These lines of code print the list of the users */
        printf("\nU %d\n",uid);
        printf("    Users =  ");
        ptr = u_head;
        while(ptr!=S){
            if(ptr->next==S){
                printf("%d",ptr->uid);
            }
            else{
                printf("%d,",ptr->uid);
            }
            ptr=ptr->next;
        }
        printf("\nDONE\n");
}

/*
 * Add new movie - Event A
 *
 * Adds movie with ID mid, category
 * category and release year year
 * to new movies list. The new movies
 * list must remain sorted (increasing
 * order based on movie ID) after every
 * insertion.
 *
 * Returns 0 on success, -1 on failure
 */
int add_new_movie(unsigned mid, movieCategory_t category, unsigned year){
    struct new_movie *p = malloc(sizeof(struct new_movie));
    struct new_movie *ptr = nm_head;
    struct new_movie *tmp_ptr = NULL;
    int flag = 0;
    p->info.mid=mid;
    p->info.year=year;
    p->category = category;
    if(nm_head==NULL){ /* If the new movies list is empty */
        nm_head = p;
        p->next = NULL;
    }
    else{
	/* Looking for the point in the list where the new tape will be placed */
        while((ptr!=NULL) && (ptr->info.mid < mid)){
            tmp_ptr = ptr;
            ptr=ptr->next;
        }

        /* Check for duplicate */
        if(ptr!=NULL && ptr->info.mid == mid){
            flag = 1;
        }

        if(flag == 0){
            if(ptr == NULL){ /* Éf the movie to be added has a bigger id than the others in the list */
                tmp_ptr->next = p;
                p->next = NULL;
            }
            else{
                if(ptr == nm_head){ /* Éf the movie to be added has a smaller id than the others in the list */
                    p->next = ptr;
                    nm_head = p;
                }
                else{
                    p->next = ptr;
                    tmp_ptr->next = p;
                }
            }
        }
    }
    /* Print the list with the new movies */
    printf("\nA %d %s %d\n",mid, CategoryAsString(category), year);
    printf("    New movies = ");
    ptr = nm_head;
    while(ptr){
        if(ptr->next == NULL){
            printf("<%d,%s,%d>",ptr->info.mid,CategoryAsString(ptr->category),ptr->info.year);
        }
        else{
            printf("<%d,%s,%d>, ",ptr->info.mid,CategoryAsString(ptr->category),ptr->info.year);
        }
        ptr = ptr->next;
    }
    printf("\nDONE\n");
    if(flag == 1){
        free(p);
        return 1;
    }
    return 0;
}

/*
 * Distribute new movies - Event D
 *
 * Distributes movies from the new movies
 * list to the per-category sorted movie
 * lists of the category list array. The new
 * movies list should is empty after this
 * event.
 */
void distribute_new_movies(void){
    struct new_movie *p = nm_head;
    struct movie *pCT0 = CategoryTable[0];
    struct movie *pCT1 = CategoryTable[1];
    struct movie *pCT2 = CategoryTable[2];
    struct movie *pCT3 = CategoryTable[3];
    struct movie *pCT4 = CategoryTable[4];
    struct movie *pCT5 = CategoryTable[5];
    struct movie *tmp;
    int counter;

    while(p!=NULL){
        /* Choosing the category to which the film belongs */
        switch (p->category){
            /* Allocating memory for a movie and placing it in the HORROR category of the category table*/
            case HORROR:
                tmp = malloc(sizeof(struct movie));
                tmp->info.mid = p->info.mid;
                tmp->info.year = p->info.year;
                tmp->next=NULL;
                if(CategoryTable[0] == NULL){
                    CategoryTable[0] = tmp;
                    pCT0 = CategoryTable[0];
                }
                else{
                    pCT0->next = tmp;
                    pCT0=pCT0->next;
                }
                break;
            /* Allocating memory for a movie and placing it in the SCIFI category of the category table*/
            case SCIFI:
                tmp = malloc(sizeof(struct movie));
                tmp->info.mid = p->info.mid;
                tmp->info.year = p->info.year;
                tmp->next=NULL;
                if(CategoryTable[1] == NULL){
                    CategoryTable[1] = tmp;
                    pCT1 = CategoryTable[1];
                }
                else{
                    pCT1->next = tmp;
                    pCT1=pCT1->next;
                }
                break;
            /* Allocating memory for a movie and placing it in the DRAMA category of the category table*/
            case DRAMA:
                tmp = malloc(sizeof(struct movie));
                tmp->info.mid = p->info.mid;
                tmp->info.year = p->info.year;
                tmp->next=NULL;
                if(CategoryTable[2] == NULL){
                    CategoryTable[2] = tmp;
                    pCT2 = CategoryTable[2];
                }
                else{
                    pCT2->next = tmp;
                    pCT2=pCT2->next;
                }
                break;
            /* Allocating memory for a movie and placing it in the ROMANCE category of the category table*/
            case ROMANCE:
                tmp = malloc(sizeof(struct movie));
                tmp->info.mid = p->info.mid;
                tmp->info.year = p->info.year;
                tmp->next=NULL;
                if(CategoryTable[3] == NULL){
                    CategoryTable[3] = tmp;
                    pCT3 = CategoryTable[3];
                }
                else{
                    pCT3->next = tmp;
                    pCT3=pCT3->next;
                }
                break;
            /* Allocating memory for a movie and placing it in the DOCUMENTARY category of the category table*/
            case DOCUMENTARY:
                tmp = malloc(sizeof(struct movie));
                tmp->info.mid = p->info.mid;
                tmp->info.year = p->info.year;
                tmp->next=NULL;
                if(CategoryTable[4] == NULL){
                    CategoryTable[4] = tmp;
                    pCT4 = CategoryTable[4];
                }
                else{
                    pCT4->next = tmp;
                    pCT4=pCT4->next;
                }
                break;
            /* Allocating memory for a movie and placing it in the COMEDY category of the category table*/
            case COMEDY:
                tmp = malloc(sizeof(struct movie));
                tmp->info.mid = p->info.mid;
                tmp->info.year = p->info.year;
                tmp->next=NULL;
                if(CategoryTable[5] == NULL){
                    CategoryTable[5] = tmp;
                    pCT5 = CategoryTable[5];
                }
                else{
                    pCT5->next = tmp;
                    pCT5=pCT5->next;
                }
                break;
            default:
                break;
        }
        p=p->next;
    }
    /* These lines of code print every category of the Category Table with the movies that it contains */
    printf("\nD\nCategorized Movies:");
    pCT0 = CategoryTable[0];
    pCT1 = CategoryTable[1];
    pCT2 = CategoryTable[2];
    pCT3 = CategoryTable[3];
    pCT4 = CategoryTable[4];
    pCT5 = CategoryTable[5];
    printf("\n    Horror: ");
    counter = 1;
    while(pCT0){
        if(pCT0->next == NULL){
            printf("<%d, %d>",pCT0->info.mid,counter);
        }
        else{
            printf("<%d, %d>, ",pCT0->info.mid,counter);
        }
        counter++;
        pCT0 = pCT0->next;
    }

    printf("\n    Sci-fi: ");
    counter = 1;
    while(pCT1){
        if(pCT1->next == NULL){
            printf("<%d, %d>",pCT1->info.mid,counter);
        }
        else{
            printf("<%d, %d>, ",pCT1->info.mid,counter);
        }
        counter++;
        pCT1 = pCT1->next;
    }

    printf("\n    Drama: ");
    counter = 1;
    while(pCT2){
        if(pCT2->next == NULL){
            printf("<%d, %d>",pCT2->info.mid,counter);
        }
        else{
            printf("<%d, %d>, ",pCT2->info.mid,counter);
        }
        counter++;
        pCT2 = pCT2->next;
    }

    printf("\n    Romance: ");
    counter = 1;
    while(pCT3){
        if(pCT3->next == NULL){
            printf("<%d, %d>",pCT3->info.mid,counter);
        }
        else{
            printf("<%d, %d>, ",pCT3->info.mid,counter);
        }
        counter++;
        pCT3 = pCT3->next;
    }

    printf("\n    Documentary: ");
    counter = 1;
    while(pCT4){
        if(pCT4->next == NULL){
            printf("<%d, %d>",pCT4->info.mid,counter);
        }
        else{
            printf("<%d, %d>, ",pCT4->info.mid,counter);
        }
        counter++;
        pCT4 = pCT4->next;
    }

    printf("\n    Comedy: ");
    counter = 1;
    while(pCT5){
        if(pCT5->next == NULL){
            printf("<%d, %d>",pCT5->info.mid,counter);
        }
        else{
            printf("<%d, %d>, ",pCT5->info.mid,counter);
        }
        counter++;
        pCT5 = pCT5->next;
    }
    printf("\nDONE\n");
}

/*
 * User watches movie - Event W
 *
 * Adds a new struct movie with information
 * corresponding to those of the movie with ID
 * mid to the top of the watch history stack
 * of user uid.
 *
 * Returns 0 on success, -1 on failure
 * (user/movie does not exist, malloc error)
 */
int watch_movie(int uid, unsigned mid){
    struct user *pUser = u_head;
    struct movie *pMovie;
    struct movie *tmp;
    int i;
    int flag = 0;

    /* Check if user with id = uid exists (if not, set flag to 1)*/
    while(pUser!=S && pUser->uid!=uid){
        pUser=pUser->next;
    }
    if(pUser==S){
        flag = 1;
    }

    if(flag == 0){
        /* Check if movie with id = mid exists (if not, set flag to 1)*/
        for(i=0; i<6; i++){
            pMovie = CategoryTable[i];
            while(pMovie!=NULL){
                if(pMovie->info.mid==mid){
                    break;
                }
                pMovie=pMovie->next;
            }
            if(pMovie!=NULL && pMovie->info.mid==mid){
                break;
            }
        }
        if(pMovie == NULL){
            flag = 1;
        }
    }

    if(flag == 0){
        /* Push movie at WatchHistory Stack */
        tmp = malloc(sizeof(struct movie));
        tmp->info.mid = pMovie->info.mid;
        tmp->info.year = pMovie->info.year;

        if(pUser->watchHistory == NULL){
            pUser->watchHistory = tmp;
            tmp->next = NULL;
        }
        else{
            tmp->next = pUser->watchHistory;
            pUser->watchHistory = tmp;
        }
    }
    /* Print the watch history of the user with id = uid*/
    printf("\nW %d %d",uid,mid);
    printf( "\n   User %d Watch History = ",uid);
    tmp = pUser->watchHistory;
    while(tmp){
        if(tmp->next == NULL){
            printf("%d",tmp->info.mid);
        }
        else{
            printf("%d, ",tmp->info.mid);
        }
        tmp = tmp->next;
    }
    printf("\nDONE\n");
    if(flag == 1){
        return 1;
    }
    return 0;
}

/*
 * Suggest movies to user - Event S
 *
 * For each user in the users list with
 * id != uid, pops a struct movie from the
 * user's watch history stack, and adds a
 * struct suggested_movie to user uid's
 * suggested movies list in alternating
 * fashion, once from user uid's suggestedHead
 * pointer and following next pointers, and
 * once from user uid's suggestedTail pointer
 * and following prev pointers.
 *
 * Returns 0 on success, -1 on failure
 */
int suggest_movies(int uid){
    struct user *pUser = u_head;
    struct user *iterateUser;
    struct suggested_movie *phead = NULL;
    struct suggested_movie *ptail = NULL;
    struct suggested_movie *node;
    struct movie *moviePop_ptr;
    int flag = 0;
    int turn = 0; /* turn = 0 if the addition will take place on the front, turn = 1 else */

    /* Check if user with id = uid exists (if not, then flag = 1)*/
    while(pUser!=S && pUser->uid!=uid){
        pUser=pUser->next;
    }
    if(pUser==S){
        flag = 1;
    }

    if(flag == 0){
        /* Start the addition in movie suggestions list */
        iterateUser = u_head;
        /* If the iterator equals the user with id = uid, then continue to the next loop */
        while(iterateUser!=S){
            if(iterateUser == pUser){
                iterateUser = iterateUser->next;
                continue;
            }
            if(iterateUser->watchHistory!=NULL){
                /* Memory allocation for a suggested movie */
                node = malloc(sizeof(struct suggested_movie));
                node->info.mid = iterateUser->watchHistory->info.mid;
                node->info.year = iterateUser->watchHistory->info.year;
                
                /* Pop movie from WatchHistory stack */
                moviePop_ptr = iterateUser->watchHistory;
                iterateUser->watchHistory = iterateUser->watchHistory->next;
                free(moviePop_ptr);
            }
            else{
                iterateUser = iterateUser->next;
                continue;
            }

            if(turn == 0){
                /* Added the movie by suggestedHead's side */
                if(phead == NULL){ /* If the list is empty */
                    pUser->suggestedHead = node;
                    pUser->suggestedTail = node;
                    phead = pUser->suggestedHead;
                    node->next = NULL;
                    node->prev = NULL;
                }
                else{
                    phead->next = node;
                    node->prev = phead;
                    node->next = NULL;
                    phead=phead->next;
                }
            }
            else{
                /* Added the movie by suggestedTails's side */
                if(ptail == NULL){ /* If the list is empty */
                    pUser->suggestedTail = node;
                    ptail = pUser->suggestedTail;            
                    node->next = NULL;
                    node->prev = NULL;
                }
                else{
                    ptail->prev = node;
                    node->next = ptail;
                    node->prev = NULL;
                    ptail = ptail->prev;
                }
            }
            turn = changeTurn(turn);
            iterateUser = iterateUser->next;
        }
        /* Union of the two lists */
        if(phead!=NULL && ptail!=NULL){
            phead->next=ptail;
            ptail->prev=phead;
        }
    }
    /* Print the suggested movies of the user with id = uid */
    phead = pUser->suggestedHead;
    printf("\nS %d",uid);
    printf("\n  User %d Suggested Movies = ",uid);
    while (phead){
        if(phead->next == NULL){
            printf("%d",phead->info.mid);
        }
        else{
            printf("%d, ",phead->info.mid);
        }
        phead = phead->next;
    }
    printf("\nDONE\n");
    if(flag == 1){
        return -1;
    }
    return 0;
}

/*
 * Filtered movie search - Event F
 *
 * User uid asks to be suggested movies
 * belonging to either category1 or category2
 * and with release year >= year. The resulting
 * suggested movies list is sorted with
 * increasing order based on movie ID (as the
 * two category lists).
 *
 * Returns 0 on success, -1 on failure
 */
int filtered_movie_search(int uid, movieCategory_t category1,movieCategory_t category2, unsigned year){
    struct user *pUser = u_head;
    struct movie *pC1 = CategoryTable[category1];
    struct movie *pC2 = CategoryTable[category2];
    struct suggested_movie *sm_head = NULL;
    struct suggested_movie *sm_tail = NULL;
    struct suggested_movie *tmp;
    int counter = 1;
    int flag = 0;

    /* Search for user (if not found, then flag = 1)*/
    while(pUser != S && pUser->uid != uid){
        pUser = pUser->next;
    }
    if(pUser == S){
        flag = 1;
    }

    if(flag == 0){
        /* Creation of suggested movies (of categories: category1 and category2) list */
        while(pC1!=NULL && pC2!=NULL){
            /* While lists of the 2 categories are not empty, keep adding movies to the new filtered movie list */
            if(pC1->info.mid < pC2->info.mid){
                if(pC1->info.year >= year){
                    tmp = malloc(sizeof(struct suggested_movie));
                    tmp->info.mid = pC1->info.mid;
                    tmp->info.year = pC1->info.year;

                    if(sm_head == NULL){
                        sm_head = tmp;
                        sm_tail = tmp;
                        tmp->prev = NULL;
                        tmp->next = NULL;
                    }
                    else{
                        tmp->prev = sm_tail;
                        sm_tail->next = tmp;
                        sm_tail = tmp;
                    }
                }
                pC1 = pC1->next;
            }
            else{
                if(pC2->info.year >= year){
                    tmp = malloc(sizeof(struct suggested_movie));
                    tmp->info.mid = pC2->info.mid;
                    tmp->info.year = pC2->info.year;

                    if(sm_head == NULL){
                        sm_head = tmp;
                        sm_tail = tmp;
                        tmp->prev = NULL;
                        tmp->next = NULL;
                    }
                    else{
                        tmp->prev = sm_tail;
                        sm_tail->next = tmp;
                        sm_tail = tmp;
                    }
                }
                pC2 = pC2->next;
            }
        }
        if(pC1 == NULL){
            /* If the list of the category1 got empty, keep adding movies from category2 */
            while(pC2){
                if(pC2->info.year >= year){
                    tmp = malloc(sizeof(struct suggested_movie));
                    tmp->info.mid = pC2->info.mid;
                    tmp->info.year = pC2->info.year;
                    if(sm_head == NULL){
                        sm_head = tmp;
                        sm_tail = tmp;
                        tmp->prev = NULL;
                        tmp->next = NULL;
                    }
                    else{
                        tmp->prev = sm_tail;
                        sm_tail->next = tmp;
                        sm_tail = tmp;
                    }
                }
                pC2 = pC2->next;
            }
        }
        else{
            while(pC1){
                /* If the list of the category2 got empty, keep adding movies from category1 */
                if(pC1->info.year >= year){
                    tmp = malloc(sizeof(struct suggested_movie));
                    tmp->info.mid = pC1->info.mid;
                    tmp->info.year = pC1->info.year;
                    if(sm_head == NULL){
                        sm_head = tmp;
                        sm_tail = tmp;
                        tmp->prev = NULL;
                        tmp->next = NULL;
                    }
                    else{
                        tmp->prev = sm_tail;
                        sm_tail->next = tmp;
                        sm_tail = tmp;
                    }
                }
                pC1 = pC1->next;
            }
        }
        /* If the initial suggested list is empty, then set the suggested list as the new created suggested list */
        if(pUser->suggestedHead == NULL){
            pUser->suggestedHead = sm_head;
            pUser->suggestedTail = sm_tail;
        }
        /* If the initial suggested list is not empty, then add to the old the suggested list, the new created suggested list */
        else{
            pUser->suggestedTail->next = sm_head;
            sm_head->prev = pUser->suggestedTail;
            pUser->suggestedTail = sm_tail;
        }
    }
    /* Print user's with id = uid suggested movies list  */
    printf("\nF %d %s %s %d",uid,CategoryAsString(category1),CategoryAsString(category2),year);
    printf("\n    User %d Suggested Movies = ",pUser->uid);
    tmp = pUser->suggestedHead;
    while(tmp){
        if(tmp->next == NULL){
            printf("<%d,%d>",tmp->info.mid,counter);
        }
        else{
            printf("<%d,%d>, ",tmp->info.mid,counter);
        }
        tmp = tmp->next;
        counter++;
    }
    printf("\nDONE\n");
    if(flag == 1){
        return 1;
    }
    return 0;
}

/*
 * Take off movie - Event T
 *
 * Movie mid is taken off the service. It is removed
 * from every user's suggested list -if present- and
 * from the corresponding category list.
 */
void take_off_movie(unsigned mid){
    struct user *pUser;
    struct suggested_movie *sm_ptr;
    struct suggested_movie *sm_tmp_ptr;    
    struct movie *p;
    struct movie *tmp_p;
    int i;

    /* Delete movie with id = mid from every user's suggested movies list */
    printf("\nT %d\n",mid);

    pUser = u_head;
    while(pUser!=S){
        sm_tmp_ptr = NULL;
        sm_ptr = pUser->suggestedHead;
	/* Searching for the movie in user list's suggested movies list, that pointer pUser points to */
        while(sm_ptr!=NULL && sm_ptr->info.mid != mid){
            sm_tmp_ptr = sm_ptr;
            sm_ptr = sm_ptr->next;
        }
        if(sm_ptr!=NULL && sm_ptr->info.mid == mid){
                if(sm_ptr == pUser->suggestedHead){ /* If the movie is at the head of the list */
                    if(sm_ptr->next!=NULL){ /* If there are no other movies in the list */
                        pUser->suggestedHead = pUser->suggestedHead->next;
                    }
                }
                else if(sm_ptr->next == NULL){ /* If the movie is at the tail of the list */
                    sm_ptr->prev->next = NULL;
                    sm_ptr->prev = NULL;
                }
                else{
                    sm_tmp_ptr->next = sm_ptr->next;
                    sm_ptr->next->prev = sm_tmp_ptr;
                    sm_ptr->next = NULL;
                    sm_ptr->prev = NULL;
                }
                free(sm_ptr);
                printf("%d removed from %d suggested list.\n",mid,pUser->uid);
                break;
            }
        pUser = pUser->next;
    }

    /* Delete movie with id = mid from category lists */
    for(i=0;i<6;i++){
        tmp_p = NULL;
        p = CategoryTable[i];
	/* Searching for the movie in Category Table's list that pointer p points to */
        while(p!=NULL && p->info.mid<mid){
            tmp_p=p;
            p=p->next;
        }
        if(p == NULL || p->info.mid!=mid){
            continue;
        }
        else{
            if(p->next == NULL && tmp_p == NULL){ /* If the movie is at the beggining of the list */
                CategoryTable[i] = NULL;
            }
            else if(p->next == NULL && tmp_p != NULL){ /* If the movie is at the end of the list and the list has more than one elements */
                tmp_p->next=NULL;
            }
            else if(tmp_p == NULL && p->next!=NULL){ /* If the movie that pointer p points to is the only movie in the list */
                CategoryTable[i] = CategoryTable[i]->next;
                p->next = NULL;
            }
            else{
                tmp_p->next = p->next;
            }
            free(p);            
            printf("%d removed from %s category list.\n",mid,CategoryAsString((movieCategory_t)i));
            break;
        }
    }
    /* Print the movies of the category table that the movie with id = mid belonged to*/
    p = CategoryTable[i];
    printf("    Category list = ");
    while(p){
        if(p->next == NULL){
            printf("%d",p->info.mid);
        }
        else{
            printf("%d, ",p->info.mid);
        }
        p = p->next;
    }
    printf("\nDONE\n");
}

/*
 * Print movies - Event M
 *
 * Prints information on movies in
 * per-category lists
 */
void print_movies(void){
    struct movie *p;
    int i;
    int counter;

    printf("\nM\nCategorized Movies:");
    for(i=0;i<6;i++){
        p = CategoryTable[i];
        counter = 1;
        printf("\n    %s: ",CategoryAsString((movieCategory_t)i));    
        while(p){
            if(p->next == NULL){
                printf("<%d, %d>",p->info.mid,counter);
            }
            else{
                printf("<%d, %d>, ",p->info.mid,counter);
            }
            counter++;
            p = p->next;
        }    
    }
    printf("\nDONE\n");
}

/*
 * Print users - Event P
 *
 * Prints information on users in
 * users list
 */
void print_users(void){
    struct user *pUser;
    struct suggested_movie *pSm;
    struct movie *pWh;

    pUser = u_head;
    printf("\nP\n");
    printf("    Users:");
    while(pUser!=S){
        pSm = NULL;
        pWh = NULL;
        printf("\n    %d\n",pUser->uid);
        pSm = pUser->suggestedHead;
        printf("       Suggested: ");
        while(pSm){
            if(pSm->next == NULL){
                printf("%d",pSm->info.mid);
            }
            else{
                printf("%d, ",pSm->info.mid);
            }
            pSm = pSm->next;
        }
        pWh = pUser->watchHistory;
        printf("\n       Watch History: ");
        while(pWh){
            if(pWh->next == NULL){
                printf("%d",pWh->info.mid);
            }
            else{
                printf("%d, ",pWh->info.mid);
            }
            pWh = pWh->next;
        }
        pUser = pUser->next;
    }
    printf("\nDONE\n");
}

/* Returns the name of the Category as a string */
char *CategoryAsString(movieCategory_t category){
    switch (category){
    case HORROR:
        return "Horror";
        break;
    case SCIFI:
        return "Sci-fi";
        break;
	case DRAMA:
        return "Drama";
        break;
	case ROMANCE:
        return "Romance";
        break;
	case DOCUMENTARY:
        return "Documentary";
        break;
	case COMEDY:
        return "Comedy";
        break;
    default:
        break;
    }
}

/* Defines the order in which movies are added to the suggested movies list - Used in event S */
int changeTurn(int turn){
    if(turn == 0){
        return 1;
    }
    return 0;
}

/* Free the memory at the end of the program */
void free_memory(void){
    struct movie *pCT;
    struct new_movie *ptr_nm_head = NULL;
    struct user *ptr_u_head;
    struct movie *pWh;
    struct suggested_movie *pSm;
    int i;

    for(i=0;i<6;i++){ /* Free the whole category table */
        while(CategoryTable[i]){
            pCT = CategoryTable[i];
            CategoryTable[i] = CategoryTable[i]->next;
            free(pCT);
        }
    }
    while(nm_head){ /* Free the new movie list */
        ptr_nm_head = nm_head;
        nm_head = nm_head->next;
        free(ptr_nm_head);
    }
    while(u_head){ /* Free the user list */
        ptr_u_head = u_head;
        while(ptr_u_head->watchHistory){
            pWh = ptr_u_head->watchHistory;
            ptr_u_head->watchHistory = ptr_u_head->watchHistory->next;
            free(pWh);
        }
        while(ptr_u_head->suggestedHead){
            pSm = ptr_u_head->suggestedHead;
            ptr_u_head->suggestedHead = ptr_u_head->suggestedHead->next;
            free(pSm);
        }
        u_head = u_head->next;
        free(ptr_u_head);
    }
}