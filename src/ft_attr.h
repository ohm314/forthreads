#ifndef _FT_ATTR_H_
#define _FT_ATTR_H_

/**
 * NOTE:
 * The routines:
 * - pthread_attr_setstack 
 * - pthread_attr_setstackaddr
 * - pthread_attr_getstack 
 * - pthread_attr_getstackaddr
 * are not wrapped and shall not be visible to 
 * Fortran programs. This had to be done because Fortran generally does not
 * expose memory addresses to the programmer. For most applications it should
 * anyway not be necessary for the programmer to allocate the thread stack
 * manually.
 *
 * To explicitly set the size of the stack the routine thread_setstacksize
 * should be used.
 **/

#include <pthread.h>
#include "ft_consts.h"
#include "ft_data.h"

/*****************************************/
/*      attribute object routines        */
/*****************************************/

void thread_attr_destroy(int *attr, int *info);


void thread_attr_init(int *attr, int *info);

void thread_attr_getdetachstate(int *attr, int *detachstate, int *info);

void thread_attr_setdetachstate(int *attr, int *detachstate, int *info);

void thread_attr_getguardsize(int *attr, size_t *guardsize, int *info);

void thread_attr_setguardsize(int *attr, size_t *guardsize, int *info);

void thread_attr_getinheritsched(int *attr, int *inheritsched, int *info);

void thread_attr_setinheritsched(int *attr, int *inheritsched, int *info);

void thread_attr_getschedparam(int *attr, struct sched_param *param, int *info);

void thread_attr_setschedparam(int *attr, struct sched_param *param, int *info);

void thread_attr_getschedpolicy(int *attr, int *policy, int *info);

void thread_attr_setschedpolicy(int *attr, int *policy, int *info);
  
void thread_attr_getscope(int *attr, int *scope, int *info);

void thread_attr_setscope(int *attr, int *scope, int *info);

void thread_attr_getstacksize(int *attr, size_t *stacksize, int *info);

void thread_attr_setstacksize(int *attr, size_t *stacksize, int *info);
  
/*****************************************/
/*       mutex attribute routines        */
/*****************************************/

void thread_mutexattr_destroy(int *attr,int *info);

void thread_mutexattr_init(int *attr,int *info);

void thread_mutexattr_getpshared(int *attr, int *pshared, int *info);

void thread_mutexattr_setpshared(int *attr, int *pshared, int *info);

void thread_mutexattr_getprioceiling(int *attr, int *prioceiling, int *info);

void thread_mutexattr_setprioceiling(int *attr, int *prioceiling, int *info);

void thread_mutexattr_getprotocol(int *attr, int *protocol, int *info);

void thread_mutexattr_setprotocol(int *attr, int *protocol, int *info);

void thread_mutexattr_gettype(int *attr, int *type, int *info);

void thread_mutexattr_settype(int *attr, int *type, int *info);


/*****************************************************/
/*    condition attriubute variable routines         */
/*****************************************************/


void thread_condattr_destroy(int *attr,int *info);


void thread_condattr_init(int *attr,int *info);

void thread_condattr_getpshared(int *attr, int *pshared, int *info);

void thread_condattr_setpshared(int *attr, int *pshared, int *info);

#ifndef __DARWIN
void thread_condattr_getclock(int *attr, int *clock_id, int *info);

void thread_condattr_setclock(int *attr, int *clock_id, int *info);
#endif

#ifdef _POSIX_BARRIERS
/**************************************************/
/*    barrier attribute variable routines         */
/**************************************************/


void thread_barrierattr_destroy(int *attr,int *info);

void thread_barrierattr_init(int *attr,int *info);

void thread_barrierattr_getpshared(int *attr, int *pshared, int *info);

void thread_barrierattr_setpshared(int *attr, int *pshared, int *info);
#endif

/**************************************************/
/*    rwlock attribute variable routines         */
/**************************************************/


void thread_rwlockattr_destroy(int *attr,int *info);

void thread_rwlockattr_init(int *attr,int *info);

void thread_rwlockattr_getpshared(int *attr, int *pshared, int *info);

void thread_rwlockattr_setpshared(int *attr, int *pshared, int *info);



#endif //_FT_ATTR_H
