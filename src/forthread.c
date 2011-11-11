/*
 * C wrappers to pthreads to make wrapping with Fortran simpler
 */


#include "ft_consts.h"
#include "ft_data.h"
#include "ft_attr.h"
#include "forthread.h"


void thread_init(int *info) {
  int i = 0;
  pthread_t stid;
  static int init = 0;
  *info = FT_OK;

  if (init) {
    *info = FT_EINIT;
    return;
  }
  threads = NULL;
  array_init(&threads,INIT_SIZE);
  thread_attrs = NULL;
  array_init(&thread_attrs,INIT_SIZE);
  thread_keys = NULL;
  array_init(&thread_keys,INIT_SIZE);
  once_ctrls = NULL;
  array_init(&once_ctrls,INIT_SIZE);
  mutexes = NULL;
  array_init(&mutexes,INIT_SIZE);
  mutex_attrs = NULL;
  array_init(&mutex_attrs,INIT_SIZE);
  conds = NULL;
  array_init(&conds,INIT_SIZE);
  cond_attrs = NULL;
  array_init(&cond_attrs,INIT_SIZE);
  barriers = NULL;
  array_init(&barriers,INIT_SIZE);
  barrier_attrs = NULL;
  array_init(&barrier_attrs,INIT_SIZE);
  spinlocks = NULL;
  varray_init(&spinlocks,INIT_SIZE);
  rwlocks = NULL;
  array_init(&rwlocks,INIT_SIZE);
  rwlock_attrs = NULL;
  array_init(&rwlock_attrs,INIT_SIZE);
  // allocate and store the thread master ID
  threads->data[0] = (pthread_t*) malloc(sizeof(pthread_t));
  stid = pthread_self();
  memcpy(threads->data[0],&stid,sizeof(pthread_t));
  threads->after++;

  init = 1;
  is_initialized = init;
}

void thread_destroy(int* info) {
  // TODO: deallocate all, destroy all threads, mutexes
  *info = FT_OK;
}


/*****************************************/
/*         Thread routines               */
/*****************************************/

void thread_alloc(int *thread_id, int *info) {
  
  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }
  
  pthread_mutex_lock(&(threads->mutex));
  if (threads->after == threads->size) {
    // we exhausted the thread id array, double space
    array_resize(&threads,threads->size*2);
  }
  threads->data[threads->after] = (pthread_t*) malloc(sizeof(pthread_t));
  
  *thread_id = threads->after;
  threads->after++;

  pthread_mutex_unlock(&(threads->mutex));

}

void thread_create(int *thread_id, int *attr_id,
    void *(**start_routine)(void *), void *arg, int* info) {
  int i = 0;
  pthread_attr_t *attr;
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }
  
  if (!is_valid(threads,*thread_id)) {
    *info = FT_EINVALID;
    return;
  }
  
  pthread_mutex_lock(&(threads->mutex));

  if (*attr_id == -1) {
    // TODO: set to NULL
    attr = (pthread_attr_t*) malloc(sizeof(pthread_attr_t));
    pthread_attr_init(attr);
    pthread_attr_setdetachstate(attr, PTHREAD_CREATE_JOINABLE);
  } else {
    if (!is_valid(thread_attrs,*attr_id)) {
      pthread_mutex_unlock(&(threads->mutex));
      *info = FT_EINVALID;
      return;
    }
    attr = thread_attrs->data[*attr_id];
  }

  *info = pthread_create(threads->data[*thread_id], attr, (*start_routine), arg);

  // TODO remove
  if (*attr_id == -1)
    free(attr);

  if (*info) {
    pthread_mutex_unlock(&(threads->mutex));
    return;
  }

  pthread_mutex_unlock(&(threads->mutex));

}

void thread_detach(int *thread_id, int *info) {

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

void thread_equal(int *t1, int *t2, int *info) {
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

void thread_exit(void *value_ptr) {
  
  pthread_exit(value_ptr);

}

void thread_join(int *thread_id, void **value_ptr, int *info) {
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

void thread_cancel(int *thread_id, int *info) {
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

void thread_testcancel(int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }


  pthread_testcancel();

}


void thread_kill(int *thread_id, int *sig, int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  if (!is_valid(threads,*thread_id)) {
    *info = FT_EINVALID;
    return;
  }
  
  *info = pthread_kill(*((pthread_t*)(threads->data[*thread_id])),*sig);

}

void thread_once_init(int *once_ctrl, int *info) {

  *info = 0;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }
  
  pthread_mutex_lock(&(once_ctrls->mutex));
  if (once_ctrls->after == once_ctrls->size) {
    // we exhausted the thread id array, double space
    array_resize(&once_ctrls,once_ctrls->size*2);
  }
  once_ctrls->data[once_ctrls->after] = (pthread_once_t*) malloc(sizeof(pthread_once_t));

  *((pthread_once_t*)once_ctrls->data[once_ctrls->after]) = PTHREAD_ONCE_INIT;
  
  *once_ctrl = once_ctrls->after;
  once_ctrls->after++;

  pthread_mutex_unlock(&(once_ctrls->mutex));
}

void thread_once(int *once_ctrl_id, void (**routine)(void), int *info) {
  *info = FT_OK;

  if (!is_initialized)
    *info = FT_EINIT;
  else if (!is_valid(once_ctrls,*once_ctrl_id))
    *info = FT_EINVALID;
  else
    *info = pthread_once(once_ctrls->data[*once_ctrl_id],*routine);

}

void thread_self(int *thread_id, int *info) {
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


void thread_atfork(void (**prepare)(void), 
    void (**parent)(void), void (**child)(void), int *info) {

  *info = pthread_atfork(*prepare,*parent,*child);

}

// cannot be implemented using pthreads
void thread_cleanup_pop(int *execute, int *info) {
  *info = FT_EINVALID;


}

// cannot be implemented using pthreads
void thread_cleanup_push(void *(*routine)(void *), void *arg, int* info) {
  *info = FT_EINVALID;

}

void thread_getconcurrency(int *currlevel, int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  *currlevel = pthread_getconcurrency();

}


void thread_setconcurrency(int *new_level, int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  *info = pthread_setconcurrency(*new_level);

}

void thread_getcpuclockid(int *thread, int *clock_id, int *info) {
  *info = FT_OK;
  clockid_t cid; //we'll it casting onto an int. This may be dangerous

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  pthread_mutex_lock(&(threads->mutex));
  if (!is_valid(threads,*thread)) {
    pthread_mutex_unlock(&(threads->mutex));
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_getcpuclockid(
                 *((pthread_t*)(threads->data[*thread])),
                 &cid);
  *clock_id = (int)cid;

  pthread_mutex_unlock(&(threads->mutex));

}

// implements pthread_getschedparam
void thread_getschedparam(int *thread, int *policy, struct sched_param *param, int *info) {
  *info = FT_OK;
  ;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  pthread_mutex_lock(&(threads->mutex));
  if (!is_valid(threads,*thread)) {
    pthread_mutex_unlock(&(threads->mutex));
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_getschedparam(*((pthread_t*)(threads->data[*thread])),policy,param);

  pthread_mutex_unlock(&(threads->mutex));

}

// implements pthreads setschedparam
void forthread_setschedparam(int *thread, int *policy, struct sched_param *param, int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  pthread_mutex_lock(&(threads->mutex));
  if (!is_valid(threads,*thread)) {
    pthread_mutex_unlock(&(threads->mutex));
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_setschedparam(*((pthread_t*)(threads->data[*thread])),*policy,param);

  pthread_mutex_unlock(&(threads->mutex));

}

void thread_setschedprio(int *thread, int *prio, int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  pthread_mutex_lock(&(threads->mutex));
  if (!is_valid(threads,*thread)) {
    pthread_mutex_unlock(&(threads->mutex));
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_setschedprio(*((pthread_t*)(threads->data[*thread])),*prio);

  pthread_mutex_unlock(&(threads->mutex));

}

void thread_setcancelstate(int *state, int *oldstate, int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  *info = pthread_setcancelstate(*state,oldstate);

}

void thread_setcanceltype(int *type, int *oldtype, int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  *info = pthread_setcanceltype(*type,oldtype);

}

/*****************************************/
/*   storing private data in threads     */
/*****************************************/


void thread_key_delete(int *key_id, int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  pthread_mutex_lock(&(thread_keys->mutex));

  if (!is_valid(thread_keys,*key_id)) {
    pthread_mutex_unlock(&(thread_keys->mutex));
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_key_delete(*((pthread_key_t*)(thread_keys->data[*key_id])));

  pthread_mutex_unlock(&(thread_keys->mutex));
}

void thread_key_create(int *key_id,void (*destructor)(void *),int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  pthread_mutex_lock(&(thread_keys->mutex));
  if (thread_keys->after == thread_keys->size) {
    // we exhausted the mutex id array, double space
    array_resize(&thread_keys,thread_keys->size*2);
  }
  thread_keys->data[thread_keys->after] = (pthread_key_t*) malloc(sizeof(pthread_key_t));

  *info = pthread_key_create((pthread_key_t*)(thread_keys->data[thread_keys->after]),destructor);

  if (*info) {
    pthread_mutex_unlock(&(thread_keys->mutex));
    return;
  }

  *key_id = thread_keys->after;
  thread_keys->after++;

  pthread_mutex_unlock(&(thread_keys->mutex));

}

// the void pointer probably wont work
void thread_getspecific(int *key, void **value, int *info) {

  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  pthread_mutex_lock(&(thread_keys->mutex));

  if (!is_valid(thread_keys,*key)) {
    pthread_mutex_unlock(&(thread_keys->mutex));
    *info = FT_EINVALID;
    return;
  }

  *value = pthread_getspecific(*((pthread_key_t*)(thread_keys->data[*key])));

  pthread_mutex_unlock(&(thread_keys->mutex));
}

// the void pointer probably wont work
void thread_setspecific(int *key, void **value, int *info) {

  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  pthread_mutex_lock(&(thread_keys->mutex));

  if (!is_valid(thread_keys,*key)) {
    pthread_mutex_unlock(&(thread_keys->mutex));
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_setspecific(*((pthread_key_t*)(thread_keys->data[*key])),
                              *value);

  pthread_mutex_unlock(&(thread_keys->mutex));
}


/*****************************************/
/*             mutex routines            */
/*****************************************/

void thread_mutex_destroy(int *mutex_id, int *info) {

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


void thread_mutex_init(int *mutex_id, int *attr_id, int *info) {
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
    attr = mutex_attrs->data[*attr_id];
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

void thread_mutex_lock(int *mutex_id, int *info) {
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

void thread_mutex_trylock(int *mutex_id, int *info) {
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

void thread_mutex_unlock(int *mutex_id, int *info) {
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

void thread_mutex_getprioceiling(int *mutex, int *prioceiling, int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  pthread_mutex_lock(&(mutexes->mutex));
  if (!is_valid(mutexes,*mutex)) {
    pthread_mutex_unlock(&(mutexes->mutex));
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_mutex_getprioceiling(
                 (pthread_mutex_t*)(mutexes->data[*mutex]),
                 prioceiling);

  pthread_mutex_unlock(&(mutexes->mutex));

}

void thread_mutex_setprioceiling(int *mutex, int *prioceiling, int *old_ceiling, int *info) {
  *info = FT_OK;

  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }

  pthread_mutex_lock(&(mutexes->mutex));
  if (!is_valid(mutexes,*mutex)) {
    pthread_mutex_unlock(&(mutexes->mutex));
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_mutex_setprioceiling(
                 (pthread_mutex_t*)(mutexes->data[*mutex]),
                 *prioceiling,old_ceiling);

  pthread_mutex_unlock(&(mutexes->mutex));

}

// TODO see what we can do with timespec
void thread_mutex_timedlock(int *mutex, struct timespec *abs_timeout, int *info) {
  *info = FT_OK;


  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }
  
  if (!is_valid(mutexes,*mutex)) {
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_mutex_timedlock((pthread_mutex_t*)(mutexes->data[*mutex]),
                                 abs_timeout);

}


/*****************************************/
/*    condition variable routines        */
/*****************************************/


void thread_cond_destroy(int *cond_id, int *info) {

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


void thread_cond_init(int *cond_id, int *attr_id, int *info) {
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

void thread_cond_timedwait(int *cond_id, int *mutex_id, struct timespec *abstime, int *info) {
  *info = FT_OK;

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
                                 abstime);

}


void thread_cond_wait(int *cond_id, int *mutex_id, int *info) {
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


void thread_cond_broadcast(int *cond_id, int *info) {
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


void thread_cond_signal(int *cond_id, int *info) {
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



/****************************************/
/*    barrier variable routines         */
/****************************************/

void thread_barrier_destroy(int *barrier_id, int *info) {

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


void thread_barrier_init(int *barrier_id, int *attr_id, int *count, int *info) {
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

void thread_barrier_wait(int *barrier_id, int *info) {
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



/*************************************/
/*    spin variable routines         */
/*************************************/

void thread_spin_destroy(int *spinlock_id, int *info) {

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


void thread_spin_init(int *spinlock_id, int *pshared, int *info) {
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
void thread_spin_lock(int *lock_id, int *info) {
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
void thread_spin_trylock(int *lock_id, int *info) {
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
void thread_spin_unlock(int *lock_id, int *info) {
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
/*     variable routines         */
/*************************************/


void thread_rwlock_destroy(int *rwlock_id, int *info) {

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


void thread_rwlock_init(int *rwlock_id, int *attr_id, int *info) {
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


void thread_rwlock_rdlock(int *lock_id, int *info) {
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

void thread_rwlock_tryrdlock(int *lock_id, int *info) {
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


void thread_rwlock_wrlock(int *lock_id, int *info) {
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

void thread_rwlock_trywrlock(int *lock_id, int *info) {
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

void thread_rwlock_unlock(int *lock_id, int *info) {
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


void thread_rwlock_timedrdlock(int *lock_id, struct timespec *abs_timeout, int *info) {
  *info = FT_OK;


  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }
  
  if (!is_valid(rwlocks,*lock_id)) {
    *info = FT_EINVALID;
    return;
  }
  
  *info = pthread_rwlock_timedrdlock((pthread_rwlock_t*)(rwlocks->data[*lock_id]),
                                 abs_timeout);

}

void thread_rwlock_timedwrlock(int *lock_id, struct timespec *abs_timeout, int *info) {
  *info = FT_OK;


  if (!is_initialized) {
    *info = FT_EINIT;
    return;
  }
  
  if (!is_valid(rwlocks,*lock_id)) {
    *info = FT_EINVALID;
    return;
  }

  *info = pthread_rwlock_timedwrlock((pthread_rwlock_t*)(rwlocks->data[*lock_id]),
                                 abs_timeout);

}





