#ifndef update_h
#define update_h
#include "svc.h"
#include "helper.h"
#include "calculator.h"
#include "creator.h"

//If There is uncommited changed it will return 0. 
void reset_branch_files(void *helper, char* id) {
    Helper *my_helper = (Helper *)helper;

    for(int i = 0; i < my_helper->head->n_files; ++i) {
        int hash = 0;
        FILE *fptr = fopen(my_helper->head->files[i].file_name, "r");
        //Check if File has been removed
        if(NULL == fptr) {
            //If it was added but deleted later
            copy_file_to_directory(&my_helper->head->files[i], id);
        } else {
            fclose(fptr);
            hash = hash_file(my_helper, my_helper->head->files[i].file_name);
            //If not check if there were any changes to the file after adding it to the system
            if(hash != my_helper->head->files[i].hash_value) {
                if(3 != my_helper->head->files[i].hash_value) {
                    copy_file_to_directory(&my_helper->head->files[i], id);
                }
            }
        }
    } 
}

//This one we need to update the branch after commit
void update_branch_after_commit(Branch * branch) {

    int not_removed_file = 0;

    for(int i = 0; i < branch->n_files; ++i) {
        if(branch->files[i].status != 3 && branch->files[i].status != 4) {
            not_removed_file += 1;
        }
    }

    File *temp = (File *)malloc(sizeof(File) * not_removed_file);
    int count = 0;

    for(int i = 0; i < branch->n_files; ++i) {
        if(branch->files[i].status != 3 && branch->files[i].status != 4) {
            branch->files[i].status = 0;
            temp[count] = branch->files[i];
            ++count;
        } else {
            free(branch->files[i].file_name);
            branch->files[i].file_name = NULL;
        }
    }
    free(branch->files);
    branch->files = temp;
    branch->n_files = not_removed_file;
}

/*
    This function will first update all the file that has been added to the list.
    Then it will return the value if 0 there is no changes and 1 if there was any changes
    If there was any changes to the file
    If There was no changes than the file shou
*/
int check_and_update(void *helper) {
    Helper *my_helper = (Helper *)helper;
    if(0 == my_helper->head->n_files) {
        return 0;
    }

    for(int i = 0; i < my_helper->head->n_files; ++i) {
        int hash = 0;
        FILE *fptr = fopen(my_helper->head->files[i].file_name, "r");
        //Check if File has been removed
        if(NULL == fptr) {
            //If it was added but deleted later
            if(my_helper->head->files[i].status == 2) {
                my_helper->head->files[i].status = 4;
            } else {
                my_helper->head->files[i].status = 3;
            }
        } else {
            fclose(fptr);
            hash = calculate_hash_value_file(my_helper->head->files[i].file_name);
            //If not check if there were any changes to the file after adding it to the system
            if(hash != my_helper->head->files[i].hash_value) {
                if(my_helper->head->files[i].status == 0) {
                    my_helper->head->files[i].status = 1;
                }
            }
        }
        my_helper->head->files[i].hash_value = hash;
    }
    //Check if There were no chagnes made to the commited file
    int count = 0;
    
    for(int i = 0; i < my_helper->head->n_files; ++i) {
        if(my_helper->head->files[i].status == 0) {
            ++count;
        }
    }
    if(count == my_helper->head->n_files) {
        return 0;
    }

    return 1;
} 

#endif