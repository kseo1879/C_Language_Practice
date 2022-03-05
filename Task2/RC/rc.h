#ifndef TREE_REF
#define TREE_REF

#include <stdlib.h>

#define RC_INVALID_REF (0xFFFFFFFFFFFFFFFF)
#define RC_INIT_SZ (32)
#define RC_GROWTH_RT (2)
#define RC_REF(type,ptr) (((type*)ptr->ref))
#define RC_DEREF(type,ptr) (*RC_REF(type,ptr))

struct rc_entry {
    size_t count;
    size_t* dep_list;
    size_t n_deps;
    size_t dep_capacity;
};

struct strong_ref {
    void* ptr;
    struct rc_entry entry;
};

struct weak_ref {
    size_t entry_id;
};

struct strong_ref* rc_alloc(void* ptr, size_t n, struct strong_ref* dep);
struct weak_ref rc_downgrade(struct strong_ref* ref);
struct strong_ref* rc_upgrade(struct weak_ref weak);
void rc_cleanup();
#endif
