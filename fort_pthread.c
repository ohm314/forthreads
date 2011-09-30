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
#define FT_EINIT -1
#define FT_EINVALID -2

/**
 * A convenient array type
 **/


typedef struct array_tag {
    void **data;
    int size;
    int after;
    pthread_mutex_t mutex;
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
array_t *mutexes = NULL;

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

/**
 * Initializes a given array. The argument array must be either
 * already allocated or a NULL pointer.
 **/
void array_init(array_t **array,int size) {
    int i;

    if (*array == NULL)
        *array = (array_t*)malloc(sizeof(array_t));
    pthread_mutex_init(&((*array)->mutex),NULL);
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


void forthread_init(int *info) {
    int i = 0;
    pthread_t stid;
    
    *info = 0;
    
    if (is_initialized) {
        *info = FT_EINIT;
        return;
    }

    array_init(&threads,INIT_SIZE);
    array_init(&thread_attrs,INIT_SIZE);
    array_init(&once_ctrls,INIT_SIZE);
    array_init(&mutexes,INIT_SIZE);
    array_init(&mutex_attrs,INIT_SIZE);
    
    // allocate and store the thread master ID
    threads->data[0] = (pthread_t*) malloc(sizeof(pthread_t));
    stid = pthread_self();
    memcpy(threads->data[0],&stid,sizeof(pthread_t));
    threads->after++;
    
    is_initialized = 1;
}

void forthread_destroy(int* info) {
    // TODO: deallocate all, destroy all threads, mutexes
  *info = 0;
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


void forthread_create(int *thread_id, int *attr_id,
        void *(*start_routine)(void *), void *arg, int* info) {
    int i = 0;
    pthread_attr_t *attr;
    *info = 0;

    if (!is_initialized) {
        *info = FT_EINIT;
        return;
    }

    pthread_mutex_lock(&(threads->mutex));
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

    *info = pthread_create(threads->data[threads->after], attr, start_routine, arg);

    if (*attr_id == -1)
        free(attr);
    
    if (*info) {
        pthread_mutex_unlock(&(threads->mutex));
        return;
    }
    
    *thread_id = threads->after;
    threads->after++;


    pthread_mutex_unlock(&(threads->mutex));
    
}

void forthread_detach(int *thread_id, int *info) {

    if (!is_initialized) {
        *info = FT_EINIT;
        return;
    }

    if (!is_valid(threads,*thread_id)) {
        *info = FT_EINVALID;
        return;
    }

    *info = pthread_detach(*((pthread_t*)(threads->data[*thread_id])));
}

void forthread_equal(int *t1, int *t2, int *info) {
    *info = 0;

    if (!is_initialized)
        *info = FT_EINIT;
    else if (!is_valid(threads,*t1))
        *info = FT_EINVALID;
    else if (!is_valid(threads,*t2))
        *info = FT_EINVALID;
    else
        *info = pthread_equal(*((pthread_t*)(threads->data[*t1])),
                *((pthread_t*)(threads->data[*t2])));

}

void forthread_exit(void *value_ptr) {

    pthread_exit(value_ptr);

}

void forthread_join(int *thread_id, void **value_ptr, int *info) {
    *info = 0;

    if (!is_initialized) {
        *info = FT_EINIT;
        return;
    }

    pthread_mutex_lock(&(threads->mutex));
    if (!is_valid(threads,*thread_id)) {
        *info = FT_EINVALID;
        return;
    }

    *info = pthread_join(*((pthread_t*)(threads->data[*thread_id])),value_ptr);

    if (info) {
        pthread_mutex_unlock(&(threads->mutex));
        return;
    }
    
    free(threads->data[*thread_id]);
    threads->data[*thread_id] = NULL;
    
    pthread_mutex_unlock(&(threads->mutex));
}

void forthread_cancel(int *thread_id, int *info) {
    *info = 0;

    if (!is_initialized) {
        *info = FT_EINIT;
        return;
    }

    if (!is_valid(threads,*thread_id)) {
        *info = FT_EINVALID;
        return;
    }

    *info = pthread_cancel(*((pthread_t*)(threads->data[*thread_id])));

}

void forthread_once(int *once_ctrl_id, void (*routine)(void), int *info) {
    *info = 0;

    if (!is_initialized)
        *info = FT_EINIT;
    else if (!is_valid(once_ctrls,*once_ctrl_id))
        *info = FT_EINVALID;
    else
        *info = pthread_once(once_ctrls->data[*once_ctrl_id],routine);

}

void forthread_self(int *thread_id, int *info) {
    pthread_t tid;
    int i = 0;
    *info = 0;
    *thread_id = -1;

    if (!is_initialized) {
        *info = FT_EINIT;
        return;
    }

    tid = pthread_self();
    for (i = 0; i < threads->after; i++) {
        if (threads->data[i] == NULL)
            continue;
        if (pthread_equal(tid,*((pthread_t*)(threads->data[i])))) {
            *thread_id = i;
            return;
        }
    }
    *info = FT_EINVALID;
}
void forthread_atfork(void (*prepare)(void), 
        void (*parent)(void), void (*child)(void), int *info) {

    *info = pthread_atfork(prepare,parent,child);

}

/*****************************************/
/*      attribute object routines        */
/*****************************************/

void forthread_attr_destroy(int *attr, int *info) {
    *info = 0;

    if (!is_initialized) {
        *info = FT_EINIT;
        return;
    }

    pthread_mutex_lock(&(thread_attrs->mutex));
    
    if (!is_valid(thread_attrs,*attr)) {
        pthread_mutex_unlock(&(thread_attrs->mutex));
        *info = FT_EINVALID;
        return;
    }
    
    *info = pthread_attr_destroy(((pthread_attr_t*)(thread_attrs->data[*attr])));
    
    if (*info) {
        pthread_mutex_unlock(&(thread_attrs->mutex));
        return;
    }
    free(thread_attrs->data[*attr]);
    thread_attrs->data[*attr] = NULL;
    
    pthread_mutex_unlock(&(thread_attrs->mutex));
    
}


void forthread_attr_init(int *attr, int *info) {
    *info = 0;

    if (!is_initialized) {
        *info = FT_EINIT;
        return;
    }

    pthread_mutex_lock(&(thread_attrs->mutex));

    if (thread_attrs->after == thread_attrs->size) {
        // we exhausted the thread id and attribute arrays, double space
        array_resize(&thread_attrs,thread_attrs->size*2);
    }
    thread_attrs->data[thread_attrs->after] = 
        (pthread_attr_t*) malloc(sizeof(pthread_attr_t));

    *info = pthread_attr_init(thread_attrs->data[thread_attrs->after]);
    
    if (*info) {
        pthread_mutex_unlock(&(thread_attrs->mutex));
        return;
    }

    *attr = thread_attrs->after;
    thread_attrs->after++;

    pthread_mutex_unlock(&thread_attrs->mutex);
    
}


/*****************************************/
/*             mutex routines            */
/*****************************************/

void forthread_mutex_destroy(int *mutex_id, int *info) {
    
    *info = 0;

    if (!is_initialized) {
        *info = FT_EINIT;
        return;
    }
    
    pthread_mutex_lock(&(mutexes->mutex));
    
    if (!is_valid(mutexes,*mutex_id)) {
        pthread_mutex_unlock(&(mutexes->mutex));
        *info = FT_EINVALID;
        return;
    }

    *info = pthread_mutex_destroy(((pthread_mutex_t*)(mutexes->data[*mutex_id])));
    
    if (info) {
        pthread_mutex_unlock(&(mutexes->mutex));
        return;
    }

    free(mutexes->data[*mutex_id]);
    mutexes->data[*mutex_id] = NULL;
    
    pthread_mutex_unlock(&mutex_destroy_mutex);
    

}


int forthread_mutex_init(int *mutex_id, int *attr_id) {
    int i = 0;
    int info = 0;
    pthread_mutexattr_t *attr;
    
    if (!is_initialized)
        return FT_EINIT;

    pthread_mutex_lock(&mutex_init_mutex);
    if (mutexes->after == mutexes->size) {
        // we exhausted the mutex id array, double space
        array_resize(&mutexes,mutexes->size*2);
    }
    mutexes->data[mutexes->after] = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
    
    if (*attr_id == -1) {
        attr = NULL;
    } else {
        attr = thread_attrs->data[*attr_id];
    }

    info = pthread_mutex_init((pthread_mutex_t*)(mutexes->data[mutexes->after]), attr);

    if (info) {
        pthread_mutex_unlock(&mutex_init_mutex);
        return info;
    }
    
    *mutex_id = mutexes->after;
    mutexes->after++;

    pthread_mutex_unlock(&mutex_init_mutex);
    
    return 0;

}

int forthread_lock(int *mutex_id) {
    int info = 0;
    
    if (!is_initialized)
        return FT_EINIT;

    if (!is_valid(mutexes,*mutex_id)) {
        return FT_EINVALID;
    }

    return pthread_mutex_lock((pthread_mutex_t*)(mutexes->data[*mutex_id]));
    
}

int forthread_trylock(int *mutex_id) {
    int info = 0;

    if (!is_initialized)
        return FT_EINIT;

    if (!is_valid(mutexes,*mutex_id)) {
        return FT_EINVALID;
    }

    return pthread_mutex_trylock((pthread_mutex_t*)(mutexes->data[*mutex_id]));

}

int forthread_unlock(int *mutex_id) {
    int info = 0;

    if (!is_initialized)
        return FT_EINIT;

    if (!is_valid(mutexes,*mutex_id))
        return FT_EINVALID;

    return pthread_mutex_unlock((pthread_mutex_t*)(mutexes->data[*mutex_id]));

}


/*****************************************/
/*       mutex attribute routines        */
/*****************************************/

int forthread_mutexattr_destroy(int *attr) {
    int info = 0;

    if (!is_initialized)
        return FT_EINIT;

    pthread_mutex_lock(&mutex_attr_mutex);
    
    if (!is_valid(mutex_attrs,*attr)) {
        pthread_mutex_unlock(&mutex_attr_mutex);
        return FT_EINVALID;
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


int forthread_mutexattr_init(int *attr) {
    int info = 0;

    if (!is_initialized)
        return FT_EINIT;

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


/*****************************************/
/*    condition variable routines        */
/*****************************************/








