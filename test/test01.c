#include "../ppm_thread.c"
#include <stdio.h>
#include <stdlib.h>

void *run(void *arg) {
    printf("Hello World! %d\n",*(int*)arg);
}

int main(int argc, char** argv) {
    int tid[1000];
    int arg[1000];
    int aid = -1;
    int info = 0;
    int i = 0;

    printf("Initializing ppm_thread\n");
    
    ppm_cthread_init();

    printf("creating a thread\n");
    for (i = 0; i < 1000;i++) {
        arg[i] = i;
        info = ppm_cthread_create(&tid[i],&aid,&run,&arg[i]);
        if (info) {
            printf("error %d\n",info);
        }
        printf("thread %d created\n",tid[i]);
    }

    for (i = 0; i < 1000; i++) {
        printf("join thread %d\n",tid[i]);
        info = ppm_cthread_join(&tid[i],NULL);
        if (info) {
            printf("error %d\n",info);
        }
    }
    return 0;
}


