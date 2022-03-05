#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "hashmap.c"

size_t hash(void* k) {
    int *key = (int*) k;
    size_t hash = *key;
    return hash;
}

int cmp(void *a, void *b) {
    if (a == b) {
        return 1;
    } else {
        return 0;
    }
}

void key_destruct(void* thing) {
    free(thing);
}

void value_destruct(void* thing) {
    free(thing);
}

int main() {

    struct hash_map* hash_map = hash_map_new(hash, cmp, key_destruct, value_destruct);

    if(NULL != hash_map) {
        printf("You have initialized Hash \n\n");
    }
    
    int *a = malloc(sizeof(int));
    *a = 2462819;
    int *b = malloc(sizeof(int));
    *b = 1111;
    int *c = malloc(sizeof(int));
    *c = 2462851;
    int *d = malloc(sizeof(int));
    *d = 2222;

    hash_map_put_entry_move(hash_map, (void *)a, (void *)b);
    hash_map_put_entry_move(hash_map, (void *)c, (void *)d);

    int *value = (int *)hash_map_get_value_ref(hash_map, a);


    hash_map_remove_entry(hash_map, (void *)a);
    hash_map_remove_entry(hash_map, (void *)c);


    hash_map_destroy(hash_map);

    return 0;
}
