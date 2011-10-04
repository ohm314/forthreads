
#include "ft_attr.h"


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
