#ifndef file_handler_h
#define file_handler_h
#define MAX_LEN 300

//This will copy the file is there is no file during merge
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

//This will copy the file from the previous commit
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

//This will create file
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

//This funciton will return the alphabetically lower version of the file
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

//This function will return the index of newly adding files in alphabetical order.
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

//This will find the index of new file added
int find_index_of_file(void *helper, char *file_name) {
    Helper *my_helper = (Helper *)helper;
    for(int i = 0; i < my_helper->head->n_files; ++i) {
        if(0 == strcmp(my_helper->head->files[i].file_name, file_name)){
            return i;
        }
    }
    return 0;
}

#endif