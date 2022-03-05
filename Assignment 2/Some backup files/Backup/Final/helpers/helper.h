#ifndef helper_h
#define helper_h
#include <stdio.h>
#include <string.h>

typedef struct commits Commits;


/*
This is a struct of file has it's own
Status is the condition of the file.
Adding/removing/changing (However after the commit every one of this status will change to 0)
  0 = Tracked (Means that this file has been added to the Commit)
  1 = changed (It was being controlled by the previous commit but there was changed)
  2 = added(Newly added to the staging area)
  3 = removed(File has recently been removed from the staging area however it hasn't yet been commited)
*/
typedef struct file {
    char *file_name;
    int hash_value;
    int status;
}File;



/*
This will have the commits that has been added to the struct
*/
typedef struct commits {
    char *commit_id;
    char *commit_msg;
    //Branch_name should be malloced in branch not commits
    char *branch_name;
    File *files; 
    size_t n_files;
    Commits *parents;
    size_t n_parents;
    //This will only not be null when the number of the parents is 2
    Commits **address_parents;
}Commits;


/*
This is a branch it will be pointing to a specific commit.
Branch fill have it's own staging area of files. 
Files in the commit would be the actuall file and files that branch keeps would be the staged files. 
However under the condition we should keep track of all the file that has been added to the 
*/
typedef struct branch {
    char *branch_name;
    //This head will always point to the last element of the branch.
    Commits *head;
    size_t n_commit;

    File *files;
    size_t n_files;
}Branch;



/**
 This is the main helper struct where it keeps some essential information
 Helper will keep the root of the commit.
 Each Branch will be pointing to the commit.
*/
typedef struct helper {
    Branch *head;
    Branch *branch_list;
    size_t n_branch;
    Commits **lost_commits;
    int n_lost;
    int *n_commits_lost;
}Helper;

#endif