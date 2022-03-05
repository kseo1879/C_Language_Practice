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

    svc_add(helper, "Testing/TestmyAss.py");
    svc_add(helper, "Banana.py");

    char *a = NULL;
    a = svc_commit(helper, "Initial Commit");
    printf("Name of the commit is %s\n", a);
    svc_reset(helper, "15ecf5");

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

