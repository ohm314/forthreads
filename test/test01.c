#include "../src/forthread.h"
#include <stdio.h>
#include <stdlib.h>

void *run(void *arg) {
    printf("Hello World! %d\n",*(int*)arg);
    return NULL;
}

int main(int argc, char** argv) {
    int tid[1000];
    int arg[1000];
    int aid = -1;
    int info = 0;
    int i = 0;
    void *(*routine)(void*) = &run;

    printf("====================== test01 ======================\n");
    printf("Initializing fort_threads\n");
    
    thread_init(&info);

    printf("creating a thread\n");
    for (i = 0; i < 10;i++) {
        arg[i] = i;
        thread_create(&tid[i],&aid,&routine,&arg[i],&info);
        if (info) {
            printf("error %d\n",info);
        }
        printf("thread %d created\n",tid[i]);
    }

    for (i = 0; i < 10; i++) {
        printf("join thread %d\n",tid[i]);
        thread_join(&tid[i],NULL,&info);
        if (info) {
            printf("error %d\n",info);
        }
    }
    printf("==================== end test01 ====================\n");
    return 0;
}


