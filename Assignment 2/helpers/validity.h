#ifndef validity_h
#define validity_h

//This will return NULL is the file doesnt exist and if not it will return the filepath to the resolution
char *file_exist_in_resolution(char *file_name, struct resolution *resolutions, int n_resolutions) {
    int exist = 0;
    int index = 0;
    for(int i = 0; i < n_resolutions; ++i) {
        if(0 == strcmp(resolutions[i].file_name, file_name)) {
            exist = 1;
            index = i;
        }
    }
    //If the file doesn't exist
    if(0 == exist) {
        return NULL;
    } else {
        return resolutions[index].resolved_file;
    }
}

/*
If branch has file should return 0
If it doesnt it will return 1
*/
int branch_has_file(Branch *branch, char *name) {
    for(int i = 0; i < branch->n_files; ++i) {
        if(strcmp(branch->files[i].file_name, name) == 0) {
            //This means that there is already a file
            return 0;
        }
    } 
    return 1;
}

/*
If branch exsit it should return 0
If it doesnt it will return 1
*/
int branch_exist(void *helper, char *name) {
    Helper *assist = (Helper *)helper;

    //This should not happen. It should be called only after init
    if(0 == assist->n_branch) {
        return 0;
    }

    for(int i = 0; i < assist->n_branch; ++i) {
        if(strcmp(name, assist->branch_list[i].branch_name) == 0) {
            return 0;
        }
    }

    return 1;
}

/*
* This function will return if the branch name is valid 1
* If not valid 0
*/
int valid_name(char* name){
  for(int i = 0; i < strlen(name); i++) {
    if((name[i] >= 'a' && name[i] <= 'z') ||  
        (name[i] >= 'A' && name[i] <= 'Z') ||
        (name[i] >= '0' && name[i] <= '9') ||
        (name[i] == '_') || (name[i] == '/') || (name[i] == '-')) {

        continue;

    } else {
        return 0;
    }
  }
  return 1;
} 

int check_for_uncommited_changes(void *helper) {
    Helper *my_helper = (Helper *)helper;

    if(0 == my_helper->head->n_files) {
        return 0;
    }

    for(int i = 0; i < my_helper->head->n_files; ++i) {
        int hash = 0;
        FILE *fptr = fopen(my_helper->head->files[i].file_name, "r");
        //Check if File has been removed
        if(NULL == fptr) {
            return 0;
        } else {
            fclose(fptr);
            hash = hash_file(my_helper, my_helper->head->files[i].file_name);
            //If not check if there were any changes to the file after adding it to the system
            if(hash != my_helper->head->files[i].hash_value || 0 != my_helper->head->files[i].status) {
                return 0;
            }
        }
    }
    return 1;
}

int check_for_uncommited_changes_merge(void *helper, Branch *other_branch) {
    Helper *my_helper = (Helper *)helper;

    if(0 == my_helper->head->n_files) {
        return 0;
    }

    for(int i = 0; i < my_helper->head->n_files; ++i) {
        FILE *fptr = fopen(my_helper->head->files[i].file_name, "r");
        //Check if File has been removed
        if(NULL == fptr) {

            int exist = 0;

            if(0 == my_helper->head->files[i].status) {
                for(int j = 0; j < other_branch->n_files; ++ j) {
                    if(0 == strcmp(my_helper->head->files[i].file_name, other_branch->files[j].file_name)) {
                        exist = 1;
                    }
                }
                //This means that the files are being tracked on both branches.
                if(1 == exist) {
                    return 0;
                }
            } else {
                continue;
            }
        } else {
            fclose(fptr);
            //If not check if there were any changes to the file after adding it to the system
            if(0 != my_helper->head->files[i].status) {
                return 0;
            }
        }
    }
    return 1;
}

#endif