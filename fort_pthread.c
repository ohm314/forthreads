#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
#define FT_OK 0
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
 * A convenient volatile array type
 **/
typedef struct varray_tag {
  volatile void **data;
  int size;
  int after;
  pthread_mutex_t mutex;
} varray_t;



/**
 * global data structures
 **/

int is_initialized = 0;


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
 * an array to hold thread condition variable (pthread_cond_t) 
 * structures
 **/
array_t *conds = NULL;

/**
 * holds thread condition variable attributes, the index does not
 * necesseraly concide with the one of conds.
 *
 * This array is just to allow different threads handle
 * their condition variables
 **/
array_t *cond_attrs = NULL;

/**
 * an array to hold thread barrier variable (pthread_barrier_t) 
 * structures
 **/
array_t *barriers = NULL;

/**
 * holds thread barrier variable attributes, the index does not
 * necesseraly concide with the one of conds.
 *
 * This array is just to allow different threads handle
 * their condition variables
 **/
array_t *barrier_attrs = NULL;

/**
 * an array to hold spinlock variable (pthread_spinlock_t) 
 * structures
 **/
varray_t *spinlocks = NULL;

/**
 * an array to hold thread read-write lock variable (pthread_rwlock_t) 
 * structures
 **/
array_t *rwlocks = NULL;

/**
 * holds thread rwlock variable attributes, the index does not
 * necesseraly concide with the one of conds.
 *
 * This array is just to allow different threads handle
 * their condition variables
 **/
array_t *rwlock_attrs = NULL;

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

/**
 * Initializes a given varray. The argument array must be either
 * already allocated or a NULL pointer.
 **/
void varray_init(varray_t **array,int size) {
  int i;

  if (*array == NULL)
    *array = (varray_t*)malloc(sizeof(varray_t));
  pthread_mutex_init(&((*array)->mutex),NULL);
  (*array)->data = (volatile void**)malloc(sizeof(void*)*size);
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

void varray_resize(varray_t **array,int size) {
  int i;

  (*array)->data = (volatile void**)realloc((*array)->data,sizeof(volatile void*)*size);
  (*array)->size = size;

  for(i = (*array)->after; i < size; i++)
    (*array)->data[i] = NULL;

}

void array_delete(array_t *array) {
  free(array->data);
  free(array);
}

void varray_delete(varray_t *array) {
  free(array->data);
  free(array);
}


void forthread_init(int *info) {
  int i = 0;
  pthread_t stid;

  *info = FT_OK;

  if (is_initialized) {
    *info = FT_EINIT;
    return;
  }

  array_init(&threads,INIT_SIZE);
  array_init(&thread_attrs,INIT_SIZE);
  array_init(&once_ctrls,INIT_SIZE);
  array_init(&mutexes,INIT_SIZE);
  array_init(&mutex_attrs,INIT_SIZE);
  array_init(&conds,INIT_SIZE);
  array_init(&cond_attrs,INIT_SIZE);
  array_init(&barriers,INIT_SIZE);
  array_init(&barrier_attrs,INIT_SIZE);
  varray_init(&spinlocks,INIT_SIZE);

  // allocate and store the thread master ID
  threads->data[0] = (pthread_t*) malloc(sizeof(pthread_t));
  stid = pthread_self();
  memcpy(threads->data[0],&stid,sizeof(pthread_t));
  threads->after++;

  is_initialized = 1;
}

void forthread_destroy(int* info) {
  // TODO: deallocate all, destroy all threads, mutexes
  *info = FT_OK;
}

// This only works for pointer arrays!!
int is_valid(array_t *arr, int id) {
  if ((id >= 0) && (id < arr->after) && 
      (arr->data[id] != NULL))
    return 1;
  else
    return 0;
}

// varray version
int vis_valid(varray_t *arr, int id) {
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
  *info = FT_OK;

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

  *info = FT_OK;

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
  *info = FT_OK;

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
  *info = FT_OK;

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

  if (*info) {
    pthread_mutex_unlock(&(threads->mutex));
    return;
  }

  free(threads->data[*thread_id]);
  threads->data[*thread_id] = NULL;

  pthread_mutex_unlock(&(threads->mutex));
}

void forthread_cancel(int *thread_id, int *info) {
  *info = FT_OK;

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
  *info = FT_OK;

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
  *info = FT_OK;
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
  *info = FT_OK;

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
  *info = FT_OK;

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

  *info = FT_OK;

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

  if (*info) {
    pthread_mutex_unlock(&(mutexes->mutex));
    return;
  }

  free(mutexes->data[*mutex_id]);
  mutexes->data[*mutex_id] = NULL;


  pthread_mutex_unlock(&(mutexes->mutex));

}


void forthread_mutex_init(int *mutex_id, int *attr_id, int *info) {
  int i = 0;
  *info = FT_OK;

  pthread_mutexattr_t *attr;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  pthread_mutex_lock(&(mutexes->mutex));
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

  *info = pthread_mutex_init((pthread_mutex_t*)(
        mutexes->data[mutexes->after]), attr);

  if (*info) {
    pthread_mutex_unlock(&(mutexes->mutex));
    return;
  }

  *mutex_id = mutexes->after;
  mutexes->after++;

  pthread_mutex_unlock(&(mutexes->mutex));


}

void forthread_lock(int *mutex_id, int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  if (!is_valid(mutexes,*mutex_id)) {
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_mutex_lock((pthread_mutex_t*)(mutexes->data[*mutex_id]));

}

void forthread_trylock(int *mutex_id, int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  if (!is_valid(mutexes,*mutex_id)) {
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_mutex_trylock((pthread_mutex_t*)(mutexes->data[*mutex_id]));


}

void forthread_mutex_unlock(int *mutex_id, int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  if (!is_valid(mutexes,*mutex_id)) {
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_mutex_unlock((pthread_mutex_t*)(mutexes->data[*mutex_id]));

}


/*****************************************/
/*       mutex attribute routines        */
/*****************************************/

void forthread_mutexattr_destroy(int *attr,int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  pthread_mutex_lock(&(mutex_attrs->mutex));

  if (!is_valid(mutex_attrs,*attr)) {
    pthread_mutex_unlock(&(mutex_attrs->mutex));
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_mutexattr_destroy(((pthread_mutexattr_t*)(
          mutex_attrs->data[*attr])));

  if (*info) {
    pthread_mutex_unlock(&(mutex_attrs->mutex));
    return;
  }

  free(mutex_attrs->data[*attr]);
  mutex_attrs->data[*attr] = NULL;

  pthread_mutex_unlock(&(mutex_attrs->mutex));



}


void forthread_mutexattr_init(int *attr,int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  pthread_mutex_lock(&(mutex_attrs->mutex));

  if (mutex_attrs->after == mutex_attrs->size) {
    // we exhausted the mutex attribute array, double space
    array_resize(&mutex_attrs,mutex_attrs->size*2);
  }
  mutex_attrs->data[mutex_attrs->after] = 
    (pthread_mutexattr_t*) malloc(sizeof(pthread_mutexattr_t));

  *info = pthread_mutexattr_init(mutex_attrs->data[mutex_attrs->after]);

  if (*info) {
    pthread_mutex_unlock(&(mutex_attrs->mutex));
    return;
  }

  *attr = mutex_attrs->after;
  mutex_attrs->after++;

  pthread_mutex_unlock(&(mutex_attrs->mutex));

}


/*****************************************/
/*    condition variable routines        */
/*****************************************/


void forthread_cond_destroy(int *cond_id, int *info) {

  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  pthread_mutex_lock(&(conds->mutex));

  if (!is_valid(conds,*cond_id)) {
    pthread_mutex_unlock(&(conds->mutex));
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_cond_destroy(((pthread_cond_t*)(conds->data[*cond_id])));

  if (*info) {
    pthread_mutex_unlock(&(conds->mutex));
    return;
  }

  free(conds->data[*cond_id]);
  conds->data[*cond_id] = NULL;
  
  pthread_mutex_unlock(&(conds->mutex));

}


void forthread_cond_init(int *cond_id, int *attr_id, int *info) {
  int i = 0;
  *info = FT_OK;
  pthread_condattr_t *attr;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  pthread_mutex_lock(&(conds->mutex));
  if (conds->after == conds->size) {
    // we exhausted the mutex id array, double space
    array_resize(&conds,conds->size*2);
  }
  conds->data[conds->after] = (pthread_cond_t*) malloc(sizeof(pthread_cond_t));

  if (*attr_id == -1) {
    attr = NULL;
  } else {
    attr = cond_attrs->data[*attr_id];
  }

  *info = pthread_cond_init((pthread_cond_t*)(conds->data[conds->after]), attr);

  if (*info) {
    pthread_mutex_unlock(&(conds->mutex));
    return;
  }

  *cond_id = conds->after;
  conds->after++;
  
  pthread_mutex_unlock(&(conds->mutex));

}

void forthread_cond_timedwait(int *cond_id, int *mutex_id, long *ns, int *info) {
  struct timespec t;
  *info = FT_OK;

  // TODO: check if ns > 10^6 is allowed
  t.tv_sec = 0;
  t.tv_nsec = *ns;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }
  
  if ((!is_valid(mutexes,*mutex_id)) || (!is_valid(conds,*cond_id))) {
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_cond_timedwait((pthread_cond_t*)(conds->data[*cond_id]),
                                 (pthread_mutex_t*)(mutexes->data[*mutex_id]),
                                 &t);

}


void forthread_cond_wait(int *cond_id, int *mutex_id, int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }
  
  if ((!is_valid(mutexes,*mutex_id)) || (!is_valid(conds,*cond_id))) {
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_cond_wait((pthread_cond_t*)(conds->data[*cond_id]),
                            (pthread_mutex_t*)(mutexes->data[*mutex_id]));

}


void forthread_cond_broadcast(int *cond_id, int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }
  
  if (!is_valid(conds,*cond_id)) {
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_cond_broadcast((pthread_cond_t*)(conds->data[*cond_id]));

}


void forthread_cond_signal(int *cond_id, int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }
  
  if (!is_valid(conds,*cond_id)) {
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_cond_signal((pthread_cond_t*)(conds->data[*cond_id]));

}

/*****************************************************/
/*    condition attriubute variable routines         */
/*****************************************************/


void forthread_condattr_destroy(int *attr,int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  pthread_mutex_lock(&(cond_attrs->mutex));

  if (!is_valid(cond_attrs,*attr)) {
    pthread_mutex_unlock(&(cond_attrs->mutex));
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_condattr_destroy(((pthread_condattr_t*)(
          cond_attrs->data[*attr])));

  if (*info) {
    pthread_mutex_unlock(&(cond_attrs->mutex));
    return;
  }

  free(cond_attrs->data[*attr]);
  cond_attrs->data[*attr] = NULL;

  pthread_mutex_unlock(&(cond_attrs->mutex));

}


void forthread_condattr_init(int *attr,int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  pthread_mutex_lock(&(cond_attrs->mutex));

  if (cond_attrs->after == cond_attrs->size) {
    // we exhausted the mutex attribute array, double space
    array_resize(&cond_attrs,cond_attrs->size*2);
  }
  cond_attrs->data[cond_attrs->after] = 
    (pthread_condattr_t*) malloc(sizeof(pthread_condattr_t));

  *info = pthread_condattr_init(cond_attrs->data[cond_attrs->after]);

  if (*info) {
    pthread_mutex_unlock(&(cond_attrs->mutex));
    return;
  }

  *attr = cond_attrs->after;
  cond_attrs->after++;

  pthread_mutex_unlock(&(cond_attrs->mutex));

}


/****************************************/
/*    barrier variable routines         */
/****************************************/

void forthread_barrier_destroy(int *barrier_id, int *info) {

  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  pthread_mutex_lock(&(barriers->mutex));

  if (!is_valid(barriers,*barrier_id)) {
    pthread_mutex_unlock(&(barriers->mutex));
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_barrier_destroy(((pthread_barrier_t*)(barriers->data[*barrier_id])));

  if (*info) {
    pthread_mutex_unlock(&(barriers->mutex));
    return;
  }

  free(barriers->data[*barrier_id]);
  barriers->data[*barrier_id] = NULL;
  
  pthread_mutex_unlock(&(barriers->mutex));

}


void forthread_barrier_init(int *barrier_id, int *attr_id, int *count, int *info) {
  int i = 0;
  *info = FT_OK;
  pthread_barrierattr_t *attr;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  pthread_mutex_lock(&(barriers->mutex));
  if (barriers->after == barriers->size) {
    // we exhausted the mutex id array, double space
    array_resize(&barriers,barriers->size*2);
  }
  barriers->data[barriers->after] = (pthread_barrier_t*) malloc(sizeof(pthread_barrier_t));

  if (*attr_id == -1) {
    attr = NULL;
  } else {
    attr = barrier_attrs->data[*attr_id];
  }

  *info = pthread_barrier_init((pthread_barrier_t*)(barriers->data[barriers->after])
                               ,attr, *count);

  if (*info) {
    pthread_mutex_unlock(&(barriers->mutex));
    return;
  }

  *barrier_id = barriers->after;
  barriers->after++;
  
  pthread_mutex_unlock(&(barriers->mutex));

}

void forthread_barrier_wait(int *barrier_id, int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }
  
  if (!is_valid(barriers,*barrier_id)) {
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_barrier_wait((pthread_barrier_t*)(barriers->data[*barrier_id]));

}


/**************************************************/
/*    barrier attribute variable routines         */
/**************************************************/


void forthread_barrierattr_destroy(int *attr,int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  pthread_mutex_lock(&(barrier_attrs->mutex));

  if (!is_valid(barrier_attrs,*attr)) {
    pthread_mutex_unlock(&(barrier_attrs->mutex));
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_barrierattr_destroy(((pthread_barrierattr_t*)(
          barrier_attrs->data[*attr])));

  if (*info) {
    pthread_mutex_unlock(&(barrier_attrs->mutex));
    return;
  }

  free(barrier_attrs->data[*attr]);
  barrier_attrs->data[*attr] = NULL;

  pthread_mutex_unlock(&(barrier_attrs->mutex));

}


void forthread_barrierattr_init(int *attr,int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  pthread_mutex_lock(&(barrier_attrs->mutex));

  if (barrier_attrs->after == barrier_attrs->size) {
    // we exhausted the mutex attribute array, double space
    array_resize(&barrier_attrs,barrier_attrs->size*2);
  }
  barrier_attrs->data[barrier_attrs->after] = 
    (pthread_barrierattr_t*) malloc(sizeof(pthread_barrierattr_t));

  *info = pthread_barrierattr_init(barrier_attrs->data[barrier_attrs->after]);

  if (*info) {
    pthread_mutex_unlock(&(barrier_attrs->mutex));
    return;
  }

  *attr = barrier_attrs->after;
  barrier_attrs->after++;

  pthread_mutex_unlock(&(barrier_attrs->mutex));

}

/*************************************/
/*    spin variable routines         */
/*************************************/

void forthread_spin_destroy(int *spinlock_id, int *info) {

  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  pthread_mutex_lock(&(spinlocks->mutex));

  if (!vis_valid(spinlocks,*spinlock_id)) {
    pthread_mutex_unlock(&(spinlocks->mutex));
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_spin_destroy(((pthread_spinlock_t*)(spinlocks->data[*spinlock_id])));

  if (*info) {
    pthread_mutex_unlock(&(spinlocks->mutex));
    return;
  }

  free((void *)spinlocks->data[*spinlock_id]);
  spinlocks->data[*spinlock_id] = NULL;
  
  pthread_mutex_unlock(&(spinlocks->mutex));

}


void forthread_spin_init(int *spinlock_id, int *pshared, int *info) {
  int i = 0;
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  pthread_mutex_lock(&(spinlocks->mutex));
  if (spinlocks->after == spinlocks->size) {
    // we exhausted the mutex id array, double space
    varray_resize(&spinlocks,spinlocks->size*2);
  }
  spinlocks->data[spinlocks->after] = (pthread_spinlock_t*) malloc(sizeof(pthread_spinlock_t));

  *info = pthread_spin_init((pthread_spinlock_t*)(spinlocks->data[spinlocks->after])
                               , *pshared);

  if (*info) {
    pthread_mutex_unlock(&(spinlocks->mutex));
    return;
  }

  *spinlock_id = spinlocks->after;
  spinlocks->after++;
  
  pthread_mutex_unlock(&(spinlocks->mutex));

}

// TODO: this might need a lock
void forthread_spin_lock(int *lock_id, int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  
  if (!vis_valid(spinlocks,*lock_id)) {
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_spin_lock((pthread_spinlock_t*)(spinlocks->data[*lock_id]));
  

}

// TODO: this might need a lock
void forthread_spin_trylock(int *lock_id, int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  
  if (!vis_valid(spinlocks,*lock_id)) {
    pthread_mutex_unlock(&(spinlocks->mutex));
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_spin_trylock((pthread_spinlock_t*)(spinlocks->data[*lock_id]));

}


// TODO: this might need a lock
void forthread_spin_unlock(int *lock_id, int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  
  if (!vis_valid(spinlocks,*lock_id)) {
    pthread_mutex_unlock(&(spinlocks->mutex));
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_spin_unlock((pthread_spinlock_t*)(spinlocks->data[*lock_id]));

}


/*************************************/
/*    spin variable routines         */
/*************************************/


void forthread_rwlock_destroy(int *rwlock_id, int *info) {

  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  pthread_mutex_lock(&(rwlocks->mutex));

  if (!is_valid(rwlocks,*rwlock_id)) {
    pthread_mutex_unlock(&(rwlocks->mutex));
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_rwlock_destroy(((pthread_rwlock_t*)(rwlocks->data[*rwlock_id])));

  if (*info) {
    pthread_mutex_unlock(&(rwlocks->mutex));
    return;
  }

  free(rwlocks->data[*rwlock_id]);
  rwlocks->data[*rwlock_id] = NULL;
  
  pthread_mutex_unlock(&(rwlocks->mutex));

}


void forthread_rwlock_init(int *rwlock_id, int *attr_id, int *info) {
  int i = 0;
  *info = FT_OK;
  pthread_rwlockattr_t *attr;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  pthread_mutex_lock(&(rwlocks->mutex));
  if (rwlocks->after == rwlocks->size) {
    // we exhausted the mutex id array, double space
    array_resize(&rwlocks,rwlocks->size*2);
  }
  rwlocks->data[rwlocks->after] = (pthread_rwlock_t*) malloc(sizeof(pthread_rwlock_t));

  if (*attr_id == -1) {
    attr = NULL;
  } else {
    attr = rwlock_attrs->data[*attr_id];
  }

  *info = pthread_rwlock_init((pthread_rwlock_t*)(rwlocks->data[rwlocks->after])
                               ,attr);

  if (*info) {
    pthread_mutex_unlock(&(rwlocks->mutex));
    return;
  }

  *rwlock_id = rwlocks->after;
  rwlocks->after++;
  
  pthread_mutex_unlock(&(rwlocks->mutex));

}


void forthread_rwlock_rdlock(int *lock_id, int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }
  
  if (!is_valid(rwlocks,*lock_id)) {
    pthread_mutex_unlock(&(rwlocks->mutex));
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_rwlock_rdlock((pthread_rwlock_t*)(rwlocks->data[*lock_id]));

}

void forthread_rwlock_tryrdlock(int *lock_id, int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }
  
  if (!is_valid(rwlocks,*lock_id)) {
    pthread_mutex_unlock(&(rwlocks->mutex));
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_rwlock_tryrdlock((pthread_rwlock_t*)(rwlocks->data[*lock_id]));

}


void forthread_rwlock_wrlock(int *lock_id, int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }
  
  if (!is_valid(rwlocks,*lock_id)) {
    pthread_mutex_unlock(&(rwlocks->mutex));
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_rwlock_wrlock((pthread_rwlock_t*)(rwlocks->data[*lock_id]));

}

void forthread_rwlock_trywrlock(int *lock_id, int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }
  
  if (!is_valid(rwlocks,*lock_id)) {
    pthread_mutex_unlock(&(rwlocks->mutex));
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_rwlock_trywrlock((pthread_rwlock_t*)(rwlocks->data[*lock_id]));

}

void forthread_rwlock_unlock(int *lock_id, int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }
  
  if (!is_valid(rwlocks,*lock_id)) {
    pthread_mutex_unlock(&(rwlocks->mutex));
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_rwlock_unlock((pthread_rwlock_t*)(rwlocks->data[*lock_id]));

}


void forthread_rwlock_timedrdlock(int *lock_id, long *ns, int *info) {
  struct timespec t;
  *info = FT_OK;

  // TODO: check if ns > 10^6 is allowed
  t.tv_sec = 0;
  t.tv_nsec = *ns;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }
  
  if (!is_valid(rwlocks,*lock_id)) {
    *info = FT_EINVALID;
    return;
  }
  
  *info = pthread_rwlock_timedrdlock((pthread_rwlock_t*)(rwlocks->data[*lock_id]),
                                 &t);

}

void forthread_rwlock_timedwrlock(int *lock_id, long *ns, int *info) {
  struct timespec t;
  *info = FT_OK;

  // TODO: check if ns > 10^6 is allowed
  t.tv_sec = 0;
  t.tv_nsec = *ns;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }
  
  if (!is_valid(rwlocks,*lock_id)) {
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_rwlock_timedwrlock((pthread_rwlock_t*)(rwlocks->data[*lock_id]),
                                 &t);

}






