#ifndef file_handler_h
#define file_handler_h
#include "svc.h"
#include "helper.h"
#include "creator.h"
#include <string.h>


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

char *string_lower(char *character) {
    char *tmp = (char *)malloc(sizeof(char) * ((strlen(character)+1)));
    for(int i = 0; i < strlen(character); ++i) {
        if(character[i] >= 'A' && character[i] <= 'Z') {
            tmp[i] = character[i] + 32;
        } else {
            tmp[i] = character[i];
        }
    }
    tmp[strlen(character)] = '\0';
    return tmp;
}

/*
    This function will return the index of newly adding files in alphabetical order.
*/ 
int alphabetical_order(void *helper, char *file_name) {
    Helper* my_helper = (Helper *)helper;
    char *nameA = NULL;
    char *nameB = NULL;
    int a = 0;
    //First Find the index of the file that needs to be added
    for(int i = 0; i < my_helper->head->n_files; ++i){
        nameA = string_lower(file_name);
        nameB = string_lower(my_helper->head->files[i].file_name);
        if(strcmp(nameA, nameB) > 0) {
            ++a;
        }
        free(nameA);
        free(nameB);
    }
    
    return a;
}

#endif