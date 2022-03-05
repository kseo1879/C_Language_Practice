#ifndef calculator_h
#define calculator_h
#include "svc.h"
#include "helper.h"
#define HEXA_DIGIT 7
//Hexa Degit should be four but since we are creating a 

int calculate_hash_value_file(char *file_path);
char *get_commit_id(Branch *branch, char *message);

//This function will return the hexadecimal representation of the Commit id. 
char *get_commit_id(Branch *branch, char *message) {
    int id = 0;

    for(int i = 0; i < strlen(message); ++i) {
        id = (id + message[i]) % 1000;
    }
    for(int i = 0; i < branch->n_files; ++i) {
        if(branch->files[i].status == 1){
            id += 9573681;
        }
        else if(branch->files[i].status == 2){
            id += 376591;
        }
        else if(branch->files[i].status == 3){
            id += 85973;
        }
        if(branch->files[i].status == 0 || branch->files[i].status == 4) {
            continue;
        } else {
            for(int j = 0; j < strlen(branch->files[i].file_name); ++j) {
                id = (id * (((int)branch->files[i].file_name[j]) % 37)) % 15485863 + 1 ;
            }
        }
    }
    char *hexa = (char *)malloc(sizeof(char) * HEXA_DIGIT);
    sprintf(hexa, "%x", id);
    hexa[HEXA_DIGIT-1] = '\0';


    if(strlen(hexa) != HEXA_DIGIT -1) {
        char* temp = (char*) malloc(sizeof(char)*strlen(hexa) + 1);
        strcpy(temp, hexa);
        for(int k = 0; k < HEXA_DIGIT - strlen(temp) - 1; ++k){
            hexa[k] = '0';
        }
        int count = 0;
        for(int i = HEXA_DIGIT - strlen(temp) - 1 ; i < HEXA_DIGIT - 1; ++i) {
            hexa[i] = temp[count];
            ++count;
        }
        hexa[HEXA_DIGIT-1] = '\0';
        free(temp);
        return hexa;
    }
    return hexa;
}

int calculate_hash_value_file(char *file_path) {

    int character;
    int hash_value = 0;
    int name = 0;
    int contents = 0;
    FILE* fptr = fopen(file_path, "r");

    //Update the value for name of the file
    for(int i = 0; i < strlen(file_path); i++){
        name = (name + file_path[i]);
    }

    //Update value of contenst in the file
    while((character = fgetc(fptr))!= EOF){
        contents = (contents + character);
    }

    fclose(fptr);

    hash_value = name % 1000;
    hash_value = (hash_value + contents) % 2000000000;
    
    return hash_value;
}

#endif