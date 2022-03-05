#include "rc.h"
#include <stdio.h>

//This node will strongref and the pointer ot 
typedef struct node {
	struct strong_ref *ref;
	struct node *prev;
	struct node *next;
}Node;

//linked_list will always contain the head of the node. and the length of all the nodes 
typedef struct linked_list {
	int length;
	Node *head;
}Linked_list;

//This is a static linked list that has head of the Node of allocated strong_ref 
static Linked_list *allocated_strong = NULL;


//This funciton will add item to to the linked list
Linked_list *add_to_linked_list(struct strong_ref *ref, Linked_list *list);

//This funciton will add item to to the linked list
Linked_list *remove_from_linked_list(struct strong_ref *ref, Linked_list *list);

//This funciton will create item to the linked list
struct strong_ref *create_strong_ref(size_t size);

//This will deallocate_all with dependency
void deallocate_all(struct strong_ref *ref);

//This will find the strong_ref in the saved inked list. 
struct strong_ref *find_strong_ref_in_linked_list(void *ptr, Linked_list *list);



/**
 * Returns an allocation of n bytes and creates an internal rc entry.
 * _______________________________________________________________________
 * If the ptr argument is NULL and deps is NULL, it will return a new 
 * allocation //DONE
 * _______________________________________________________________________
 * If the ptr argument is not NULL and an entry exists, it will increment
 *  the reference count of the allocation and return a strong_ref pointer
 * //DONE
 * _______________________________________________________________________
 * If the ptr argument is NULL and deps is not NULL, it will return 
 * a new allocation but the count will correlate to the dependency
 * if the dependency is deallocated the reference count on the object will 
 * decrement
 * _______________________________________________________________________
 * If the ptr argument is not NULL and an entry exists and dep is not
 * NULL, it will increment the count of the strong reference but the count
 * will be related to the dependency, if the dependency is deallocated the
 * reference count on the object will decrement
 */
//It can be devided into two parts where ptr is NULL and where PTR is not null
struct strong_ref* rc_alloc(void* ptr, size_t n, struct strong_ref* dep) {
	struct strong_ref *temporary_ptr = NULL;
	struct strong_ref *dependency = NULL;
	//If the ptr argument is NULL;
	if(NULL == ptr) {
		temporary_ptr = create_strong_ref(n);
		//If the dependency is also NULL;
		if(NULL == dep) {
			allocated_strong = add_to_linked_list(temporary_ptr, allocated_strong);
			return temporary_ptr;
		}

		dependency = find_strong_ref_in_linked_list(dep->ptr, allocated_strong);
		if(NULL == dependency) {
			return NULL;
		}
		++dependency->entry.n_deps;
		dependency->entry.dep_list = (size_t *)realloc(dependency->entry.dep_list, sizeof(size_t *) * (dependency->entry.n_deps));
		dependency->entry.dep_list[dependency->entry.n_deps - 1] = (size_t) (temporary_ptr);

		allocated_strong = add_to_linked_list(temporary_ptr, allocated_strong);
		return temporary_ptr;
	} else {
		//If Dependency is not provided
		if(NULL == dep) {
			temporary_ptr = find_strong_ref_in_linked_list(ptr, allocated_strong);
			if(temporary_ptr == NULL) {
				return NULL;
			}
			++temporary_ptr->entry.count; 
		} else { //If PTR is provided and dependency is not null;
			dependency = find_strong_ref_in_linked_list(dep->ptr, allocated_strong);
			if(dependency == NULL) {
				return NULL;
			}
			//Dependency is here but the ptr might have been deallocated so double check needs to happen
			temporary_ptr = find_strong_ref_in_linked_list(ptr, allocated_strong);
			if(temporary_ptr == NULL) {
				return NULL;
			}
			++temporary_ptr->entry.count; 
			++dependency->entry.n_deps;
			dependency->entry.dep_list = (size_t *)realloc(dependency->entry.dep_list, sizeof(size_t *) * (dependency->entry.n_deps));
			dependency->entry.dep_list[dependency->entry.n_deps - 1] = (size_t) (temporary_ptr);
			allocated_strong = add_to_linked_list(temporary_ptr, allocated_strong);
		}

	}
	return temporary_ptr;
}



/**
 * Downgrades a strong reference to a weak reference, this will decrement the 
 * reference count by 1
 * If ref is NULL, the function will return an invalid weak ref object
 * If ref is a value that does not exist in the reference graph, it will return
 * an weak_ref object that is invalid
 * ____________________________________________________________________________
 * If ref is a value that does exist in the reference graph, it will return
 *    a valid weak_ref object
 * ____________________________________________________________________________
 * An invalid weak_ref object is where its entry_id field is set to 
 *   0xFFFFFFFFFFFFFFFF
 * ____________________________________________________________________________
 * @param strong_ref* ref (reference to allocation)
 * @return weak_ref (reference with an entry id)
 */
struct weak_ref rc_downgrade(struct strong_ref* ref) {
	struct weak_ref r;
	r.entry_id = (size_t) RC_INVALID_REF;

	//If ref is null or does not exist in the allocated list
	if(NULL == ref || NULL == find_strong_ref_in_linked_list(ref->ptr, allocated_strong)) {
		return r;
	} 
	
	struct strong_ref *tmp = find_strong_ref_in_linked_list(ref->ptr, allocated_strong);
	--tmp->entry.count;
	if(0 == tmp->entry.count) {
		if(tmp->entry.n_deps != 0) {
			deallocate_all(tmp);
		} else {
			allocated_strong = remove_from_linked_list(tmp, allocated_strong);
			free(tmp->ptr);
			if(NULL != tmp->entry.dep_list) {
				free(tmp->entry.dep_list);
			}
			free(tmp);
		}
		return r;
	}

	r.entry_id = (size_t)ref->ptr;

	return r;
}



/**
 * Upgrdes a weak reference to a strong reference.
 * The weak reference should check that the entry id is valid (bounds check)
 * If a strong reference no longer exists or has been deallocated, the return 
 *   result should be null.
 */
struct strong_ref* rc_upgrade(struct weak_ref ref) {
	if(ref.entry_id == RC_INVALID_REF || NULL == (void *)ref.entry_id) {
		return NULL;
	}

	if(NULL == allocated_strong) {
		return NULL;
	}

	struct strong_ref *tmp = find_strong_ref_in_linked_list((void *)ref.entry_id, allocated_strong);
	//If it has been deallocated;
	if(tmp == NULL) {
		return NULL;
	}
	++tmp->entry.count;
	return tmp;
}



/**
 * Cleans up the reference counting graph.
 */
void rc_cleanup() {
	if(NULL != allocated_strong || NULL != allocated_strong->head) {
		Node *cursur = allocated_strong->head;
		if(cursur->ref != NULL) {
			deallocate_all(cursur->ref);
		}
		// while(cursur->ref != NULL) {
		// 	allocated_strong = remove_from_linked_list(cursur->ref, allocated_strong);
		// 	free(cursur->ref->ptr);
		// 	if(NULL != cursur->ref->entry.dep_list) {
		// 		free(cursur->ref->entry.dep_list);
		// 	}
		// 	// free(cursur->ref);
		// 	// if(NULL == cursur->next) {
		// 	// 	break;
		// 	// }
		// 	cursur = cursur->next;
		// }
	}
}

// ++++++++++++++++++++++++++++Below is new implementation++++++++++++++++++++++++++++

Linked_list *add_to_linked_list(struct strong_ref *ref, Linked_list *list) {
	if(list == NULL) {
		list = (Linked_list *)malloc(sizeof(Linked_list));
		list->length = 1;
		list->head = (Node *)malloc(sizeof(Node));
		list->head->prev = NULL;
		list->head->next = NULL;
		list->head->ref = ref;
		return list;
	} else {
		list->length ++;
		Node *cursur = list->head;
		while(cursur->next != NULL) {
			cursur = cursur->next;
		}
		cursur->next = (Node *)malloc(sizeof(Node));
		cursur->next->prev = cursur;
		cursur->next->next = NULL;
		cursur->next->ref = ref;
		return list;
	}
}


Linked_list *remove_from_linked_list(struct strong_ref *ref, Linked_list *list) {
	if(NULL == list || NULL == ref) {
		return NULL;
	}
	if(list->head == NULL) {
		return NULL;
	}

	Node *tmp_node = NULL;
	Node *cursur = list->head;
	for(int i = 0; i < list->length; ++i) {
		if(cursur->ref == ref) {
			tmp_node = cursur;
			break;
		}
		if(NULL != cursur->next) {
			cursur = cursur->next; //Being Extra careful that there is node
		}
	}

	if(NULL == tmp_node) {
		return NULL;
	} else {
		//If the list had only one element
		if(list->length == 1) {
			list->length = 0;
			free(list->head);
			free(list);
			list = NULL;
			return list;
		}
		--list->length;
		//if the list had more than one element and it's the begining node
		if(NULL == tmp_node->prev) {
			if(NULL != tmp_node->next) {
				list->head = tmp_node->next;
				tmp_node->prev = NULL;
			}
			free(tmp_node);
			return list;
		}
		//If it's in the middle
		if(NULL != tmp_node->next) {
			tmp_node->prev->next = tmp_node->next;
			tmp_node->next->prev = tmp_node->prev;
			free(tmp_node);
			return list;
		} else { //If the node is in the last
			tmp_node->prev->next = NULL;
			free(tmp_node);
			return list;
		}	
	}
}


struct strong_ref *create_strong_ref(size_t size) {
	struct strong_ref *temporary = (struct strong_ref *)malloc(sizeof(struct strong_ref));
	temporary->ptr = malloc(size);
	temporary->entry.count = 1;
	temporary->entry.dep_list = NULL;
	temporary->entry.n_deps = 0;
	temporary->entry.dep_capacity = 0;
	return temporary;
}


void deallocate_all(struct strong_ref *ref) {

	struct strong_ref *tmp = find_strong_ref_in_linked_list(ref->ptr, allocated_strong);
	//tmp might already be deallocated
	if(NULL != tmp) {
		for(int i = 0; i < (int)tmp->entry.n_deps; ++i) {
			deallocate_all((void *)(tmp->entry.dep_list[i]));
		}
		allocated_strong = remove_from_linked_list(tmp, allocated_strong);
		free(tmp->ptr);
		if(NULL != tmp->entry.dep_list) {
				free(tmp->entry.dep_list);
		}
		free(tmp);
	}
}


struct strong_ref *find_strong_ref_in_linked_list(void *ptr, Linked_list *list) {
	/**
	* Below condition is to check if the list is null if programmer accidently didn't
	* free the memory correctly
	*/

	if(list == NULL) {
		return NULL;
	} 
	
	if(list->length == 0) {
		
		return NULL;
	}

	if(list->head == NULL) {
		
		return NULL;
	}

	
	//Here it starts
	Node *cursur = list->head;
	for(int i = 0; i < list->length; i++) {
		if(ptr == cursur->ref->ptr) {
			return cursur->ref;
		}
		if(NULL != cursur->next) {
			cursur = cursur->next; //JUST being extra careful here.
		}
	}
	return NULL;
}


// #include "rc_test.c"

// int main() {

// 	int a = 0;
// 	a = rc_example_3();
// 	if(NULL == allocated_strong) {
// 		printf("You Got it!!\n");
// 	}

// 	return 0;
// }
 