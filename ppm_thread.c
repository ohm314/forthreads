#include <pthread.h>
#include <stdlib.h>
#include <string.h>
/*
 * C wrappers to pthreads to make wrapping with Fortran simpler
 */


/**
 * global parameters
 **/

#define INIT_SIZE 4

/**
 * error codes
 **/
#define PPM_EINIT -1
#define PPM_EINVALID -2

/**
 * A convenient array type
 **/


typedef struct {
    void **data;
    int size;
    int after;
} array_t;

/**
 * global data structures
 **/


int is_initialized = 0;


pthread_mutex_t thread_create_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t thread_join_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t thread_attr_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_attr_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_init_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_destroy_mutex = PTHREAD_MUTEX_INITIALIZER;


/**
 * holds the thread IDs
 **/
array_t *threads = NULL;

/** 
 * holds thread attributes, the index does not
 * necesseraly conincide with the one of threads.
 *
 * This array is just to allow different threads spawn new
 * threads at the same time.
 **/
array_t *thread_attrs = NULL;

/*
 * holds the mutex IDs
 **/
array_t *mutex = NULL;

/**
 * holds thread mutex attributes, the index does not
 * necesseraly concide with the one of mutexes.
 *
 * This array is just to allow different threads handle
 * their mutexes
 **/
array_t *mutex_attrs = NULL;

/**
 * an array to hold pthread_once_t structures
 **/
array_t *once_ctrls = NULL;

void array_init(array_t **array,int size) {
    int i;

    if (*array == NULL)
        *array = (array_t*)malloc(sizeof(array_t));
    (*array)->data = (void**)malloc(sizeof(void*)*size);
    for(i = 0; i < size; i++)
        (*array)->data[i] = NULL;
    (*array)->size = size;
    (*array)->after = 0;
}

void array_resize(array_t **array,int size) {
    int i;

    (*array)->data = (void**)realloc((*array)->data,sizeof(void*)*size);
    (*array)->size = size;
    
    for(i = (*array)->after; i < size; i++)
        (*array)->data[i] = NULL;
}

void array_delete(array_t *array) {
    free(array->data);
    free(array);
}


int ppm_cthread_init() {
    int i = 0;
    int info = 0;
    pthread_t stid;
    if (is_initialized)
        return 0;

    array_init(&threads,INIT_SIZE);
    array_init(&thread_attrs,INIT_SIZE);
    array_init(&once_ctrls,INIT_SIZE);
    array_init(&mutex,INIT_SIZE);
    array_init(&mutex_attrs,INIT_SIZE);
    
    // allocate and store the thread master ID
    threads->data[0] = (pthread_t*) malloc(sizeof(pthread_t));
    stid = pthread_self();
    memcpy(threads->data[0],&stid,sizeof(pthread_t));
    threads->after++;
    
    is_initialized = 1;
    return 0;
}

int ppm_cthread_destroy() {
    // TODO: deallocate all, destroy all threads, mutexes
}

// This only works for pointer arrays!!
int is_valid(array_t *arr, int id) {
    if ((id >= 0) && (id < arr->after) && 
        (arr->data[id] != NULL))
        return 1;
    else
        return 0;
}

/*****************************************/
/*         Thread routines               */
/*****************************************/


int ppm_cthread_create(int *thread_id, int *attr_id, void *(*start_routine)(void *),void *arg) {
    int i = 0;
    int info = 0;
    pthread_attr_t *attr;
    
    if (!is_initialized)
        return PPM_EINIT;

    pthread_mutex_lock(&thread_create_mutex);
    if (threads->after == threads->size) {
        // we exhausted the thread id and attribute arrays, double space
        array_resize(&threads,threads->size*2);
    }
    threads->data[threads->after] = (pthread_t*) malloc(sizeof(pthread_t));
    
    if (*attr_id == -1) {
        attr = (pthread_attr_t*) malloc(sizeof(pthread_attr_t));
        pthread_attr_init(attr);
        pthread_attr_setdetachstate(attr, PTHREAD_CREATE_JOINABLE);
    } else {
        attr = thread_attrs->data[*attr_id];
    }

    info = pthread_create(threads->data[threads->after], attr, start_routine, arg);

    if (info) {
        pthread_mutex_unlock(&thread_create_mutex);
        return info;
    }
    
    *thread_id = threads->after;
    threads->after++;

    if (*attr_id == -1)
        free(attr);

    pthread_mutex_unlock(&thread_create_mutex);
    
    return 0;
}

int ppm_cthread_detach(int *thread_id) {

    if (!is_initialized)
        return PPM_EINIT;

    if (!is_valid(threads,*thread_id)) {
        return PPM_EINVALID;
    }

    return pthread_detach(*((pthread_t*)(threads->data[*thread_id])));
}

int ppm_cthread_equal(int *t1, int *t2) {

    if (!is_initialized)
        return PPM_EINIT;

    if (!is_valid(threads,*t1)) {
        return PPM_EINVALID;
    }
    
    if (!is_valid(threads,*t2)) {
        return PPM_EINVALID;
    }
    
    return pthread_equal(*((pthread_t*)(threads->data[*t1])),
                *((pthread_t*)(threads->data[*t2])));
}

int ppm_cthread_exit(void *value_ptr) {

    pthread_exit(value_ptr);

}

int ppm_cthread_join(int *thread_id, void **value_ptr) {
    int info = 0;

    if (!is_initialized)
        return PPM_EINIT;

    pthread_mutex_lock(&thread_join_mutex);
    if (!is_valid(threads,*thread_id)) {
        return PPM_EINVALID;
    }

    info = pthread_join(*((pthread_t*)(threads->data[*thread_id])),value_ptr);

    if (info) {
        pthread_mutex_unlock(&thread_join_mutex);
        return info;
    }
    
    free(threads->data[*thread_id]);
    threads->data[*thread_id] = NULL;
    
    pthread_mutex_unlock(&thread_join_mutex);
    return info;
}

int ppm_cthread_cancel(int *thread_id) {
    int info = 0;

    if (!is_initialized)
        return PPM_EINIT;

    if (!is_valid(threads,*thread_id)) {
        return PPM_EINVALID;
    }

    return pthread_cancel(*((pthread_t*)(threads->data[*thread_id])));

}

int ppm_cthread_once(int *once_ctrl_id, void (*routine)(void)) {
    int info = 0;

    if (!is_initialized)
        return PPM_EINIT;

    if (!is_valid(once_ctrls,*once_ctrl_id)) {
        return PPM_EINVALID;
    }

    return pthread_once(once_ctrls->data[*once_ctrl_id],routine);

}

int ppm_cthread_self() {
    pthread_t tid;
    int i = 0;

    tid = pthread_self();
    for (i = 0; i < threads->after; i++) {
        if (threads->data[i] == NULL)
            continue;
        if (pthread_equal(tid,*((pthread_t*)(threads->data[i]))))
            return i;
    }
    return -1;
}

int ppm_cthread_atfork(void (*prepare)(void), void (*parent)(void), void (*child)(void)) {

    return pthread_atfork(prepare,parent,child);
}

/*****************************************/
/*      attribute object routines        */
/*****************************************/

int ppm_cthread_attr_destroy(int *attr) {
    int info = 0;

    if (!is_initialized)
        return PPM_EINIT;

    pthread_mutex_lock(&thread_attr_mutex);
    
    if (!is_valid(thread_attrs,*attr)) {
        pthread_mutex_unlock(&thread_attr_mutex);
        return PPM_EINVALID;
    }
    
    info = pthread_attr_destroy(((pthread_attr_t*)(thread_attrs->data[*attr])));
    
    if (info) {
        pthread_mutex_unlock(&thread_attr_mutex);
        return info;
    }
    free(thread_attrs->data[*attr]);
    thread_attrs->data[*attr] = NULL;
    
    pthread_mutex_unlock(&thread_attr_mutex);
    
    return 0;
}


int ppm_cthread_attr_init(int *attr) {
    int info = 0;

    if (!is_initialized)
        return PPM_EINIT;

    pthread_mutex_lock(&thread_attr_mutex);

    if (thread_attrs->after == thread_attrs->size) {
        // we exhausted the thread id and attribute arrays, double space
        array_resize(&thread_attrs,thread_attrs->size*2);
    }
    thread_attrs->data[thread_attrs->after] = 
        (pthread_attr_t*) malloc(sizeof(pthread_attr_t));

    info = pthread_attr_init(thread_attrs->data[thread_attrs->after]);
    
    if (info) {
        pthread_mutex_unlock(&thread_attr_mutex);
        return info;
    }

    *attr = thread_attrs->after;
    thread_attrs->after++;

    pthread_mutex_unlock(&thread_attr_mutex);
    
    return info;
}


/*****************************************/
/*             mutex routines            */
/*****************************************/

int ppm_cthread_mutex_destroy(int *mutex_id) {
    
    int info = 0;

    if (!is_initialized)
        return PPM_EINIT;
    
    pthread_mutex_lock(&mutex_destroy_mutex);
    
    if (!is_valid(mutex,*mutex_id)) {
        pthread_mutex_unlock(&mutex_destroy_mutex);
        return PPM_EINVALID;
    }

    info = pthread_mutex_destroy(((pthread_mutex_t*)(mutex->data[*mutex_id])));
    
    if (info) {
        pthread_mutex_unlock(&mutex_destroy_mutex);
        return info;
    }

    free(mutex->data[*mutex_id]);
    mutex->data[*mutex_id] = NULL;
    
    pthread_mutex_unlock(&mutex_destroy_mutex);
    
    return 0;

}


ppm_cthread_mutex_init(int *mutex_id, int *attr_id) {
    int i = 0;
    int info = 0;
    pthread_mutexattr_t *attr;
    
    if (!is_initialized)
        return PPM_EINIT;

    pthread_mutex_lock(&mutex_init_mutex);
    if (mutex->after == mutex->size) {
        // we exhausted the mutex id array, double space
        array_resize(&mutex,mutex->size*2);
    }
    mutex->data[mutex->after] = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
    
    if (*attr_id == -1) {
        attr = NULL;
    } else {
        attr = thread_attrs->data[*attr_id];
    }

    info = pthread_mutex_init((pthread_mutex_t*)(mutex->data[mutex->after]), attr);

    if (info) {
        pthread_mutex_unlock(&mutex_init_mutex);
        return info;
    }
    
    *mutex_id = mutex->after;
    mutex->after++;

    pthread_mutex_unlock(&mutex_init_mutex);
    
    return 0;

}

int ppm_cthread_lock(int *mutex_id) {
    int info = 0;
    
    if (!is_initialized)
        return PPM_EINIT;

    if (!is_valid(mutex,*mutex_id)) {
        return PPM_EINVALID;
    }

    return pthread_mutex_lock((pthread_mutex_t*)(mutex->data[*mutex_id]));
    
}

int ppm_cthread_trylock(int *mutex_id) {
    int info = 0;

    if (!is_initialized)
        return PPM_EINIT;

    if (!is_valid(mutex,*mutex_id)) {
        return PPM_EINVALID;
    }

    return pthread_mutex_trylock((pthread_mutex_t*)(mutex->data[*mutex_id]));

}

int ppm_cthread_unlock(int *mutex_id) {
    int info = 0;

    if (!is_initialized)
        return PPM_EINIT;

    if (!is_valid(mutex,*mutex_id))
        return PPM_EINVALID;

    return pthread_mutex_unlock((pthread_mutex_t*)(mutex->data[*mutex_id]));

}



/*****************************************/
/*       mutex attribute routines        */
/*****************************************/

int ppm_cthread_mutexattr_destroy(int *attr) {
    int info = 0;

    if (!is_initialized)
        return PPM_EINIT;

    pthread_mutex_lock(&mutex_attr_mutex);
    
    if (!is_valid(mutex_attrs,*attr)) {
        pthread_mutex_unlock(&mutex_attr_mutex);
        return PPM_EINVALID;
    }
    
    info = pthread_mutexattr_destroy(((pthread_mutexattr_t*)(mutex_attrs->data[*attr])));
    
    if (info) {
        pthread_mutex_unlock(&mutex_attr_mutex);
        return info;
    }

    free(mutex_attrs->data[*attr]);
    mutex_attrs->data[*attr] = NULL;
    
    pthread_mutex_unlock(&mutex_attr_mutex);
    
    return 0;


}


int ppm_cthread_mutexattr_init(int *attr) {
    int info = 0;

    if (!is_initialized)
        return PPM_EINIT;

    pthread_mutex_lock(&mutex_attr_mutex);

    if (mutex_attrs->after == mutex_attrs->size) {
        // we exhausted the mutex attribute array, double space
        array_resize(&mutex_attrs,mutex_attrs->size*2);
    }
    mutex_attrs->data[mutex_attrs->after] = 
        (pthread_mutexattr_t*) malloc(sizeof(pthread_mutexattr_t));

    info = pthread_mutexattr_init(mutex_attrs->data[mutex_attrs->after]);
    
    if (info) {
        pthread_mutex_unlock(&mutex_attr_mutex);
        return info;
    }

    *attr = mutex_attrs->after;
    mutex_attrs->after++;

    pthread_mutex_unlock(&mutex_attr_mutex);
    
    return info;

}






