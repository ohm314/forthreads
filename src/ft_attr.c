
#include "ft_attr.h"


/*****************************************/
/*      attribute object routines        */
/*****************************************/



void thread_attr_destroy(int *attr, int *info) {
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


void thread_attr_init(int *attr, int *info) {
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

void thread_attr_getdetachstate(int *attr, int *detachstate, int *info) {
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

  *info = pthread_attr_getdetachstate((pthread_attr_t*)(thread_attrs->data[*attr]),detachstate);

  pthread_mutex_unlock(&(thread_attrs->mutex));

}

void thread_attr_setdetachstate(int *attr, int *detachstate, int *info) {
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

  *info = pthread_attr_setdetachstate((pthread_attr_t*)(thread_attrs->data[*attr]),*detachstate);

  pthread_mutex_unlock(&(thread_attrs->mutex));

}

void thread_attr_getguardsize(int *attr, size_t *guardsize, int *info) {
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

  *info = pthread_attr_getguardsize((pthread_attr_t*)(thread_attrs->data[*attr]),guardsize);

  pthread_mutex_unlock(&(thread_attrs->mutex));

}

void thread_attr_setguardsize(int *attr, size_t *guardsize, int *info) {
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

  *info = pthread_attr_setguardsize((pthread_attr_t*)(thread_attrs->data[*attr]),*guardsize);

  pthread_mutex_unlock(&(thread_attrs->mutex));

}

void thread_attr_getinheritsched(int *attr, int *inheritsched, int *info) {
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

  *info = pthread_attr_getinheritsched((pthread_attr_t*)(thread_attrs->data[*attr]),inheritsched);

  pthread_mutex_unlock(&(thread_attrs->mutex));

}

void thread_attr_setinheritsched(int *attr, int *inheritsched, int *info) {
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

  *info = pthread_attr_setinheritsched((pthread_attr_t*)(thread_attrs->data[*attr]),*inheritsched);

  pthread_mutex_unlock(&(thread_attrs->mutex));

}

void thread_attr_getschedparam(int *attr, struct sched_param *param, int *info) {
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

  *info = pthread_attr_getschedparam((pthread_attr_t*)(thread_attrs->data[*attr]),param);


  pthread_mutex_unlock(&(thread_attrs->mutex));

}

void thread_attr_setschedparam(int *attr, struct sched_param *param, int *info) {
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

  *info = pthread_attr_setschedparam((pthread_attr_t*)(thread_attrs->data[*attr]),param);

  pthread_mutex_unlock(&(thread_attrs->mutex));

}

void thread_attr_getschedpolicy(int *attr, int *policy, int *info) {
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

  *info = pthread_attr_getschedpolicy((pthread_attr_t*)(thread_attrs->data[*attr]),policy);

  pthread_mutex_unlock(&(thread_attrs->mutex));

}

void thread_attr_setschedpolicy(int *attr, int *policy, int *info) {
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

  *info = pthread_attr_setschedpolicy((pthread_attr_t*)(thread_attrs->data[*attr]),*policy);

  pthread_mutex_unlock(&(thread_attrs->mutex));

}

void thread_attr_getscope(int *attr, int *scope, int *info) {
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

  *info = pthread_attr_getscope((pthread_attr_t*)(thread_attrs->data[*attr]),scope);

  pthread_mutex_unlock(&(thread_attrs->mutex));

}

void thread_attr_setscope(int *attr, int *scope, int *info) {
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

  *info = pthread_attr_setscope((pthread_attr_t*)(thread_attrs->data[*attr]),*scope);

  pthread_mutex_unlock(&(thread_attrs->mutex));

}

void thread_attr_getstacksize(int *attr, size_t *stacksize, int *info) {
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

  *info = pthread_attr_getstacksize((pthread_attr_t*)(thread_attrs->data[*attr]),stacksize);

  pthread_mutex_unlock(&(thread_attrs->mutex));

}

void thread_attr_setstacksize(int *attr, size_t *stacksize, int *info) {
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

  *info = pthread_attr_setstacksize((pthread_attr_t*)(thread_attrs->data[*attr]),*stacksize);

  pthread_mutex_unlock(&(thread_attrs->mutex));

}


/*****************************************/
/*       mutex attribute routines        */
/*****************************************/

void thread_mutexattr_destroy(int *attr,int *info) {
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


void thread_mutexattr_init(int *attr,int *info) {
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

void thread_mutexattr_getpshared(int *attr, int *pshared, int *info) {
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

  *info = pthread_mutexattr_getpshared(
                 (pthread_mutexattr_t*)(mutex_attrs->data[*attr]),
                 pshared);

  pthread_mutex_unlock(&(mutex_attrs->mutex));

}

void thread_mutexattr_setpshared(int *attr, int *pshared, int *info) {
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

  *info = pthread_mutexattr_setpshared(
                 (pthread_mutexattr_t*)(mutex_attrs->data[*attr]),
                 *pshared);

  pthread_mutex_unlock(&(mutex_attrs->mutex));

}

void thread_mutexattr_getprioceiling(int *attr, int *prioceiling, int *info) {
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

  *info = pthread_mutexattr_getprioceiling(
                 (pthread_mutexattr_t*)(mutex_attrs->data[*attr]),
                 prioceiling);

  pthread_mutex_unlock(&(mutex_attrs->mutex));

}

void thread_mutexattr_setprioceiling(int *attr, int *prioceiling, int *info) {
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

  *info = pthread_mutexattr_setprioceiling(
                 (pthread_mutexattr_t*)(mutex_attrs->data[*attr]),
                 *prioceiling);

  pthread_mutex_unlock(&(mutex_attrs->mutex));

}

void thread_mutexattr_getprotocol(int *attr, int *protocol, int *info) {
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

  *info = pthread_mutexattr_getprotocol(
                 (pthread_mutexattr_t*)(mutex_attrs->data[*attr]),
                 protocol);

  pthread_mutex_unlock(&(mutex_attrs->mutex));

}

void thread_mutexattr_setprotocol(int *attr, int *protocol, int *info) {
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

  *info = pthread_mutexattr_setprotocol(
                 (pthread_mutexattr_t*)(mutex_attrs->data[*attr]),
                 *protocol);

  pthread_mutex_unlock(&(mutex_attrs->mutex));

}

void thread_mutexattr_gettype(int *attr, int *type, int *info) {
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

  *info = pthread_mutexattr_gettype(
                 (pthread_mutexattr_t*)(mutex_attrs->data[*attr]),
                 type);

  pthread_mutex_unlock(&(mutex_attrs->mutex));

}

void thread_mutexattr_settype(int *attr, int *type, int *info) {
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

  *info = pthread_mutexattr_settype(
                 (pthread_mutexattr_t*)(mutex_attrs->data[*attr]),
                 *type);

  pthread_mutex_unlock(&(mutex_attrs->mutex));

}

/*****************************************************/
/*    condition attriubute variable routines         */
/*****************************************************/


void thread_condattr_destroy(int *attr,int *info) {
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


void thread_condattr_init(int *attr,int *info) {
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

void thread_condattr_getpshared(int *attr, int *pshared, int *info) {
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

  *info = pthread_condattr_getpshared(
                 (pthread_condattr_t*)(cond_attrs->data[*attr]),
                 pshared);

  pthread_mutex_unlock(&(cond_attrs->mutex));

}

void thread_condattr_setpshared(int *attr, int *pshared, int *info) {
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

  *info = pthread_condattr_setpshared(
                 (pthread_condattr_t*)(cond_attrs->data[*attr]),
                 *pshared);

  pthread_mutex_unlock(&(cond_attrs->mutex));

}

#ifndef __DARWIN
void thread_condattr_getclock(int *attr, int *clock_id, int *info) {
  *info = FT_OK;
  clockid_t cid; //we'll it casting onto an int. This may be dangerous

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

  *info = pthread_condattr_getclock(
                 (pthread_condattr_t*)(cond_attrs->data[*attr]),
                 &cid);
  *clock_id = (int)cid;

  pthread_mutex_unlock(&(cond_attrs->mutex));

}

void thread_condattr_setclock(int *attr, int *clock_id, int *info) {
  *info = FT_OK;
  clockid_t cid; //we'll be casting an int onto it. This may be dangerous.
  cid = (clockid_t)clock_id; // this works with libc 2.13 x86_64 - check if there are any notable exceptions

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

  *info = pthread_condattr_setclock(
                 (pthread_condattr_t*)(cond_attrs->data[*attr]),
                 cid);

  pthread_mutex_unlock(&(cond_attrs->mutex));

}
#endif


#ifdef _POSIX_BARRIERS
/**************************************************/
/*    barrier attribute variable routines         */
/**************************************************/


void thread_barrierattr_destroy(int *attr,int *info) {
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


void thread_barrierattr_init(int *attr,int *info) {
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

void thread_barrierattr_getpshared(int *attr, int *pshared, int *info) {
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

  *info = pthread_barrierattr_getpshared(
                 (pthread_barrierattr_t*)(barrier_attrs->data[*attr]),
                 pshared);

  pthread_mutex_unlock(&(barrier_attrs->mutex));

}

void thread_barrierattr_setpshared(int *attr, int *pshared, int *info) {
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

  *info = pthread_barrierattr_setpshared(
                 (pthread_barrierattr_t*)(barrier_attrs->data[*attr]),
                 *pshared);

  pthread_mutex_unlock(&(barrier_attrs->mutex));

}
#endif

/**************************************************/
/*    rwlock attribute variable routines         */
/**************************************************/


void thread_rwlockattr_destroy(int *attr,int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  pthread_mutex_lock(&(rwlock_attrs->mutex));

  if (!is_valid(rwlock_attrs,*attr)) {
    pthread_mutex_unlock(&(rwlock_attrs->mutex));
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_rwlockattr_destroy(((pthread_rwlockattr_t*)(
          rwlock_attrs->data[*attr])));

  if (*info) {
    pthread_mutex_unlock(&(rwlock_attrs->mutex));
    return;
  }

  free(rwlock_attrs->data[*attr]);
  rwlock_attrs->data[*attr] = NULL;

  pthread_mutex_unlock(&(rwlock_attrs->mutex));

}


void thread_rwlockattr_init(int *attr,int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  pthread_mutex_lock(&(rwlock_attrs->mutex));

  if (rwlock_attrs->after == rwlock_attrs->size) {
    // we exhausted the mutex attribute array, double space
    array_resize(&rwlock_attrs,rwlock_attrs->size*2);
  }
  rwlock_attrs->data[rwlock_attrs->after] = 
    (pthread_rwlockattr_t*) malloc(sizeof(pthread_rwlockattr_t));

  *info = pthread_rwlockattr_init(rwlock_attrs->data[rwlock_attrs->after]);

  if (*info) {
    pthread_mutex_unlock(&(rwlock_attrs->mutex));
    return;
  }

  *attr = rwlock_attrs->after;
  rwlock_attrs->after++;

  pthread_mutex_unlock(&(rwlock_attrs->mutex));

}

void thread_rwlockattr_getpshared(int *attr, int *pshared, int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  pthread_mutex_lock(&(rwlock_attrs->mutex));
  if (!is_valid(rwlock_attrs,*attr)) {
    pthread_mutex_unlock(&(rwlock_attrs->mutex));
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_rwlockattr_getpshared(
                 (pthread_rwlockattr_t*)(rwlock_attrs->data[*attr]),
                 pshared);

  pthread_mutex_unlock(&(rwlock_attrs->mutex));

}

void thread_rwlockattr_setpshared(int *attr, int *pshared, int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  pthread_mutex_lock(&(rwlock_attrs->mutex));
  if (!is_valid(rwlock_attrs,*attr)) {
    pthread_mutex_unlock(&(rwlock_attrs->mutex));
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_rwlockattr_setpshared(
                 (pthread_rwlockattr_t*)(rwlock_attrs->data[*attr]),
                 *pshared);

  pthread_mutex_unlock(&(rwlock_attrs->mutex));

}






