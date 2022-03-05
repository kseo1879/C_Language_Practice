#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include <pthread.h>

/*
 * This Headerfile is for constructing Linked List.
 * Each of the linked list will contain the root 
 * of bucket when using separate chaining. 
*/

typedef struct node Node;
typedef struct linked_list Linked_list;

typedef struct node {
    
    void *key;
    void *value;
    Node *next;
    Node *prev;

} Node;

//lock is to ensure that only one thread is modifying the linked list. 
typedef struct linked_list {

    int length;
    Node *root;
    pthread_mutex_t lock;

} Linked_list;

#endif
