#ifndef cleaner_h
#define cleaner_h
#include "svc.h"
#include "helper.h"

//Function Declarations
void clean_file(File *file);
void clean_commit(Commits *commits, char *branch_name);
void clean_branch(Branch *branch);
void memory_cleaner(void *data);

void clean_file(File *file) {
    file->hash_value = 0;
    file->status = 0;
    if(NULL != file->file_name) {
        free(file->file_name);
    }
}

void clean_lost_commit(Commits *commits) {
    Commits *tmp = commits;
    char *branch_name =commits->branch_name;
    while(0 == strcmp(commits->branch_name, branch_name)) {
        free(commits->commit_id);
        free(commits->commit_msg);
        for(int i = 0; i < commits->n_files; ++i) {
            clean_file(&(commits->files[i]));
        }
        free(commits->files);
        //Files are going to be handles in branches not in commits
        if(commits->parents == NULL) {
            free(commits);
            break;
        } else {
            commits = commits->parents;
            free(tmp);
            tmp = commits;
        }
    }
}

/*
    char *commit_id;
    char *commit_msg;
    char *branch_name;
    File *files; 
    size_t n_files;
    Commits *parents;
    size_t n_child;
*/
void clean_commit(Commits *commits, char *branch_name) {
    //As long as the commits are made in the same commits;
    if(NULL == commits) {
        return;
    }
    free(commits->commit_id);
    free(commits->commit_msg);
    for(int i = 0; i < commits->n_files; ++i) {
        clean_file(&(commits->files[i]));
    }
    free(commits->files);
    if(commits->n_parents == 2) {
        free(commits->address_parents);
    }
}

void clean_branch(Branch *branch) {
    //First of all free the commits
    if(0 != branch->n_commit) {
        
        for(int i = 0; i < branch->n_commit; ++i) {
            Commits *tmp = branch->head;
            //if(strcmp(branch->branch_name, branch->head->branch_name)) {
            clean_commit(branch->head, branch->branch_name);
            branch->head = branch->head->parents;
            free(tmp);
            //}
        }
    }

    if(0 != branch->n_files) {
        for(int i = 0; i < branch->n_files; i ++) {
            clean_file(&(branch->files[i]));
        }
    }
    free(branch->files);
}

void memory_cleaner(void *data) {
    if(NULL == data) {
        return;
    }
    Helper *helper = (Helper *)data;

    if(0 != helper->n_branch) {
        for(int i = (int)helper->n_branch -1 ; i >= 0; --i) {
            clean_branch(&(helper->branch_list[i]));
        }
    }
    //Clean the lost commits after

    if(0 != helper->n_lost) {
        for(int i = 0; i < helper->n_lost; ++i) {
            Commits *tmp = helper->lost_commits[i];
            Commits *head = tmp;
            for(int j = 0; j < helper->n_commits_lost[i]; ++j) {
                clean_commit(head, "Nothing");
                head = head->parents;
                free(tmp);
                tmp = head;
            }
        } 
        free(helper->lost_commits);
        free(helper->n_commits_lost);
    }

    if(0 != helper->n_branch) {
        for(int i = 0; i < (int)helper->n_branch; ++i) {
            free(helper->branch_list[i].branch_name);
        }
    }

    free(helper->branch_list);
    helper->branch_list = NULL;
    helper->head = NULL;
    helper->n_branch = 0;
    free(helper);

    //First We need to clean the file as we go through the 

    return;
}

#endif