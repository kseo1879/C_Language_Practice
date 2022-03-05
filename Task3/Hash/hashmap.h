#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>
#include "linked_list.h"

/*
 * This will hold all the information required 
 * to obtain the hash.
 * 1. List size is referring to the number of buckets
 * 2. Num of keys are useful when to determine the resize
 * 3. It is a pointer to pointers of the linked list. 
 * And the rest is functions associated to the hash
*/
struct hash_map {
    int list_size;
    int filled_bucket_size;
    Linked_list **list;
    
    //Funtions
    size_t (*hash)(void *);
    int (*cmp)(void *, void *);
    void (*key_destruct)(void *);
    void (*value_destruct)(void *);
    pthread_mutex_t lock;
};

struct hash_map* hash_map_new(size_t (*hash)(void*), int (*cmp)(void*,void*),
    void (*key_destruct)(void*), void (*value_destruct)(void*));

void hash_map_put_entry_move(struct hash_map* map, void* k, void* v);

int find_index_to_put(struct hash_map *map, void *k);

int check_exist_key(Linked_list *list, void *k, int (*cmp)(void*,void*));

void hash_map_remove_entry(struct hash_map* map, void* k);

void* hash_map_get_value_ref(struct hash_map* map, void* k);

void hash_map_destroy(struct hash_map* map);

#endif
