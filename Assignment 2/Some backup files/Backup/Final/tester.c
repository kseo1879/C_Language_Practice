#include <assert.h>
#include "helpers/svc.h"
#include "svc.c"
#include <stdio.h>

// clang tester.c -o tester -fsanitize=address -g



int main(int argc, char **argv) {

    // // char *name = "commits/";    
    // // FILE *fptr = fopen("test|data.txt", "wt");
    // // if(NULL == fptr) {
    // //     printf("Wrong\n");
    // //     return -1;
    // // } 
    // // fputc('A', fptr);
    // // fclose(fptr);

    Helper *helper = (Helper *) svc_init();
    //파일 3개 
    int a = 0;
    a = svc_add(helper, "COMP2017/svc.h");
    printf("Return value %d\n", a);
    printf("\n");
    a = svc_add(helper, "COMP2017/svc.c");
    printf("Return value %d\n", a);
    printf("\n");
    char *commit = NULL;
    commit = svc_commit(helper, "Initial commit");
    printf("Return value %s\n", commit);
    printf("\n");

    a = svc_branch(helper, "random_branch");
    printf("Return value %d\n", a);
    printf("\n");

    a = svc_checkout(helper, "random_branch"); 
    printf("Return value %d\n", a);
    printf("\n");

    copy_file_when_merge("a.txt", "COMP2017/svc.c");

    a = svc_rm(helper, "COMP2017/svc.h");
    printf("Return value %d\n", a);
    printf("\n");

    commit = svc_commit(helper, "Implemented svc_init");
    printf("Return value %s\n", commit);
    printf("\n");


    a = svc_reset(helper, "7b3e30");
    printf("Return value %d\n", a);
    printf("\n");

    copy_file_when_merge("a.txt", "COMP2017/svc.c");

    commit = svc_commit(helper, "Implemented svc_init");
    printf("Return value %s\n", commit);
    printf("\n");

    void *commits = get_commit(helper, "24829b");

    int n_prev;
    char **prev_commits = get_prev_commits(helper, commits, &n_prev);
    printf("%s, %d\n", prev_commits[0], n_prev);
    printf("\n");

    a = svc_checkout(helper, "master");
    printf("Return value %d\n", a);
    printf("\n");

    resolution *resolutions = malloc(sizeof(resolution)); 
    resolutions[0].file_name = "COMP2017/svc.c"; 
    resolutions[0].resolved_file = "a.txt";

    commit = svc_merge(helper, "random_branch", resolutions, 1);
    free(resolutions);
    printf("Return value %s\n", commit);
    printf("\n");

    print_commit(helper, "48eac3");

    commits = get_commit(helper, "48eac3");
    prev_commits = get_prev_commits(helper, commits, &n_prev);
    printf("%s, %s, %d\n", prev_commits[0], prev_commits[1], n_prev);
    printf("\n");


    // svc_branch(helper, "Calvin");
    // svc_checkout(helper, "Calvin");

    // svc_add(helper, "aA.py");
    // a = svc_commit(helper, "Added aA.py");
    // svc_checkout(helper, "Calvin");

    // svc_add(helper, "apple.py");
    // resolution *res = NULL;
    // svc_checkout(helper, "master");
    // svc_merge(helper, "Calvin", res, 1);

    // // a = svc_commit(helper, "Initial Commit");
    // // printf("Name of the commit is %s\n", a);

    // //print_commit(helper, a);
    // //svc_rm(helper, "Banana.py");
    // //a = svc_commit(helper, "Initial Commit");
    // //print_commit(helper, a);
    cleanup(helper);
}

