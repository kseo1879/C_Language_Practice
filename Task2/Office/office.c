#include "office.h"
#include <string.h>

typedef struct queue {
  struct employee *value;
  struct queue *next;
}Queue;

//This will update that queue with no subs
void get_employee_with_no_sub(Queue *queue, Queue *tmp_queue);

//This will update the queue that are in the same level
void get_employee_in_level(Queue *queue, Queue *tmp_queue, size_t level);

//This will return everyelement in the level
void get_employee_with_name(Queue *queue, Queue *tmp_queue, const char* name); 

//This will return a queue with the supervisor;
void get_employee(Queue *queue, struct employee *tmp, struct employee *supervisor);

//This will return the depth of the node
size_t depth_of_employee(struct employee *employee);

//This will add employee to the queue
void add_to_queue(Queue *queue, struct employee *employee);

//This will add all to the quid in same level
void add_all_to_queue_level(Queue* queue, struct employee *employee, size_t level);

//This will free the memory allocated with the queue. However it won't free it's employ member.
void free_the_queue(Queue *queue);

//This will add everything to the queue
void add_all_to_queue(Queue* queue, struct employee *employee);

//This will free the queue and the employee
void free_the_queue_employee(Queue *queue);

//This will find the index of employee form the subordinate list of its supervisor.
int find_index_of_subordinates(struct employee* employee);

/**
 * Places an employee within the office, if the supervisor field is NULL
 *  it is assumed the employee will be placed under the next employee that is
 * not superivising any other employee (top-down, left-to-right).
 * 
 * If the supervisor is not NULL, it will be added to the supervisor's subordinates list
 *  of employees (make sure the supervisor exists in hierarchy).
 * if the office or employee are null, the function not do anything.
 */
void office_employee_place(struct office* off, struct employee* supervisor,
  struct employee* emp) {

    //struct employee *tmp;
    Queue *queue;
    Queue *tmp_queue;

    //Error Handling: Office and Employ should not be NULL
    if(NULL == off || NULL == emp){
        return;
    }
    //By this stage office and employ is not NULL

    //1: Check if office has a head
    //2: If it has a head than check if the supervisor is NULL
    //3: If not find a supervisor and add to subordinates.
    if(NULL == off->department_head){ //1
        off->department_head = (struct employee *)malloc(sizeof(struct employee));
        off->department_head->name = (char *)malloc(sizeof(char) * (strlen(emp->name)+1));
        strcpy(off->department_head->name, emp->name);
        off->department_head->subordinates = NULL;
        off->department_head->supervisor = NULL; 
        off->department_head->n_subordinates = 0;
    }else if(NULL == supervisor){//2
        queue = (Queue *)malloc(sizeof(Queue));
        tmp_queue = (Queue *)malloc(sizeof(Queue));
        queue->value = NULL;
        queue->next = NULL;
        tmp_queue->value = NULL;
        tmp_queue->next = NULL;

        add_to_queue(tmp_queue, off->department_head);
        
        get_employee_with_no_sub(queue, tmp_queue);
        
        queue->value->n_subordinates = 1;
        queue->value->subordinates = (struct employee *)malloc(sizeof(struct employee) * (queue->value->n_subordinates));
        queue->value->subordinates[0].name = (char *)malloc(sizeof(char) * (strlen(emp->name)+1));
        strcpy(queue->value->subordinates[0].name, emp->name);
        queue->value->subordinates[0].subordinates = NULL;
        queue->value->subordinates[0].supervisor = queue->value; 
        queue->value->subordinates[0].n_subordinates = 0;
        free_the_queue(queue);
        queue = NULL;
    }else{//By this step supervisor is provied and needed to be added to it's subordinates
        queue = (Queue *)malloc(sizeof(queue));
        queue->value = NULL;
        queue->next = NULL;
        get_employee(queue, off->department_head, supervisor);
        if(NULL == queue->value){
            return;
        }
        queue->value->n_subordinates += 1;
        queue->value->subordinates = (struct employee *)realloc(queue->value->subordinates, sizeof(struct employee) * (queue->value->n_subordinates));
        queue->value->subordinates[queue->value->n_subordinates-1].name = (char *)malloc(sizeof(char) * (strlen(emp->name)+1));
        strcpy(queue->value->subordinates[queue->value->n_subordinates-1].name, emp->name);
        queue->value->subordinates[queue->value->n_subordinates-1].subordinates = NULL;
        queue->value->subordinates[queue->value->n_subordinates-1].supervisor = queue->value; 
        queue->value->subordinates[queue->value->n_subordinates-1].n_subordinates = 0;
        free_the_queue(queue);
        queue = NULL;
    }
    
}


/**
 * Fires an employee, removing from the office
 * If employee is null, nothing should occur
 * If the employee does not supervise anyone, they will just be removed
 * If the employee is supervising other employees, the first member of that 
 *  team will replace him.
 */
void office_fire_employee(struct employee *employee) {
	//Error Handling
    if(NULL == employee){
        return;
    }
    //If is has a supervisor
    if(NULL != employee->supervisor){
        //If it has a supervisor and a subordinates
        if(0 != employee->n_subordinates){
            //if the subordinate of the employee has no other subordinates.
            if(employee->subordinates[0].n_subordinates == 0){
                //If it only has one subordinate
                if(employee->n_subordinates == 1){
                    struct employee *supervisor = employee->supervisor;
                    employee->subordinates[0].supervisor = supervisor;
                    free(employee->name);
                    employee->name = employee->subordinates[0].name;
                    free(employee->subordinates);
                    // int a = find_index_of_subordinates(employee);
                    // struct employee *tmp = &(supervisor->subordinates[a]);
                    // *(supervisor->subordinates+a) = employee->subordinates[0];
                    employee->subordinates = NULL;
                    employee->n_subordinates = 0;
                    return;
                }
                // int a = find_index_of_subordinates(&(employee->subordinates[0]));
                for(int i = 0; i < employee->n_subordinates -1; ++ i) {
                    struct employee tmp;
                    tmp = (employee->subordinates[i]);
                    (employee->subordinates[i])  = employee->subordinates[i+1];
                    employee->subordinates[i+1] = tmp;
                }
                free(employee->name);
                employee->name = employee->subordinates[employee->n_subordinates-1].name;
                //size_t a = employee->n_subordinates-1;
                free(employee->subordinates);
            }
        }else{//If it has a supervisor but no subordinates
            //If They only have one sub
            if(employee->supervisor->n_subordinates == 1){
                struct employee *supervisor = employee->supervisor;
                free(employee->name);
                employee->name = NULL;
                free(supervisor->subordinates);
                supervisor->subordinates = NULL;
                supervisor->n_subordinates = 0;
                return;
            }
            //More than one
            int a = -1;
            //printf("Hello2\n");
            a = find_index_of_subordinates(employee);
            free(employee->name);
            for(int i = a; i < employee->supervisor->n_subordinates - 1; ++ i) {
                struct employee *tmp;
                tmp = &(employee->supervisor->subordinates[i]);
                //printf("%p\n", tmp);
                *(employee->supervisor->subordinates + i)  = employee->supervisor->subordinates[i + 1];
                *(employee->supervisor->subordinates+(i + 1)) = *tmp;
            }
            employee->supervisor->subordinates = (struct employee *)realloc(employee->supervisor->subordinates, sizeof(struct employee) * (employee->supervisor->n_subordinates));
            employee->supervisor->n_subordinates -= 1;
        }
    }else{ //If it doesn't have a supervisor it means that its head therefore nothing should happen
        return;
    }
}


/**
 * Retrieves the first encounter where the employee's name is matched to one in the office
 * If the employee does not exist, it must return NULL
 * if office or name are NULL, your function must do nothing
 */ 
struct employee* office_get_first_employee_with_name(struct office* office,
  const char* name) {
	if(NULL == office || name == NULL){
        return NULL;
    }
    
    if(NULL == office->department_head || NULL == office->department_head->name) {
        return NULL;
    }

    Queue *queue = (Queue *)malloc(sizeof(Queue));
    Queue *tmp_queue = (Queue *)malloc(sizeof(Queue));
    queue->value = NULL;
    queue->next = NULL;
    tmp_queue->value = NULL;
    tmp_queue->next = NULL;
    add_to_queue(tmp_queue, office->department_head);

    //add_all_to_queue_level(queue, office->department_head, level);
    get_employee_with_name(queue, tmp_queue, name);
    //Review from here
    Queue *tmp;
    tmp = queue;
    size_t count = 0;
    while(tmp->value != NULL){
        count ++;
        if(tmp->next == NULL){
            break;
        }
        tmp = tmp->next;
    }
    tmp = queue;
    if(count == 0){
        free_the_queue(queue);
        return NULL;
    }
    if(queue->value == NULL){
        free_the_queue(queue);
        return NULL;
    }
    struct employee *employee;
    employee = queue->value;

    queue = tmp;
    free_the_queue(queue);
    queue = NULL;
    return employee;
}


/**
 * Retrieves the last encounter where the employee's name is matched to one in the office
 * If the employee does not exist, it must return NULL
 * if office or name are NULL, your function must do nothing
 */ 
struct employee* office_get_last_employee_with_name(struct office* office,
  const char* name) {

	if(NULL == office || strlen(name) == 0 || NULL == name){
        return NULL;
    }
    
    if(NULL == office->department_head) {
        return NULL;
    }
    

    Queue *queue = (Queue *)malloc(sizeof(Queue));
    Queue *tmp_queue = (Queue *)malloc(sizeof(Queue));
    queue->value = NULL;
    queue->next = NULL;
    tmp_queue->value = NULL;
    tmp_queue->next = NULL;
    add_to_queue(tmp_queue, office->department_head);
    //add_all_to_queue_level(queue, office->department_head, level);
    get_employee_with_name(queue, tmp_queue, name);
    //Review from here
    Queue *tmp;
    tmp = queue;
    size_t count = 0;
    while(tmp->value != NULL){
        count ++;
        if(tmp->next == NULL){
            break;
        }
        tmp = tmp->next;
    }
    tmp = queue;
    if(count == 0){
        free_the_queue(queue);
        return NULL;
    }
    if(queue->value == NULL){
        return NULL;
    }
    struct employee *employee = queue->value;
    for(int i = 0; i < count ; ++i){
        employee = queue->value;
        queue = queue->next;
    }
    queue = tmp;
    free_the_queue(queue);
    queue = NULL;
    return employee;
}


/**
 * This function will need to retrieve all employees at a level.
 * A level is defined as distance away from the boss. For example, all 
 * subordinates of the boss are 1 level away, subordinates of the boss's subordinates
 * are 2 levels away.
 * 
 * if office, n_employees or emplys are NULL, your function must do nothing
 * You will need to provide an allocation for emplys and specify the
 * correct number of employees found in your query.
 */
void office_get_employees_at_level(struct office* office, size_t level,
  struct employee** emplys, size_t* n_employees) {
    //Checking for the errors
    
    if(NULL == office || NULL == n_employees || NULL == emplys){
        return;
    }
    
    if(NULL == office->department_head) {
        return;
    }
    

    Queue *queue = (Queue *)malloc(sizeof(Queue));
    Queue *tmp_queue = (Queue *)malloc(sizeof(Queue));
    queue->value = NULL;
    queue->next = NULL;
    tmp_queue->value = NULL;
    tmp_queue->next = NULL;
    add_to_queue(tmp_queue, office->department_head);
    //add_all_to_queue_level(queue, office->department_head, level);
    get_employee_in_level(queue, tmp_queue, level);
    //Review from here
    Queue *tmp;
    tmp = queue;
    size_t count = 0;
    while(tmp->value != NULL){
        count ++;
        if(tmp->next == NULL){
            break;
        }
        tmp = tmp->next;
    }
    if(count == 0){
        free_the_queue(queue);
        *n_employees = count;
        return;
    }
    (*emplys) = (struct employee *)malloc(sizeof(struct employee) * count);
    
    *n_employees = count;
    tmp = queue;
    for(int i = 0; i < count; ++i){
        (*emplys)[i].name = (char *)malloc(sizeof(char) * (strlen(queue->value->name) + 1));
        strcpy((*emplys)[i].name, queue->value->name);

        if(NULL != queue->value->supervisor){
            (*emplys)[i].supervisor = queue->value->supervisor;
        }else{
            (*emplys)[i].supervisor = NULL;
        }

        if(NULL != queue->value->subordinates){ 
            (*emplys)[i].subordinates = (queue->value->subordinates);
        }else{
            (*emplys)[i].subordinates = NULL;
        }

        (*emplys)[i].n_subordinates = queue->value->n_subordinates;
        queue = queue->next;

    }
    queue = tmp;
    free_the_queue(queue);
    queue = NULL;
    return;
}


/**
 * Will retrieve a list of employees that match the name given
 * If office, name, emplys or n_employees is NULL, this function should do
 * nothing
 * if office, n_employees, name or emplys are NULL, your function must do
 * nothing. 
 * You will need to provide an allocation to emplys and specify the
 * correct number of employees found in your query.
 */
void office_get_employees_by_name(struct office* office, const char* name,
  struct employee** emplys, size_t* n_employees) {
	if(NULL == office || NULL == n_employees || NULL == emplys){
        return;
    }
    
    if(NULL == office->department_head) {
        return;
    }
    

    Queue *queue = (Queue *)malloc(sizeof(Queue));
    Queue *tmp_queue = (Queue *)malloc(sizeof(Queue));
    queue->value = NULL;
    queue->next = NULL;
    tmp_queue->value = NULL;
    tmp_queue->next = NULL;
    add_to_queue(tmp_queue, office->department_head);
    //add_all_to_queue_level(queue, office->department_head, level);
    get_employee_with_name(queue, tmp_queue, name);
    //Review from here
    Queue *tmp;
    tmp = queue;
    size_t count = 0;
    while(tmp->value != NULL){
        count ++;
        if(tmp->next == NULL){
            break;
        }
        tmp = tmp->next;
    }
    if(count == 0){
        free_the_queue(queue);
        *n_employees = count;
        return;
    }
    (*emplys) = (struct employee *)malloc(sizeof(struct employee) * count);
    
    *n_employees = count;
    tmp = queue;
    for(int i = 0; i < count; ++i){
        (*emplys)[i].name = (char *)malloc(sizeof(char) * (strlen(queue->value->name) + 1));
        strcpy((*emplys)[i].name, queue->value->name);

        if(NULL != queue->value->supervisor){
            (*emplys)[i].supervisor = queue->value->supervisor;
        }else{
            (*emplys)[i].supervisor = NULL;
        }

        if(NULL != queue->value->subordinates){ 
            (*emplys)[i].subordinates = (queue->value->subordinates);
        }else{
            (*emplys)[i].subordinates = NULL;
        }

        (*emplys)[i].n_subordinates = queue->value->n_subordinates;
        queue = queue->next;

    }
    queue = tmp;
    free_the_queue(queue);
    queue = NULL;
    return;
}

//Done
/**
 * You will traverse the office and retrieve employees using a postorder traversal
 * If off, emplys or n_employees is NULL, this function should do nothing
 *
 * You will need to provide an allocation to emplys and specify the
 * correct number of employees found in your query.
 */
void office_get_employees_postorder(struct office* off, struct employee** emplys, size_t* n_employees) {
    if(NULL == off || NULL == n_employees || NULL == emplys){
        return;
    }
    Queue *queue = (Queue *)malloc(sizeof(queue));
    Queue *tmp;
    queue->next = NULL;
    queue->value = NULL;
    add_all_to_queue(queue, off->department_head);
    tmp = queue;
    size_t count = 0;
    while(tmp->value != NULL){
        count ++;
        if(tmp->next == NULL){
            break;
        }
        tmp = tmp->next;
    }
    if(count == 0){
        *n_employees = count;
        return;
    }
    (*emplys) = (struct employee *)malloc(sizeof(struct employee) * count);
    *n_employees = count;
    tmp = queue;
    for(int i = 0; i < count; ++i){
        (*emplys)[i].name = (char *)malloc(sizeof(char) * (strlen(queue->value->name) + 1));
        strcpy((*emplys)[i].name, queue->value->name);

        if(NULL != queue->value->supervisor){
            (*emplys)[i].supervisor = queue->value->supervisor;
        }else{
            (*emplys)[i].supervisor = NULL;
        }

        if(NULL != queue->value->subordinates){ 
            (*emplys)[i].subordinates = (queue->value->subordinates);
        }else{
            (*emplys)[i].subordinates = NULL;
        }

        (*emplys)[i].n_subordinates = queue->value->n_subordinates;
        queue = queue->next;

    }
    free_the_queue(tmp);
}

//Done
/**
 * The employee will be promoted to the same level as their supervisor and will
 *  join their supervisor's team.
 * If the employee has members on their team, the first employee from that team
 *   will be promoted to manage that team.
 * if emp is NULL, this function will do nothing
 * if the employee is at level 0 or level 1, they cannot be promoted
 */
void office_promote_employee(struct employee* emp) {
	return;
}

//Done
/**
 * Demotes an employee, placing them under the supervision of another employee.
 * If supervisor or emp is null, nothing should occur
 * If the employee does not supervise anyone, they will not be demoted as they
 *  are already at the lowest position
 * If an employee is to be demoted but their new distance from the boss is less
 *  than the previous position, nothing will happen.
 * Otherwise, the employee should be assigned at the end the supervisor's team
 *  and the first employee from the previously managed team will be promoted.
 *
 * Edge case:
 * if the supervisor use to be an subordinate to the demoted employee
 *   (they will get promoted)
 * the demoted employee will be attached to subordinate's new subordinate's
 *   list not their previous list.
 */
void office_demote_employee(struct employee* supervisor, struct employee* emp){
	return;
}

//Done
/**
 * The office disbands
 * (You will need to free all memory associated with employees attached to
 *   the office and the office itself)
 */
void office_disband(struct office* office) {
    
    if(NULL == office){
        return;
    }
    if(office->department_head == NULL){
        free(office);
        return;
    }
    //By this stage there is at least one employee.
    Queue *queue = (Queue *)malloc(sizeof(queue));
    queue->next = NULL;
    queue->value = NULL;

    add_all_to_queue(queue, office->department_head);
    free_the_queue_employee(queue);
    office->department_head = NULL;

    free_the_queue(queue);
    free(office);
    office = NULL;
}

//Done
int find_index_of_subordinates(struct employee* employee) {
    if(employee->supervisor == NULL){
        return -1;
    }
    int index = 0;
    struct employee *supervisor = employee->supervisor;
    for(int i = 0; i < supervisor->n_subordinates; ++i){
        if(employee == &(supervisor->subordinates[i])){
            index = i; 
        }
        
    }
    return index;
}

//Done
void get_employee_with_no_sub(Queue *queue, Queue *tmp_queue) {
    
    Queue *cursur = tmp_queue;
    Queue *storage = (Queue *)malloc(sizeof(queue));
    storage->next = NULL;
    storage->value = NULL;

    //tmp_queue will store everything in the same level)
    while(NULL != cursur->value){
        if(0 == cursur->value->n_subordinates){
            add_to_queue(queue, cursur->value);
        }
        cursur = cursur->next;
    }

    //By this stage we need to check if their is element in the queue
    if(NULL != queue->value){
        free_the_queue(storage);
        free_the_queue(tmp_queue);
        storage = NULL;
        tmp_queue = NULL;
        return;
    }
    //By this stage their is still nothing in the tmp_queue so we need to update the tmp_queue
    cursur = tmp_queue;
    while(NULL != cursur->value){
        for(int i = 0; i < cursur->value->n_subordinates; ++i){
            add_to_queue(storage, &(cursur->value->subordinates[i]));
        }
        cursur = cursur->next;
    }
    free_the_queue(tmp_queue);
    tmp_queue = storage;
    get_employee_with_no_sub(queue, tmp_queue);
}

// //Done
void get_employee_in_level(Queue *queue, Queue *tmp_queue, size_t level) {
    Queue *cursur = tmp_queue;
    Queue *storage = (Queue *)malloc(sizeof(Queue));
    storage->next = NULL;
    storage->value = NULL;
    size_t depth = 0;
    depth = depth_of_employee(cursur->value);
    //tmp_queue will store everything in the same level)
    while(NULL != cursur->value && (depth == level)){
        add_to_queue(queue, cursur->value);
        cursur = cursur->next;
    }

    //By this stage we need to check if their is element in the queue
    if(NULL != queue->value){
        //printf("NUMBER\n");
        free_the_queue(storage);
        free_the_queue(tmp_queue);
        storage = NULL;
        tmp_queue = NULL;
        return;
    }
    //By this stage their is still nothing in the tmp_queue so we need to update the tmp_queue
    cursur = tmp_queue;
    while(NULL != cursur->value){
        for(int i = 0; i < cursur->value->n_subordinates; ++i){
            add_to_queue(storage, &(cursur->value->subordinates[i]));
        }
        cursur = cursur->next;
    }
    //printf("NUMBER2\n");
    free_the_queue(tmp_queue);
    tmp_queue = storage;
    if(tmp_queue->next == NULL){
        //printf("NUMBER3\n");
        free_the_queue(storage);
        storage = NULL;
        tmp_queue = NULL;
        return;
    }
    get_employee_in_level(queue, tmp_queue, level);
}

void get_employee_with_name(Queue *queue, Queue *tmp_queue, const char* name) {
    Queue *cursur = tmp_queue;
    Queue *storage = (Queue *)malloc(sizeof(Queue));
    storage->next = NULL;
    storage->value = NULL;
    size_t depth = 0;
    depth = depth_of_employee(cursur->value);
    //tmp_queue will store everything in the same level)
    //&& strcmp(name, cursur->value->name) == 0
    while(NULL != cursur->value) {
        if(strcmp(name, cursur->value->name) == 0){
            add_to_queue(queue, cursur->value);
        }
        cursur = cursur->next;
    }


    //By this stage their is still nothing in the tmp_queue so we need to update the tmp_queue
    cursur = tmp_queue;
    while(NULL != cursur->value){
        for(int i = 0; i < cursur->value->n_subordinates; ++i){
            add_to_queue(storage, &(cursur->value->subordinates[i]));
        }
        cursur = cursur->next;
    }
    //printf("NUMBER2\n");
    free_the_queue(tmp_queue);
    tmp_queue = storage;
    if(tmp_queue->next == NULL){
        free_the_queue(storage);
        storage = NULL;
        tmp_queue = NULL;
        return;
    }
    get_employee_with_name(queue, tmp_queue, name);
}

//Done
size_t depth_of_employee(struct employee *employee) {
    struct employee *cursur;
    size_t count = 0;
    if(employee->supervisor == NULL){
        return count;
    }
    cursur = employee;
    while(cursur->supervisor != NULL){
        count ++;
        cursur = cursur->supervisor;
    }
    return count;
}

//Done
void get_employee(Queue *queue, struct employee *tmp, struct employee *supervisor) {
    if(tmp == supervisor){
        add_to_queue(queue, tmp);
        return;
    }
    for(int i = 0; i < tmp->n_subordinates; ++i){
        get_employee(queue, &(tmp->subordinates[i]), supervisor);
    }
    return;
}

//Done
void add_all_to_queue(Queue* queue, struct employee *employee) {

    for(int i = 0; i < employee->n_subordinates; ++i){
        add_all_to_queue(queue, &(employee->subordinates[i]));
    }
    if(NULL != employee){
        add_to_queue(queue, employee);
    }
}

//Done
void add_all_to_queue_level(Queue* queue, struct employee *employee, size_t level) {
    size_t depth = 0;
    depth = depth_of_employee(employee);

    for(int i = 0; i < employee->n_subordinates; ++i){
        add_all_to_queue_level(queue, &(employee->subordinates[i]), level);
    }
    if(NULL != employee && (depth == level)){
        add_to_queue(queue, employee);
    }
}

//Done
void add_to_queue(Queue *queue, struct employee *employee) {
 
    if(queue->value == NULL){
        queue->value = employee;
        queue->next = (Queue *)malloc(sizeof(Queue));
        queue->next->value = NULL;
        queue->next->next = NULL;
        return;
    }

    Queue *cursur = queue;
    while(cursur->next != NULL){
        cursur = cursur->next;
    }
    cursur->value = employee;
    cursur->next = (Queue *)malloc(sizeof(Queue));
    cursur->next->value = NULL;
    cursur->next->next = NULL;
}

//Done
void free_the_queue(Queue *queue) {
    static int a = 0;
    a ++;

    // printf("A = %d\n", a);
    if(queue == NULL) {
        return;
    }

    Queue *tmp = queue;
    //If only one elements
    if(NULL == tmp->next){
        free(queue);
        return;
    }
    //More than two elements
    while(NULL != queue){
        tmp = queue->next;
        free(queue);
        queue = tmp;
    }
}


//Done
void free_the_queue_employee(Queue *queue) {
    Queue *tmp = queue;
    //If only one elements
    // if(NULL == tmp->next){
    //     if(queue)
    //     free(queue->value->name);
    //     free(queue->value);
    //     free(queue);
    //     return;
    // }
    //More than two elements
    while(NULL != queue->value){
        tmp = queue->next;
        for(int i = 0; i < queue->value->n_subordinates; ++i){
            if(NULL != &(queue->value->subordinates[i])){
                if(NULL != queue->value->subordinates[i].name){
                    free(queue->value->subordinates[i].name);
                    queue->value->subordinates[i].name = NULL;
                }                
            }
        }
        if(NULL != queue->value->subordinates){
            free(queue->value->subordinates);
            queue->value->subordinates = NULL;
            queue->value->n_subordinates = 0;
        }
        //Its the last element
        if(NULL == queue->next->value){
            if(NULL != queue->value->name){
                free(queue->value->name);
                queue->value->name = NULL;
            }
            if(NULL != queue->value){
                free(queue->value);
                queue->value = NULL;
            }
        }
        queue = tmp;
    }
}

// #include "office_test.c"


// int main(){
//     int a = 0;
//     a = test_office_place_1();
//     if(a != 0){
//         printf("Test1 Complete\n");
//     }
//     a = 0;
//     a = test_office_place_2();
//     if(a != 0){
//         printf("Test2 Complete\n");
//     }
//     a = 0;
//     a =  test_office_place_2_disband();
//     if(a != 0){
//         printf("Test3 Complete\n");
//     }
// }