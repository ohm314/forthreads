#ifndef FORT_PTHREAD_H_
#define FORT_PTHREAD_H_

#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



void forthread_init(int *info);

void forthread_destroy(int* info);


/*****************************************/


void forthread_create(int *thread_id, int *attr_id,
                      void *(*start_routine)(void *),
                      void *arg, int* info);

void forthread_detach(int *thread_id, int *info);

void forthread_equal(int *t1, int *t2, int *info);


void forthread_exit(void *value_ptr);
  

void forthread_join(int *thread_id, void **value_ptr, int *info);

void forthread_cancel(int *thread_id, int *info);

void forthread_once(int *once_ctrl_id, void (*routine)(void), int *info);

void forthread_self(int *thread_id, int *info);


void forthread_atfork(void (*prepare)(void), 
                      void (*parent)(void), 
                      void (*child)(void), int *info);



/*****************************************/
/*             mutex routines            */
/*****************************************/

void forthread_mutex_destroy(int *mutex_id, int *info);



void forthread_mutex_init(int *mutex_id, int *attr_id, int *info);

void forthread_lock(int *mutex_id, int *info);

void forthread_trylock(int *mutex_id, int *info);

void forthread_mutex_unlock(int *mutex_id, int *info);



/*****************************************/
/*    condition variable routines        */
/*****************************************/


void forthread_cond_destroy(int *cond_id, int *info);


void forthread_cond_init(int *cond_id, int *attr_id, int *info);

void forthread_cond_timedwait(int *cond_id, int *mutex_id, long *ns, int *info);


void forthread_cond_wait(int *cond_id, int *mutex_id, int *info);


void forthread_cond_broadcast(int *cond_id, int *info);


void forthread_cond_signal(int *cond_id, int *info);



/****************************************/
/*    barrier variable routines         */
/****************************************/

void forthread_barrier_destroy(int *barrier_id, int *info);


void forthread_barrier_init(int *barrier_id, int *attr_id, int *count, int *info);

void forthread_barrier_wait(int *barrier_id, int *info);



/*************************************/
/*    spin variable routines         */
/*************************************/

void forthread_spin_destroy(int *spinlock_id, int *info);


void forthread_spin_init(int *spinlock_id, int *pshared, int *info);

void forthread_spin_lock(int *lock_id, int *info);

void forthread_spin_trylock(int *lock_id, int *info);


void forthread_spin_unlock(int *lock_id, int *info);

/*************************************/
/*    rwlock variable routines         */
/*************************************/


void forthread_rwlock_destroy(int *rwlock_id, int *info);

void forthread_rwlock_init(int *rwlock_id, int *attr_id, int *info);


void forthread_rwlock_rdlock(int *lock_id, int *info);

void forthread_rwlock_tryrdlock(int *lock_id, int *info);


void forthread_rwlock_wrlock(int *lock_id, int *info);

void forthread_rwlock_trywrlock(int *lock_id, int *info);

void forthread_rwlock_unlock(int *lock_id, int *info);


void forthread_rwlock_timedrdlock(int *lock_id, long *ns, int *info);

void forthread_rwlock_timedwrlock(int *lock_id, long *ns, int *info);



#endif //FORT_PTHREAD_H_
