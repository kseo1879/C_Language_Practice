#ifndef creator_h
#define creator_h
#include "svc.h"
#include "helper.h"
#include "file_handler.h"

//This will generate commit
void generate_commit(Branch *branch, Commits *commits, char *message, char *id) {

    commits->commit_id = (char *)malloc(sizeof(char) * (strlen(id) +1));
    strcpy(commits->commit_id, id);
    
    
    commits->commit_msg = (char *)malloc(sizeof(char) * (strlen(message) + 1));
    strcpy(commits->commit_msg, message);
    commits->branch_name = branch->branch_name;

    int not_removed_file = 0;

    for(int i = 0; i < branch->n_files; ++i) {
        if(branch->files[i].status != 4) {
            not_removed_file += 1;
        }
    }

    commits->n_files = not_removed_file;
    commits->files = (File *)malloc(sizeof(File) * commits->n_files);

    int index = 0;
    for(int i = 0; i < branch->n_files; ++i) {
        if(branch->files[i].status != 4) {
            //This means we need to keep in track of the files
            if(branch->files[i].status != 3) {
                create_file_with_name(&(branch->files[i]), id);
            }
            commits->files[index].file_name = (char *)malloc(sizeof(char) * 
                (strlen(branch->files[i].file_name) + 1));
            strcpy(commits->files[index].file_name, branch->files[i].file_name);
            commits->files[index].hash_value = branch->files[i].hash_value;
            commits->files[index].status = branch->files[i].status;
            ++index;
        }
    }
    free(id);
}

#endif