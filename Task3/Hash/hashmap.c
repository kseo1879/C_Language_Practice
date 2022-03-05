#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hashmap.h"

//This is the initial size of the bucket. It will resize when 75 percent of them is full. 
#define INIT_BUCKET_SIZE 32

/*
 * Creates a new hash map with an associagted hash function, 
 * comparison function and destructor function.
*/
struct hash_map* hash_map_new(size_t (*hash)(void*), int (*cmp)(void*,void*),
    void (*key_destruct)(void*), void (*value_destruct)(void*)) {

		//The return NULL condition
		if(NULL == hash || NULL == cmp || 
		NULL == key_destruct || NULL == value_destruct) {
			return NULL;
		}		

		//else we need to construct the map. 
		struct hash_map *my_map = (struct hash_map *)malloc(sizeof(struct hash_map));
		my_map -> list_size = INIT_BUCKET_SIZE;
		my_map -> filled_bucket_size = 0;

		//Initializing Bucket. 
		my_map -> list = (Linked_list **)malloc(sizeof(Linked_list *) * INIT_BUCKET_SIZE);
		for(int i = 0; i < INIT_BUCKET_SIZE; ++i) {
			my_map -> list[i] = NULL;
		}

		//Initializing Functions
		my_map -> hash = hash;
		my_map -> cmp = cmp;
		my_map -> key_destruct = key_destruct;
		my_map ->value_destruct = value_destruct;
	
	return my_map;
}

/*
 * Puts an entry in the hashmap, associating a key with a value. 
 * If the Key does not exist, a new entry should be created.
 * If the entry exist, oldentry should be removed (hash_map_remove_entry)
*/
void hash_map_put_entry_move(struct hash_map* map, void* k, void* v) {

	//Error
	if(NULL == map || NULL == k || NULL == v) {
		return;
	}

	struct hash_map *my_map = map;

	//This will find the index of the bucket
	int index = find_index_to_put(my_map, k);

	//Now we need to update the list
	Linked_list *my_list = my_map -> list[index];


	if(NULL == my_list) {
		//We need to make the key and value
		my_map -> filled_bucket_size += 1;
		my_map -> list[index] = (Linked_list *)malloc(sizeof(Linked_list));
		pthread_mutex_lock(&(my_map -> list[index] -> lock));
		my_list = my_map -> list[index];
		my_list -> length = 1;
		my_list -> root = (Node *)malloc(sizeof(Node));
		
		Node *node = my_list -> root;
		node -> key = k;
		node -> value = v;
		node -> prev = NULL;
		node -> next = NULL;
		pthread_mutex_unlock(&(my_list->lock));
		return;
	} else {
		//We need to modify the data structure
		pthread_mutex_lock(&(my_list->lock));
		Node *cursur = my_list -> root;
		index = check_exist_key(my_list, k, my_map -> cmp);
		if(-1 == index) {
			//There is no key exist
			for(int i = 0; i < (my_list -> length - 1); ++i) {
				cursur = cursur -> next;
			} 

			my_list -> length += 1;

			cursur -> next = (Node *)malloc(sizeof(Node));
			cursur -> next -> key = k;
			cursur -> next -> value = v;
			cursur -> next -> prev = cursur;
			cursur -> next -> next = NULL;
			pthread_mutex_unlock(&(my_list->lock));
			return;
		} else {
			//There is already a key exist in the hash
			for(int i = 0; i < index; ++i) {
				cursur = cursur -> next;
			}

			if(v != cursur -> value) {
				my_map -> value_destruct(cursur -> value);
			}

			if(k != cursur -> key) {
				my_map -> key_destruct(cursur -> key);
			}
			cursur -> key = k;
			cursur -> value = v;
			pthread_mutex_unlock(&(my_list->lock));
		}	
	}	
}

/*
 * This will return to index to put the entry to the old hash map
*/
int find_index_to_put(struct hash_map *map, void *k) {

	int list_size = map -> list_size;
	size_t hash_value = map -> hash(k);
	int bucket_index = hash_value % list_size;

	return bucket_index;
}

/*
 * This function will return the index of the key found
 * If the key is not found it will return -1.
*/
int check_exist_key(Linked_list *list, void *k, int (*cmp)(void *,void *)) {

	if(NULL == list) {
		return -1;
	}
	
	Node *cursur = list -> root;

	for(int i = 0; i < list -> length; ++i) {
		
		if(NULL == cursur) {
			perror("This shouldn't be happending");
		}
		int res = cmp(cursur -> key, k);
		if(1 == res) {
			return i;
		}
		cursur = cursur -> next;
	}

	return -1;
}

void hash_map_remove_entry(struct hash_map* map, void* k) {
	//Error handling
	if(NULL == map || NULL == k) {
		return;
	}
	
	int index = find_index_to_put(map, k);
	Linked_list *my_list = map -> list[index];
	int key_num = -1;
	if(NULL != my_list) {
		pthread_mutex_lock(&(my_list->lock));
		key_num = check_exist_key(my_list, k, map -> cmp);
		pthread_mutex_unlock(&(my_list->lock));
	} else {
		return;
	}

	if(-1 == key_num) {
		//This means that the key does not exsist. 
		return;
	}

	//By this stage their should exist the node with key
	//1. Destroy the key and value.
	pthread_mutex_lock(&(my_list->lock));

	Node *cursur = my_list -> root;
	for(int i = 0; i < key_num; ++i) {
		cursur = cursur -> next;
	}
	map -> value_destruct(cursur->value);
	map -> key_destruct(cursur ->key);
	my_list -> length -= 1;
	pthread_mutex_unlock(&(my_list->lock));

	//Now connect the cursur;
	pthread_mutex_lock(&(my_list->lock));
	if(NULL != cursur -> next) {
		if(NULL != cursur -> prev) {
			cursur -> next -> prev = cursur -> prev;
			cursur -> prev -> next = cursur -> next;
		} else {
			//This means it's the beginning node.
			cursur -> next -> prev = NULL;
			my_list -> root = cursur -> next;
		}
		free(cursur);
		pthread_mutex_unlock(&(my_list->lock));
	} else {
		//This means it's the last node
		if(NULL != cursur -> prev) {
			cursur -> prev -> next = NULL;
			free(cursur);
			pthread_mutex_unlock(&(my_list->lock));
		} else {
			free(cursur);
			pthread_mutex_unlock(&(my_list->lock));
			free(my_list);
			map -> filled_bucket_size -= 1;
			map -> list[index] = NULL;
		}
	}
}

void* hash_map_get_value_ref(struct hash_map* map, void* k) {
	if(NULL == map || NULL == k) {
		return NULL;
	}

	int index = find_index_to_put(map, k);
	Linked_list *my_list = map -> list[index];

	if(NULL != my_list) {
		pthread_mutex_lock(&(my_list->lock));
	}
	int key_num = -1;
	key_num = check_exist_key(my_list, k, map -> cmp);
	if(NULL != my_list) {
		pthread_mutex_unlock(&(my_list->lock));
	}

	if(-1 == key_num) {
		return NULL;
	} else {
		pthread_mutex_lock(&(my_list->lock));
		Node *cursur = my_list -> root;
		for(int i = 0; i < key_num; ++i) {
			cursur = cursur -> next;
		}
		pthread_mutex_unlock(&(my_list->lock));

		return cursur -> value;
	}

	return NULL;
}

void hash_map_destroy(struct hash_map* map) {
	//Error handling
	
	if(NULL == map) {
		return;
	}
	pthread_mutex_lock(&(map -> lock));


	//First Destory all the Nodes and buckets
	int bucket_size = map -> list_size;
	Linked_list *my_list = NULL;
	for(int i = 0; i < bucket_size; ++i) {
		my_list = map -> list[i];
		//If There is no node in the list
		if(NULL == my_list) {
			continue;
		}

		Node *cursur = my_list -> root;
		Node *tmp = NULL;
		for(int k = 0; k < my_list -> length; ++k) {
			tmp = cursur -> next;
			map -> key_destruct(cursur -> key);
			map -> value_destruct(cursur -> value);
			free(cursur);
			cursur = tmp;
		}
		free(my_list);
	}
	free(map -> list);
	map -> list_size = 0;
	map -> filled_bucket_size = 0;
	pthread_mutex_unlock(&(map -> lock));
	free(map);
	return;
}