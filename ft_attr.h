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
 * To explicitly set the size of the stack the routine forthread_setstacksize
 * should be used.
 **/

#include <pthread.h>
#include "ft_consts.h"
#include "ft_data.h"

/*****************************************/
/*      attribute object routines        */
/*****************************************/

void forthread_attr_destroy(int *attr, int *info);


void forthread_attr_init(int *attr, int *info);

void forthread_attr_getdetachstate(int *attr, int *detachstate, int *info);

void forthread_attr_setdetachstate(int *attr, int *detachstate, int *info);

void forthread_attr_getguardsize(int *attr, int *guardsize, int *info);

void forthread_attr_setguardsize(int *attr, int *guardsize, int *info);

void forthread_attr_getinheritsched(int *attr, int *inheritsched, int *info);

void forthread_attr_setinheritsched(int *attr, int *inheritsched, int *info);

// implements pthreads getschedparam
void forthread_attr_getschedpriority(int *attr, int *sched_priority, int *info);

// implements pthreads setschedparam
void forthread_attr_setschedpriority(int *attr, int *sched_priority, int *info);

void forthread_attr_getschedpolicy(int *attr, int *policy, int *info);

void forthread_attr_setschedpolicy(int *attr, int *policy, int *info);
  
void forthread_attr_getscope(int *attr, int *scope, int *info);

void forthread_attr_setscope(int *attr, int *scope, int *info);

void forthread_attr_getstacksize(int *attr, int *stacksize, int *info);

void forthread_attr_setstacksize(int *attr, int *stacksize, int *info);
  
/*****************************************/
/*       mutex attribute routines        */
/*****************************************/

void forthread_mutexattr_destroy(int *attr,int *info);

void forthread_mutexattr_init(int *attr,int *info);

void forthread_mutexattr_getpshared(int *attr, int *pshared, int *info);

void forthread_mutexattr_setpshared(int *attr, int *pshared, int *info);

void forthread_mutexattr_getprioceiling(int *attr, int *prioceiling, int *info);

void forthread_mutexattr_setprioceiling(int *attr, int *prioceiling, int *info);

void forthread_mutexattr_getprotocol(int *attr, int *protocol, int *info);

void forthread_mutexattr_setprotocol(int *attr, int *protocol, int *info);

void forthread_mutexattr_gettype(int *attr, int *type, int *info);

void forthread_mutexattr_settype(int *attr, int *type, int *info);


/*****************************************************/
/*    condition attriubute variable routines         */
/*****************************************************/


void forthread_condattr_destroy(int *attr,int *info);


void forthread_condattr_init(int *attr,int *info);

void forthread_condattr_getpshared(int *attr, int *pshared, int *info);

void forthread_condattr_setpshared(int *attr, int *pshared, int *info);

void forthread_condattr_getclock(int *attr, int *clock_id, int *info);

void forthread_condattr_setclock(int *attr, int *clock_id, int *info);

/**************************************************/
/*    barrier attribute variable routines         */
/**************************************************/


void forthread_barrierattr_destroy(int *attr,int *info);

void forthread_barrierattr_init(int *attr,int *info);

void forthread_barrierattr_getpshared(int *attr, int *pshared, int *info);

void forthread_barrierattr_setpshared(int *attr, int *pshared, int *info);

/**************************************************/
/*    rwlock attribute variable routines         */
/**************************************************/


void forthread_rwlockattr_destroy(int *attr,int *info);

void forthread_rwlockattr_init(int *attr,int *info);

void forthread_rwlockattr_getpshared(int *attr, int *pshared, int *info);

void forthread_rwlockattr_setpshared(int *attr, int *pshared, int *info);



#endif //_FT_ATTR_H
