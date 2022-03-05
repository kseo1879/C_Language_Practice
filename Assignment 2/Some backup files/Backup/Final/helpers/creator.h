#ifndef creator_h
#define creator_h
#include "svc.h"
#include "helper.h"
#define MAX_LEN 300


void copy_file_when_merge(char *source, char *destination) {
    int count;
    char buffer[5] = {0, };

    FILE *src = fopen(source, "rb");
    FILE *dest = fopen(destination, "wb");

    while (feof(src) == 0)
    {
        count = fread(buffer, sizeof(char), 4, src);
        fwrite(buffer, sizeof(char), count, dest);
        memset(buffer, 0, 5);
    }

    fclose(dest);
    fclose(src);

    return;
}

// commits/123456_(file_path)
void copy_file_to_directory(File *file, char *id) {
    int count;
    char buffer[5] = {0, };
    char *changed_file_name = (char *)malloc(sizeof(char) * (strlen(file->file_name)+1));
    strcpy(changed_file_name, file->file_name);

    for(int i = 0; i < strlen(changed_file_name); ++i) {
        if(changed_file_name[i] == '/') {
            changed_file_name[i] = '?';
        }
    }

    char dest_file_name[MAX_LEN] = {0, };
    strcat(dest_file_name, id);
    strcat(dest_file_name, "_");
    strcat(dest_file_name, changed_file_name);


    FILE *src = fopen(dest_file_name, "rb");
    FILE *dest = fopen(file->file_name, "wb");

    while (feof(src) == 0)
    {
        count = fread(buffer, sizeof(char), 4, src);
        fwrite(buffer, sizeof(char), count, dest);
        memset(buffer, 0, 5);
    }

    fclose(dest);
    fclose(src);
    free(changed_file_name);

    return;
}

void create_file_with_name(File *file, char *id) {
    int count;
    char buffer[5] = {0, };
    char *changed_file_name = (char *)malloc(sizeof(char) * (strlen(file->file_name)+1));
    strcpy(changed_file_name, file->file_name);

    for(int i = 0; i < strlen(changed_file_name); ++i) {
        if(changed_file_name[i] == '/') {
            changed_file_name[i] = '?';
        }
    }

    char dest_file_name[MAX_LEN] = {0, };
    strcat(dest_file_name, id);
    strcat(dest_file_name, "_");
    strcat(dest_file_name, changed_file_name);


    FILE *src = fopen(file->file_name, "rb");
    FILE *dest = fopen(dest_file_name, "wb");

    while (feof(src) == 0)
    {
        count = fread(buffer, sizeof(char), 4, src);
        fwrite(buffer, sizeof(char), count, dest);
        memset(buffer, 0, 5);
    }

    fclose(dest);
    fclose(src);
    free(changed_file_name);

    return;
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