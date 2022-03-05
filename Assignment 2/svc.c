#include "helpers/svc.h"
#include "helpers/helper.h"
#include "helpers/cleaner.h"
#include "helpers/calculator.h"
#include "helpers/validity.h"
#include "helpers/creator.h"
#include "helpers/file_handler.h"
#include "helpers/update.h"

/*
* There are multiple functions that are declared in the library above.
* For the convinience of reading this code here is the list of additional
* functions declared in each header file
*
*           ====helpers/cleaner.h====
*
*   void clean_file(File *file)
*   void clean_lost_commit(Commits *commits)
*   void clean_commit(Commits *commits, char *branch_name)
*   void clean_branch(Branch *branch)
*   void memory_cleaner(void *data)
*
*   Description: This headerfile funcitons are used when cleanup is called.
*   It will cleanup the heap mememory allocated throughout the iteration.
*
*           ====helpers/calculator.h====
*
*   char *get_commit_id(Branch *branch, char *message)
*   int calculate_hash_value_file(char *file_path)
*
*   Description: This headerfile will calculate demanding values.
*
*           ====helpers/validity.h====
*
*   char *file_exist_in_resolution(char *file_name, struct resolution *resolutions, int n_resolutions)
*   int branch_has_file(Branch *branch, char *name)
*   int branch_exist(void *helper, char *name) 
*   int valid_name(char* name)
*   int check_for_uncommited_changes(void *helper)
*   int check_for_uncommited_changes_merge(void *helper, Branch *other_branch)
*
*   Description: This headerfile funcitons are used usually when error checking
*   or to some other extent in specific situation
*   
*           ====helpers/creator.h====
*
*   void generate_commit(Branch *branch, Commits *commits, char *message, char *id)
*
*   Description: This headerfile will help create commit
*
*           ====helpers/file_handler.h====
*
*   void copy_file_when_merge(char *source, char *destination)
*   void copy_file_to_directory(File *file, char *id)
*   void create_file_with_name(File *file, char *id)
*   char *string_lower(char *character)
*   int alphabetical_order(void *helper, char *file_name)
*   int find_index_of_file(void *helper, char *file_name)
*
*   Description: This headerfile funcitons are used when creating file, copying them
*   and has functions to help decide the index of the correct file addition to the 
*   staging area.
*
*           ====helpers/update.h====
*   void reset_branch_files(void *helper, char* id)
*   void update_branch_after_commit(Branch * branch)
*   int check_and_update(void *helper)
*
*   Description: This headerfile will help update the nesessary data after commits happens.
*
* ---------------------End of declarations---------------------
*/

void *svc_init(void) {
    Helper *helper = (Helper *)malloc(sizeof(Helper));

    //Initializing Helper
    helper->branch_list = (Branch *)malloc(sizeof(Branch));
    helper->n_branch = 1;

    
    //Initializing Branch With Master Branch
    
    helper->branch_list[0].branch_name = (char *)malloc(sizeof(char) *(strlen("master") + 1));
    helper->branch_list[0].files = NULL;
    helper->branch_list[0].n_files = 0;
    helper->branch_list[0].head = NULL; 
    helper->branch_list[0].n_commit = 0;
    strcpy(helper->branch_list[0].branch_name , "master");


    //Initializing Head
    helper->head = &(helper->branch_list[0]);
    helper->lost_commits = NULL;
    helper->n_lost = 0;
    return helper;
}


void cleanup(void *helper) {
    //Defined in headerfile "cleaner.h" under ./helpers/;
    memory_cleaner(helper);
}


int hash_file(void *helper, char *file_path) {

    int hash_value = 0;

    if(NULL == file_path) {
        return -1;
    }

    FILE *fptr = fopen(file_path, "r");
    
    if(NULL == fptr) {
        return -2;
    }

    fclose(fptr);

    hash_value = calculate_hash_value_file(file_path);

    return hash_value;
}


char *svc_merge(void *helper, char *branch_name, struct resolution *resolutions, int n_resolutions) {
    //If Branch Name is NULL
    
    if(NULL == branch_name || NULL == helper) {
        printf("Invalid branch name\n");
        return NULL;
    }

    Helper *my_helper = (Helper *)helper;
    int validity = branch_exist(helper, branch_name);
    if(0 != validity) {
        printf("Branch not found\n");
        return NULL;
    }

    if(0 == strcmp(my_helper->head->branch_name, branch_name)) {
        printf("Cannot merge a branch with itself\n");
        return NULL;
    }

    Branch *active_branch = my_helper->head;
    Branch *merging_branch = NULL;
    

    for(int i = 0; i < my_helper->n_branch; ++i){
        if(0 == strcmp(branch_name, my_helper->branch_list[i].branch_name)){
            merging_branch = &(my_helper->branch_list[i]);
        }
    }

    if(0 == check_for_uncommited_changes_merge(helper, merging_branch)) {
        printf("Changes must be committed\n");
        return NULL;
    }
    //This is going to be the branch that is found to be merged in the head. 

    if(0 == merging_branch->n_commit) {
        printf("There is not commit to merge");
        return NULL;
    }

    int result = 0;
    int index = 0;
    for(int i = 0; i < merging_branch->n_files; ++i) {
        result = svc_add(helper, merging_branch->files[i].file_name);
        //This means that there is a file that exist in both of the branch. 
        if(-2 == result) {
            //Now we need to check if the hash value from both of the tree is the same value
            //It will return the hash value because we have already done the handling
            index = find_index_of_file(my_helper, merging_branch->files[i].file_name);
            //Check if the hash_value is same from the file_system that we have saved.
            //This means that it is conflicting
            if(active_branch->files[index].hash_value != merging_branch->files[i].hash_value) {
                //check if the resolution has file. If not it should be deleted from the file system
                char *resolution_file_path = NULL;
                resolution_file_path = file_exist_in_resolution(merging_branch->files[i].file_name, resolutions, n_resolutions);
                if(NULL == resolution_file_path) {
                    //File should be removed (Uncertain yet)
                    svc_rm(my_helper, merging_branch->files[i].file_name);
                } else {
                    //Make file to be modified,
                    copy_file_when_merge(resolution_file_path, merging_branch->files[i].file_name);
                }
            } else {
                //Even if it doesn't exist if their is a resolution file it should add
                if(active_branch->files[index].hash_value != hash_file(helper, merging_branch->files[i].file_name)) {
                    //printf("This was why you had \n\n");
                    copy_file_to_directory(&(active_branch->files[index]), active_branch->head->commit_id);
                }
                for(int j = 0; j < n_resolutions; j ++) {
                    if(0 == strcmp(merging_branch->files[i].file_name, resolutions[j].file_name)) {
                        if(NULL == resolutions[j].resolved_file) {
                            svc_rm(helper, resolutions[j].file_name);
                        } else {
                            copy_file_when_merge(resolutions[j].resolved_file, merging_branch->files[i].file_name);
                        }
                    }
                }
            }
        } else {
            char *resolution_file_path = NULL;
            resolution_file_path = file_exist_in_resolution(merging_branch->files[i].file_name, resolutions, n_resolutions);
            if(NULL != resolution_file_path) {
                copy_file_when_merge(resolution_file_path, merging_branch->files[i].file_name);
            } else {
                int deleted = -1;
                for(int k = 0; k < active_branch->head->n_files; ++k) {
                    if(0 == strcmp(active_branch->head->files[k].file_name, merging_branch->files[i].file_name)) {
                        deleted = k;
                    }
                }
                if(deleted != -1) {
                    if(3 != active_branch->head->files[deleted].status) {
                        copy_file_to_directory(&merging_branch->files[i], merging_branch->head->commit_id);
                        svc_add(helper, merging_branch->files[i].file_name);
                    } else {
                        copy_file_to_directory(&merging_branch->files[i], merging_branch->head->commit_id);
                        svc_add(helper, merging_branch->files[i].file_name);
                        int deleing_index = -1;
                        for(int q = 0; q < active_branch->n_files; ++q) {
                            if(0 == strcmp(active_branch->files[q].file_name, merging_branch->files[i].file_name)) {
                                deleing_index = q;
                            }
                        }
                        active_branch->files[deleing_index].status = 4;
                    }
                } else {
                    copy_file_to_directory(&merging_branch->files[i], merging_branch->head->commit_id);
                    svc_add(helper, merging_branch->files[i].file_name);
                }
            }
        }  
    }

    char *commit_id = NULL;
    char commit_msg[MAX_LEN] = "Merged branch ";
    strcat(commit_msg, merging_branch->branch_name);
    commit_id = svc_commit(my_helper, commit_msg);
    Commits *my_commit = get_commit(my_helper, commit_id);
    my_commit->n_parents = 2;
    my_commit->address_parents = (Commits **)malloc(sizeof(Commits*) * 2);
    my_commit->address_parents[0] = my_helper->head->head->parents;
    my_commit->address_parents[1] = merging_branch->head;
    printf("Merge successful\n");
    return commit_id;
}


void *get_commit(void *helper, char *commit_id) {
    if(NULL == commit_id || NULL == helper) {
        return NULL;
    }
    Helper *my_helper = (Helper *)helper;
    Commits *commits = NULL;
    Commits *head = NULL;
    
    for(int i = 0; i < my_helper->n_branch; ++i) {
        head = my_helper->branch_list[i].head;
        for(int j = 0; j < my_helper->branch_list[i].n_commit; ++j) {
            if(0 == strcmp(head->commit_id, commit_id)) {
                commits = head;
                break;   
            }
            if(NULL!= head->parents) {
                head = head->parents;
            }
        }
    }
    
    if(NULL != commits) {
        return commits;
    }
    
    return NULL;
}


char **get_prev_commits(void *helper, void *commit, int *n_prev) {

    if(NULL == n_prev) {
        return NULL;
    }

    if(NULL == helper || NULL == commit) {
        *n_prev = 0;
        return NULL;
    }

    Commits *my_commit = (Commits *)commit;
    if(0 == my_commit->n_parents|| NULL == my_commit->parents) {
        *n_prev = 0;
        return NULL;
    }

    if(1 == my_commit->n_parents) {
        *n_prev = 1;
        char **name = (char **)malloc(sizeof(char *));
        name[0] = my_commit->parents[0].commit_id;
        return name;
    }

    if(2 == my_commit->n_parents) {
        *n_prev = 2;
        char **name = (char **)malloc(sizeof(char *) * 2);
        name[0] = my_commit->address_parents[0]->commit_id;
        name[1] = my_commit->address_parents[1]->commit_id;
        return name;
    }

    return NULL;
}


void print_commit(void *helper, char *commit_id) {
    if(NULL == commit_id) {
        printf("Invalid commit id\n");
        return;
    }
    Commits *my_commits = get_commit(helper, commit_id);
    if(NULL == my_commits) {
        printf("Invalid commit id\n");
        return;
    }

    int tracked_files = 0;

    printf("%s [%s]: %s\n", my_commits->commit_id, my_commits->branch_name, my_commits->commit_msg);

    for(int i = 0; i < my_commits->n_files; ++i){
        if(my_commits->files[i].status == 2){
          printf("    + %s\n", my_commits->files[i].file_name);
        }
    }
    
    for(int i = 0; i < my_commits->n_files; ++i){
        if(my_commits->files[i].status == 3){
          printf("    - %s\n", my_commits->files[i].file_name);
        } else {
            ++tracked_files;
        }
    }

    for(int i = 0; i < my_commits->n_files; ++i){
        if(my_commits->files[i].status == 1){
          printf("    / %s\n", my_commits->files[i].file_name);
        }
    }
    printf("\n");
    printf("    Tracked files (%d):\n",tracked_files);
    for(int i = 0; i < my_commits->n_files; ++i) {
        if(my_commits->files[i].status != 3){
            printf("    [%10d] %s\n", my_commits->files[i].hash_value, my_commits->files[i].file_name);
        }
    }
    return;
}


int svc_branch(void *helper, char *branch_name) {
    if(NULL == branch_name || NULL == helper) {
        return -1;
    }
    //valid_name is under header file validity
    int validity = valid_name(branch_name);

    if(0 == validity) {
        return -1;
    }
    validity = branch_exist(helper, branch_name);
    //Check if the name has already been occupied
    if(0 == validity) {
        return -2;
    }
    int condition = 0;

    condition = check_for_uncommited_changes(helper);

    if(0 == condition) {
        return -3;
    }
    
    Helper *my_helper = (Helper *)helper;
    //Getting The address of previous helper
    int index = 0;
    for(int i = 0; i < my_helper->n_branch; ++i) {
        if(my_helper->head == &(my_helper->branch_list[i])) {
            index = i;
        }
    }

    my_helper->n_branch += 1;
    my_helper->branch_list = (Branch *)realloc(my_helper->branch_list, sizeof(Branch) * my_helper->n_branch);
    my_helper->branch_list[my_helper->n_branch-1].branch_name = (char *)malloc(strlen(branch_name) + 1);
    strcpy(my_helper->branch_list[my_helper->n_branch-1].branch_name, branch_name);
    my_helper->head = &(my_helper->branch_list[index]);
    //Pointing to the same commited File in the begining
    if(NULL != my_helper->head->head) {
        my_helper->branch_list[my_helper->n_branch-1].head = my_helper->head->head;
    }

    //Doing a File Copy
    my_helper->branch_list[my_helper->n_branch-1].n_files = my_helper->head->n_files;
    my_helper->branch_list[my_helper->n_branch-1].files = (File *)malloc(sizeof(File) * my_helper->head->n_files);
    for(int i = 0; i < my_helper->head->n_files; ++i) {
        
        my_helper->branch_list[my_helper->n_branch-1].files[i].file_name = (char *)malloc(sizeof(char) * 
            (strlen(my_helper->head->files[i].file_name) + 1) );

        strcpy(my_helper->branch_list[my_helper->n_branch-1].files[i].file_name, my_helper->head->files[i].file_name);
        my_helper->branch_list[my_helper->n_branch-1].files[i].hash_value = my_helper->head->files[i].hash_value;
        my_helper->branch_list[my_helper->n_branch-1].files[i].status = my_helper->head->files[i].status;
        // printf("We are saving file %s\n", my_helper->head->files[i].file_name);
    }
    my_helper->branch_list[my_helper->n_branch-1].n_commit = 0;

    return 0;
}


int svc_checkout(void *helper, char *branch_name) {
    Helper *my_helper = (Helper *)helper;
    if(NULL == branch_name || 0 == my_helper->n_branch) {
        return -1;
    }
    int validity = 0;
    validity = branch_exist(helper, branch_name);
    //This means that the branch does not exist.
    if(validity != 0) {
        return -1;
    }

    validity = check_for_uncommited_changes(helper);

    if(validity == 0) {
        return -2;
    }
    
    int index = 0;
    
    for(int i = 0; i < my_helper->n_branch; ++i) {
        if(0 == strcmp(my_helper->branch_list[i].branch_name, branch_name)) {
            index = i;
        }
    }

    for(int i = 0; i < my_helper->head->n_files; ++i) {
        remove(my_helper->head->files[i].file_name);
    }

    my_helper->head = &(my_helper->branch_list[index]);

    for(int i = 0 ; i < my_helper->head->head->n_files; ++i) {
        if(3 != my_helper->head->head->files[i].status) {
            copy_file_to_directory(&my_helper->head->head->files[i], my_helper->head->head->commit_id);
        }
    }
    return 0;
}


char **list_branches(void *helper, int *n_branches) {
    
    Helper *my_helper = (Helper *)helper;

    if(NULL == n_branches) {
        return NULL;
    }
    char **names = (char **)malloc(sizeof(char *) * my_helper->n_branch);
    *n_branches = (int)my_helper->n_branch;

    for(int i = 0; i < my_helper->n_branch; ++i) {
        names[i] = my_helper->branch_list[i].branch_name;
        printf("%s\n",names[i]);
    }

    return names;
}


int svc_add(void *helper, char *file_name) {
    int condition = -1;
    if(NULL == file_name || NULL == helper) {
        return -1;
    }

    Helper *my_helper = helper;

    //We first need to check if the file name exist in the path. 
    if(NULL != my_helper->head->files && 0 != my_helper->head->n_files) {
        //Branch_has_file is declared in validity.h
        condition = branch_has_file(my_helper->head, file_name);
        //This means that there is already a file here
        if(0 == condition) {
            return -2;
        }
    }

    condition = hash_file(my_helper, file_name);

    if(-2 == condition) {
        return -3;
    }
    
    //We need to add the file (needs to be malloced)
    if(0 == my_helper->head->n_files) {
        my_helper->head->n_files = 1;
        my_helper->head->files = (File *)malloc(sizeof(File));
        my_helper->head->files->file_name = (char *)malloc(sizeof(char)* (strlen(file_name) + 1));
        strcpy(my_helper->head->files->file_name, file_name);
        my_helper->head->files->status = 2;
        my_helper->head->files->hash_value = condition;
    } else { //If there was more than one file that has been store (it needs to be store alphabetically)
        int index = alphabetical_order(my_helper, file_name);
        my_helper->head->n_files += 1;

        File *tmp = (File *)malloc(sizeof(File) * my_helper->head->n_files);
    
        int original_index = 0;
        for(int i = 0; i < my_helper->head->n_files; ++i) {
            if(i == index) {
                tmp[i].file_name = (char *)malloc(sizeof(char)* (strlen(file_name) + 1)); 
                strcpy(tmp[i].file_name, file_name);
                tmp[i].status = 2;
                tmp[i].hash_value = condition;
            } else {
                tmp[i] = my_helper->head->files[original_index];
                ++original_index;
            }
        }
        free(my_helper->head->files);
        my_helper->head->files = tmp;
    }


    return condition;
}


int svc_rm(void *helper, char *file_name) {

    if(NULL == file_name) {
        return -1;
    }

    Helper *my_helper = (Helper *)helper;

    for(int i = 0 ; i < my_helper->head->n_files; ++i) {
        if(0 == strcmp(my_helper->head->files[i].file_name, file_name)) {
            if(3 != my_helper->head->files[i].status) {
                my_helper->head->files[i].status = 3;
                return my_helper->head->files[i].hash_value;
            }
        }
    }
    return -2;
}


int svc_reset(void *helper, char *commit_id) { 

    if(NULL == commit_id || NULL == helper) {
        return -1;
    }

    Helper *my_helper = (Helper *)helper;
    Commits *commits = NULL;
    Commits *head = my_helper->head->head;
    

    while(NULL != head) {
        if(0 == strcmp(head->commit_id, commit_id)) {
            commits = head;
            break;   
        }
        if(NULL!= head->parents) {
            head = head->parents;
        } else {
            break;
        }
    }

    if(NULL == commits) {
        return -2;
    }

    int n_commits_to_delte = 0;
    head = my_helper->head->head;
    char *branch_name = my_helper->head->branch_name;


    while(NULL != head) {
        if(0 == strcmp(head->commit_id, commit_id)) {
            commits = head;
            break;   
        }
        if(NULL!= head->parents) {
            if(0 == strcmp(my_helper->head->branch_name, branch_name)) {
                ++n_commits_to_delte;
                --my_helper->head->n_commit;
            }
            head = head->parents;
        } else {
            break;
        }
    }

    /*
        First of all we need to store the lost contents only if the current branch's 
        n_Commit is bigger than 0 (If it has any commits done)
    */
    if(0 != n_commits_to_delte) {
        if(0 == my_helper->n_lost) {
            my_helper->lost_commits = (Commits **)malloc(sizeof(Commits*) * 1);
            my_helper->lost_commits[0] = my_helper->head->head;
            ++my_helper->n_lost;
            my_helper->n_commits_lost = (int *)malloc(sizeof(int));
            my_helper->n_commits_lost[0] = n_commits_to_delte;
        } else {
            ++my_helper->n_lost;
            my_helper->lost_commits = (Commits **)realloc(my_helper->lost_commits, sizeof(Commits*) * my_helper->n_lost);
            my_helper->lost_commits[my_helper->n_lost-1] = my_helper->head->head;
            my_helper->n_commits_lost = (int *)malloc(sizeof(int) * my_helper->n_lost);
            my_helper->n_commits_lost[my_helper->n_lost - 1] = n_commits_to_delte;
        }
    }

    //Now We need to update the current branches contents to that content;
    my_helper->head->head = commits;

    //First Free the files that the branch used to have
    for(int i = 0; i < my_helper->head->n_files; ++i) {
        clean_file(&my_helper->head->files[i]);
    }
    free(my_helper->head->files);
    my_helper->head->n_files = commits->n_files;
    my_helper->head->files = (File *)malloc(sizeof(File) * commits->n_files);

    for(int i = 0; i < my_helper->head->n_files; ++i) {
        
        my_helper->head->files[i].file_name = (char *)malloc(sizeof(char) * 
            (strlen(commits->files[i].file_name) + 1) );

        strcpy(my_helper->head->files[i].file_name, commits->files[i].file_name);
        my_helper->head->files[i].hash_value = commits->files[i].hash_value;
        my_helper->branch_list[my_helper->n_branch-1].files[i].status = commits->files[i].status;
    }
    reset_branch_files(helper, commits->commit_id);
    update_branch_after_commit(my_helper->head);

    return 0;
}


char *svc_commit(void *helper, char *message) {

    if(NULL == message || NULL == helper) {
        return NULL;
    }
    
    Helper *my_helper = (Helper *)helper;

    int condition = check_and_update(my_helper);

    //This means that there were no changes to the tracked file no new file or deletion
    if(0 == condition) {
        return NULL;
    }
    //By This stage There should be no return NULL state.
    Branch *current_branch = my_helper->head;

    if(NULL == current_branch->head) {
        //We need to initialize the commit.
        current_branch->head = (Commits *)malloc(sizeof(Commits));
        current_branch->head->parents = NULL;
        current_branch->head->n_parents = 0;
        current_branch->n_commit = 1;
    } else {   
        Commits *tmp = (Commits *)malloc(sizeof(Commits));
        tmp->parents = current_branch->head;
        current_branch->head = tmp;
        current_branch->head->n_parents = 1;
        current_branch->n_commit += 1;
    }
    char *id = NULL;
    //This is defined in Calculator headerfile

    id = get_commit_id(current_branch, message);

    // printf("Current Branch name of commit is %s\n", id); 

    //That is method is defined in creator.h
    generate_commit(current_branch, current_branch->head, message, id);
    //This is also update in update.h
    //This funtion will changed the file status of staged file to 0 and and it will also 
    //delete the deleted file
    update_branch_after_commit(current_branch);

    return current_branch->head->commit_id;
}

