#ifndef FORT_PTHREAD_H_
#define FORT_PTHREAD_H_

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>




void thread_init(int *info);

void thread_destroy(int* info);


/*****************************************/

void thread_alloc(int *thread_id, int *info);

void thread_create(int *thread_id, int *attr_id,
                      void *(**start_routine)(void *),
                      void *arg, int* info);

void thread_detach(int *thread_id, int *info);

void thread_equal(int *t1, int *t2, int *info);


void thread_exit(void *value_ptr);
  

void thread_join(int *thread_id, void **value_ptr, int *info);

void thread_cancel(int *thread_id, int *info);

void thread_kill(int *thread_id, int *sig, int *info);

void thread_once_init(int *once_ctrl, int *info);

void thread_once(int *once_ctrl_id, void (**routine)(void), int *info);

void thread_self(int *thread_id, int *info);


void thread_atfork(void (**prepare)(void), 
                      void (**parent)(void), 
                      void (**child)(void), int *info);

// cannot be implemented using pthreads
// this should be done in fortran
// TODO provide fortran implementation if possible
void thread_cleanup_pop(int *execute,
                           int *info);

// cannot be implemented using pthreads
// this should be done in fortran
// TODO provide fortran implementation if possible
void thread_cleanup_push(void *(*routine)(void *),
                            void *arg, int* info);

void thread_getconcurrency(int *currlevel, int *info);

void thread_setconcurrency(int *newlevel, int *info);


#ifndef __DARWIN
void thread_getcpuclockid(int *thread, int *clock_id, int *info);
#endif

void thread_getschedparam(int *thread, int *policy, struct sched_param *param, int *info);
void thread_setschedparam(int *thread, int *policy, struct sched_param *param, int *info);
#ifndef __DARWIN
void thread_setschedprio(int *thread, int *prio, int *info);
#endif

void thread_setcancelstate(int *state, int *oldstate, int *info);
void thread_setcanceltype(int *type, int *oldtype, int *info);

  
/*****************************************/
/*   storing private data in threads     */
/*****************************************/

void thread_key_delete(int *key_id, int *info);

void thread_key_create(int *key_id,void (*destructor)(void *),int *info);

void thread_getspecific(int *key, void **value, int *info);

void thread_setspecific(int *key, void **value, int *info);

/*****************************************/
/*             mutex routines            */
/*****************************************/

void thread_mutex_destroy(int *mutex_id, int *info);

void thread_mutex_init(int *mutex_id, int *attr_id, int *info);

void thread_mutex_lock(int *mutex_id, int *info);

void thread_mutex_trylock(int *mutex_id, int *info);

void thread_mutex_unlock(int *mutex_id, int *info);

void thread_mutex_getprioceiling(int *mutex, int *prioceiling, int *info);

void thread_mutex_setprioceiling(int *mutex, int *prioceiling, int *old_ceiling, int *info);

#ifndef __DARWIN
void thread_mutex_timedlock(int *mutex, struct timespec *abs_timeout, int *info);
#endif

/*****************************************/
/*    condition variable routines        */
/*****************************************/


void thread_cond_destroy(int *cond_id, int *info);

void thread_cond_init(int *cond_id, int *attr_id, int *info);

void thread_cond_timedwait(int *cond_id, int *mutex_id, struct timespec *abstime, int *info);

void thread_cond_wait(int *cond_id, int *mutex_id, int *info);

void thread_cond_broadcast(int *cond_id, int *info);

void thread_cond_signal(int *cond_id, int *info);



#ifdef _POSIX_BARRIERS
/****************************************/
/*    barrier variable routines         */
/****************************************/

void thread_barrier_destroy(int *barrier_id, int *info);


void thread_barrier_init(int *barrier_id, int *attr_id, int *count, int *info);

void thread_barrier_wait(int *barrier_id, int *info);
#endif


#ifndef __DARWIN
/*************************************/
/*    spin variable routines         */
/*************************************/

void thread_spin_destroy(int *spinlock_id, int *info);


void thread_spin_init(int *spinlock_id, int *pshared, int *info);

void thread_spin_lock(int *lock_id, int *info);

void thread_spin_trylock(int *lock_id, int *info);


void thread_spin_unlock(int *lock_id, int *info);
#endif

/*************************************/
/*    rwlock variable routines       */
/*************************************/


void thread_rwlock_destroy(int *rwlock_id, int *info);

void thread_rwlock_init(int *rwlock_id, int *attr_id, int *info);

void thread_rwlock_rdlock(int *lock_id, int *info);

void thread_rwlock_tryrdlock(int *lock_id, int *info);

void thread_rwlock_wrlock(int *lock_id, int *info);

void thread_rwlock_trywrlock(int *lock_id, int *info);

void thread_rwlock_unlock(int *lock_id, int *info);

#ifndef __DARWIN
void thread_rwlock_timedrdlock(int *lock_id, struct timespec *abs_timeout, int *info);

void thread_rwlock_timedwrlock(int *lock_id, struct timespec *abs_timeout, int *info);
#endif


#endif //FORT_PTHREAD_H_
