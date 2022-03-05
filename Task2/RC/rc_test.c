#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rc.h"

typedef struct command command_t;

struct command {
	char* str;
	int (*exe)();
};


int rc_example_1() {
    int pass = 0;
    int expected = 2;

    struct strong_ref* m = rc_alloc(NULL, sizeof(int), NULL);

    *((int*)m->ptr) = 100;

    int v = *((int*)m->ptr);

    if(v == 100) {
      pass++;
    }

    struct weak_ref w = rc_downgrade(m);
    struct strong_ref* p = rc_upgrade(w);
    if(p == NULL) {
        pass++;
    }

    return pass == expected;
}

int rc_example_2() {
    int pass = 0;
    int expected = 3;

    struct strong_ref* m = rc_alloc(NULL, sizeof(int), NULL);
    struct strong_ref* a = rc_alloc(m->ptr, 0, NULL);

    *((int*)m->ptr) = 100;

    struct weak_ref w = rc_downgrade(m);
    struct strong_ref* p = rc_upgrade(w);


    if(p != NULL) {
        pass++;
    }

    int v = *((int*)p->ptr);
    if(v == 100) {
      pass++;
    }
    rc_downgrade(p);
    w = rc_downgrade(a);

    p = rc_upgrade(w);
    if(p == NULL) {
        pass++;
    }

    return pass == expected;
}

struct nothing {
    int *apple;
    int banana;
};

int rc_example_3() {
    int pass = 0;
    int expected = 3;

    struct strong_ref* m = rc_alloc(NULL, sizeof(struct nothing), NULL);
    struct strong_ref* b = rc_alloc(m->ptr, 0, NULL);
    struct strong_ref* k = rc_alloc(NULL, sizeof(int), m);

    (((struct nothing *)m->ptr)->apple) = k->ptr;
    *(((struct nothing *)m->ptr)->apple) = 100;

    struct weak_ref w = rc_downgrade(m);
    //struct weak_ref p = rc_downgrade(b);

    rc_cleanup();
    
    struct strong_ref *g = rc_upgrade(w);



    // if(p != NULL) {
    //     pass++;
    // }

    // int v = *((int*)p->ptr);
    // if(v == 100) {
    //   pass++;
    // }
    // rc_downgrade(p);
    // w = rc_downgrade(a);

    // p = rc_upgrade(w);
    // if(p == NULL) {
    //     pass++;
    // }

    return pass == expected;
}

// command_t tests[] = {
//   { "rc_example_1", rc_example_1 },
//   { "rc_example_2", rc_example_2 },
// };


// int main(int argc, char** argv) {
//   int test_n = sizeof(tests) / sizeof(command_t);
//   if(argc >= 2) {
// 		for(int i = 0; i < test_n; i++) {
// 			if(strcmp(argv[1], tests[i].str) == 0) {
// 				if(tests[i].exe()) {
// 				  fprintf(stdout, "%s Passed\n", tests[i].str);
// 				} else {
// 				  fprintf(stdout, "%s Failed\n", tests[i].str);
// 				}
// 			}
// 		}
// 	}
// }