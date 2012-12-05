

subroutine forthread_init(info)

use forthread_data
implicit none

#include "ciface.h"

integer, intent(out) :: info

allocate(routine_table(init_size))
routine_table_size = init_size

call thread_init(info)

call thread_mutex_init(routine_table_mutex,-1,info)

end subroutine forthread_init


subroutine forthread_destroy(info)

use forthread_data
implicit none

#include "ciface.h"

integer, intent(out) :: info

deallocate(routine_table)
routine_table_size = 0
call thread_mutex_destroy(routine_table_mutex,info)
call thread_destroy(info)
end subroutine forthread_destroy


subroutine forthread_create(thread_id,attr_id,run,arg,info)

use iso_c_binding
use forthread_data
implicit none

#include "ciface.h"

integer, intent(out)                :: thread_id
integer, intent(in)                 :: attr_id
procedure(i_run)                    :: run !type i_run
integer                   , target  :: arg
integer, intent(out)                :: info

integer                                      :: i
procedure(i_start_routine), bind(c), pointer :: start_routinep
type(ptr_t_run), dimension(:), pointer       :: tmp
type(t_run), pointer :: runp
call thread_mutex_lock(routine_table_mutex,info)

call thread_alloc(thread_id,info)
if (thread_id.gt.routine_table_size) then
   nullify(tmp)
   allocate(tmp(routine_table_size*2))
   do i=1,routine_table_size
       tmp(i) = routine_table(i)
   enddo
   deallocate(routine_table)
   routine_table => tmp
   routine_table_size = routine_table_size*2
endif
allocate(routine_table(thread_id)%t)
routine_table(thread_id)%t%run => run
routine_table(thread_id)%t%arg => arg
start_routinep => start_routine

call thread_create(thread_id,attr_id,c_funloc(start_routinep),&
                   c_loc(routine_table(thread_id)%t),info)

call thread_mutex_unlock(routine_table_mutex,info)


end subroutine forthread_create
    
subroutine forthread_detach(thread_id,info)
implicit none

#include "ciface.h"

integer, intent(in)             :: thread_id
integer, intent(out)            :: info

call thread_detach(thread_id,info)
end subroutine forthread_detach

subroutine forthread_equal(t1,t2,info)
implicit none

#include "ciface.h"

integer, intent(in)             :: t1
integer, intent(in)             :: t2
integer, intent(out)            :: info

call thread_equal(t1,t2,info)
end subroutine forthread_equal

! Exits the current thread
subroutine forthread_exit(val)

use iso_c_binding
implicit none

#include "ciface.h"


integer,            pointer :: val


call thread_exit(c_loc(val))
end subroutine forthread_exit

subroutine forthread_join(thread_id,val,info)

use iso_c_binding
implicit none

#include "ciface.h"


integer, intent(in)         :: thread_id
integer,             pointer:: val
integer, intent(out)        :: info

type(c_ptr)                 :: value_ptr
call thread_join(thread_id,value_ptr,info)
call c_f_pointer(value_ptr,val)
end subroutine forthread_join

subroutine forthread_cancel(thread_id,info)
implicit none

#include "ciface.h"

integer, intent(in)         :: thread_id
integer, intent(out)        :: info

call thread_cancel(thread_id,info)
end subroutine forthread_cancel

subroutine forthread_kill(thread_id,sig,info)
implicit none

#include "ciface.h"

integer, intent(in)         :: thread_id
integer, intent(in)         :: sig
integer, intent(out)        :: info

call thread_kill(thread_id,sig,info)
end subroutine forthread_kill

subroutine forthread_once_init(once_ctrl_id,info)
use iso_c_binding
implicit none

integer, intent(out) :: once_ctrl_id
integer, intent(out) :: info

#include "ciface.h"

call thread_once_init(once_ctrl_id,info)
end subroutine

subroutine forthread_once(once_ctrl_id,init_routine,info)
use iso_c_binding
use forthread_data
implicit none

#include "ciface.h"

integer, intent(in)                 :: once_ctrl_id
procedure(i_once), bind(c)           :: init_routine
! dangerous but works! (gfortran)
! TODO test in other compilers
integer, intent(out)                :: info


call thread_once(once_ctrl_id,c_funloc(init_routine),info)

end subroutine

! TODO implement thread_atfork

subroutine forthread_getconcurrency(currlevel,info)
implicit none

#include "ciface.h"

integer       , intent(out)     :: currlevel
integer       , intent(out)     :: info

call thread_getconcurrency(currlevel,info)

end subroutine forthread_getconcurrency

subroutine forthread_setconcurrency(newlevel,info)
implicit none

#include "ciface.h"

integer       , intent(in)      :: newlevel
integer       , intent(out)     :: info

call thread_setconcurrency(newlevel,info)

end subroutine forthread_setconcurrency

#ifndef __DARWIN
subroutine forthread_getcpuclockid(thread,clock_id,info)
implicit none

#include "ciface.h"

integer       , intent(in)      :: thread
integer       , intent(out)     :: clock_id
integer       , intent(out)     :: info

call thread_getcpuclockid(thread,clock_id,info)

end subroutine forthread_getcpuclockid
#endif

subroutine forthread_getschedparam(thread,policy,param,info)

use iso_c_binding
use forthread_types
implicit none

#include "ciface.h"
integer       , intent(in)      :: thread
integer       , intent(out)     :: policy
type(sched_param), intent(out)  :: param
integer       , intent(out)     :: info

call thread_getschedparam(thread,policy,param,info)
end subroutine forthread_getschedparam

subroutine forthread_setschedparam(thread,policy,param,info)

use iso_c_binding
use forthread_types
implicit none

#include "ciface.h"
integer       , intent(in)      :: thread
integer       , intent(in)      :: policy
type(sched_param), intent(in)   :: param
integer       , intent(out)     :: info

call thread_setschedparam(thread,policy,param,info)
end subroutine forthread_setschedparam

#ifndef __DARWIN
subroutine forthread_setschedprio(thread,prio,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: thread
integer       , intent(in)      :: prio
integer       , intent(out)     :: info

call thread_setschedprio(thread,prio,info)
end subroutine forthread_setschedprio
#endif

subroutine forthread_setcancelstate(state,oldstate,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: state
integer       , intent(out)     :: oldstate
integer       , intent(out)     :: info

call thread_setcancelstate(state,oldstate,info)
end subroutine forthread_setcancelstate

subroutine forthread_setcanceltype(ctype,oldctype,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: ctype
integer       , intent(out)     :: oldctype
integer       , intent(out)     :: info

call thread_setcanceltype(ctype,oldctype,info)
end subroutine forthread_setcanceltype

!*****************************************!
!*   sharing private data in threads     *!
!*****************************************!

subroutine forthread_key_delete(key_id,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: key_id
integer       , intent(out)     :: info

call thread_key_delete(key_id,info)
end subroutine forthread_key_delete

subroutine forthread_key_create(key_id,destructor,info)
use iso_c_binding
use forthread_data
implicit none

#include "ciface.h"

integer, intent(out)                :: key_id
procedure(i_destructor), bind(c)    :: destructor
! dangerous but works! (gfortran)
! TODO test in other compilers
integer, intent(out)                :: info


call thread_key_create(key_id,c_funloc(destructor),info)

end subroutine

! no wrappers provided for the following two routines
!void thread_getspecific(int *key, void **value, int *info);

!void thread_setspecific(int *key, void **value, int *info);



!*****************************************!
!*             mutex routines            *!
!*****************************************!


subroutine forthread_mutex_destroy(mutex_id,info)
implicit none

#include "ciface.h"
integer, intent(in)      :: mutex_id
integer, intent(out)     :: info

call thread_mutex_destroy(mutex_id,info)
end subroutine forthread_mutex_destroy

subroutine forthread_mutex_init(mutex_id,attr_id,info)
implicit none

#include "ciface.h"
integer, intent(out)     :: mutex_id
integer, intent(in)      :: attr_id
integer, intent(out)     :: info

call thread_mutex_init(mutex_id,attr_id,info)
end subroutine forthread_mutex_init

subroutine forthread_mutex_lock(mutex_id,info)
implicit none

#include "ciface.h"
integer, intent(in)      :: mutex_id
integer, intent(out)     :: info

call thread_mutex_lock(mutex_id,info)
end subroutine forthread_mutex_lock

subroutine forthread_mutex_trylock(mutex_id,info)
implicit none

#include "ciface.h"
integer, intent(in)      :: mutex_id
integer, intent(out)     :: info

call thread_mutex_trylock(mutex_id,info)
end subroutine forthread_mutex_trylock

subroutine forthread_mutex_unlock(mutex_id,info)
implicit none

#include "ciface.h"
integer, intent(in)      :: mutex_id
integer, intent(out)     :: info

call thread_mutex_unlock(mutex_id,info)
end subroutine forthread_mutex_unlock

subroutine forthread_mutex_getprioceiling(mutex,prioceiling,info)
implicit none

#include "ciface.h"
integer, intent(in)      :: mutex
integer, intent(out)     :: prioceiling
integer, intent(out)     :: info

call thread_mutex_getprioceiling(mutex,prioceiling,info)
end subroutine forthread_mutex_getprioceiling

subroutine forthread_mutex_setprioceiling(mutex,prioceiling,old_ceiling,info)
implicit none

#include "ciface.h"
integer, intent(in)      :: mutex
integer, intent(in)      :: prioceiling
integer, intent(out)     :: old_ceiling
integer, intent(out)     :: info

call thread_mutex_setprioceiling(mutex,prioceiling,old_ceiling,info)
end subroutine forthread_mutex_setprioceiling

#ifndef __DARWIN
subroutine forthread_mutex_timedlock(mutex,abs_timeout,info)
use forthread_types
implicit none

#include "ciface.h"
integer,        intent(in)      :: mutex
type(timespec), intent(in)      :: abs_timeout
integer,        intent(out)     :: info

call thread_mutex_timedlock(mutex,abs_timeout,info)

end subroutine forthread_mutex_timedlock
#endif

!*****************************************!
!*    condition variable routines        *!
!*****************************************!

subroutine forthread_cond_destroy(cond_id,info)
implicit none

#include "ciface.h"
integer, intent(in)      :: cond_id
integer, intent(out)     :: info

call thread_cond_destroy(cond_id,info)
end subroutine forthread_cond_destroy

subroutine forthread_cond_init(cond_id,attr_id,info)
implicit none

#include "ciface.h"
integer, intent(out)     :: cond_id
integer, intent(in)      :: attr_id
integer, intent(out)     :: info

call thread_cond_init(cond_id,attr_id,info)
end subroutine forthread_cond_init

subroutine forthread_cond_timedwait(mutex,abstime,info)
use forthread_types
implicit none

#include "ciface.h"
integer, intent(in)      :: mutex
type(timespec), intent(in)      :: abstime
integer, intent(out)     :: info

call thread_cond_timedwait(mutex,abstime,info)
end subroutine forthread_cond_timedwait

subroutine forthread_cond_wait(cond_id,mutex_id,info)
implicit none

#include "ciface.h"
integer, intent(in)      :: cond_id
integer, intent(in)      :: mutex_id
integer, intent(out)     :: info

call thread_cond_wait(cond_id,mutex_id,info)
end subroutine forthread_cond_wait

subroutine forthread_cond_broadcast(cond_id,info)
implicit none

#include "ciface.h"
integer, intent(in)      :: cond_id
integer, intent(out)     :: info

call thread_cond_broadcast(cond_id,info)
end subroutine forthread_cond_broadcast

subroutine forthread_cond_signal(cond_id,info)
implicit none

#include "ciface.h"
integer, intent(in)      :: cond_id
integer, intent(out)     :: info

call thread_cond_signal(cond_id,info)
end subroutine forthread_cond_signal


#ifndef __DARWIN
!****************************************!
!*    barrier variable routines         *!
!****************************************!



subroutine forthread_barrier_destroy(barrier_id,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: barrier_id
integer       , intent(out)     :: info

call thread_barrier_destroy(barrier_id,info)
end subroutine forthread_barrier_destroy



subroutine forthread_barrier_init(barrier_id,attr_id,tcount,info)
implicit none

#include "ciface.h"
integer       , intent(out)     :: barrier_id
integer       , intent(in)      :: attr_id
integer       , intent(in)      :: tcount
integer       , intent(out)     :: info

call thread_barrier_init(barrier_id,attr_id,tcount,info)
end subroutine forthread_barrier_init



subroutine forthread_barrier_wait(barrier_id,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: barrier_id
integer       , intent(out)     :: info

call thread_barrier_wait(barrier_id,info)
end subroutine forthread_barrier_wait


!*************************************!
!*    spin variable routines         *!
!*************************************!


subroutine forthread_spin_destroy(spinlock_id,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: spinlock_id
integer       , intent(out)     :: info

call thread_spin_destroy(spinlock_id,info)
end subroutine forthread_spin_destroy



subroutine forthread_spin_init(spinlock_id,pshared,info)
implicit none

#include "ciface.h"
integer       , intent(out)     :: spinlock_id
integer       , intent(in)      :: pshared
integer       , intent(out)     :: info

call thread_spin_init(spinlock_id,pshared,info)
end subroutine forthread_spin_init



subroutine forthread_spin_lock(lock_id,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: lock_id
integer       , intent(out)     :: info

call thread_spin_lock(lock_id,info)
end subroutine forthread_spin_lock



subroutine forthread_spin_trylock(lock_id,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: lock_id
integer       , intent(out)     :: info

call thread_spin_trylock(lock_id,info)
end subroutine forthread_spin_trylock



subroutine forthread_spin_unlock(lock_id,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: lock_id
integer       , intent(out)     :: info

call thread_spin_unlock(lock_id,info)
end subroutine forthread_spin_unlock

#endif

!*************************************!
!*    rwlock variable routines       *!
!*************************************!


subroutine forthread_rwlock_destroy(rwlock_id,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: rwlock_id
integer       , intent(out)     :: info

call thread_rwlock_destroy(rwlock_id,info)
end subroutine forthread_rwlock_destroy



subroutine forthread_rwlock_init(rwlock_id,attr_id,info)
implicit none

#include "ciface.h"
integer       , intent(out)     :: rwlock_id
integer       , intent(in)      :: attr_id
integer       , intent(out)     :: info

call thread_rwlock_init(rwlock_id,attr_id,info)
end subroutine forthread_rwlock_init



subroutine forthread_rwlock_rdlock(lock_id,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: lock_id
integer       , intent(out)     :: info

call thread_rwlock_rdlock(lock_id,info)
end subroutine forthread_rwlock_rdlock



subroutine forthread_rwlock_tryrdlock(lock_id,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: lock_id
integer       , intent(out)     :: info

call thread_rwlock_tryrdlock(lock_id,info)
end subroutine forthread_rwlock_tryrdlock



subroutine forthread_rwlock_wrlock(lock_id,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: lock_id
integer       , intent(out)     :: info

call thread_rwlock_wrlock(lock_id,info)
end subroutine forthread_rwlock_wrlock



subroutine forthread_rwlock_trywrlock(lock_id,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: lock_id
integer       , intent(out)     :: info

call thread_rwlock_trywrlock(lock_id,info)
end subroutine forthread_rwlock_trywrlock



subroutine forthread_rwlock_unlock(lock_id,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: lock_id
integer       , intent(out)     :: info

call thread_rwlock_unlock(lock_id,info)
end subroutine forthread_rwlock_unlock



#ifndef __DARWIN
subroutine forthread_rwlock_timedrdlock(lock_id,abs_timeout,info)
use forthread_types
implicit none

#include "ciface.h"
integer       , intent(in)      :: lock_id
type(timespec), intent(in)      :: abs_timeout
integer       , intent(out)     :: info

call thread_rwlock_timedrdlock(lock_id,abs_timeout,info)
end subroutine forthread_rwlock_timedrdlock



subroutine forthread_rwlock_timedwrlock(lock_id,abs_timeout,info)
use forthread_types
implicit none

#include "ciface.h"
integer       , intent(in)      :: lock_id
type(timespec), intent(in)      :: abs_timeout
integer       , intent(out)     :: info

call thread_rwlock_timedwrlock(lock_id,abs_timeout,info)
end subroutine forthread_rwlock_timedwrlock
#endif

!*****************************************!
!*      attribute object routines        *!
!*****************************************!


subroutine forthread_attr_destroy(attr,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer       , intent(out)     :: info

call thread_attr_destroy(attr,info)
end subroutine forthread_attr_destroy



subroutine forthread_attr_init(attr,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer       , intent(out)     :: info

call thread_attr_init(attr,info)
end subroutine forthread_attr_init



subroutine forthread_attr_getdetachstate(attr,detachstate,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer       , intent(out)     :: detachstate
integer       , intent(out)     :: info

call thread_attr_getdetachstate(attr,detachstate,info)
end subroutine forthread_attr_getdetachstate



subroutine forthread_attr_setdetachstate(attr,detachstate,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer       , intent(in)      :: detachstate
integer       , intent(out)     :: info

call thread_attr_setdetachstate(attr,detachstate,info)
end subroutine forthread_attr_setdetachstate



subroutine forthread_attr_getguardsize(attr,guardsize,info)
use forthread_types
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer(size_t), intent(out)    :: guardsize
integer       , intent(out)     :: info

call thread_attr_getguardsize(attr,guardsize,info)
end subroutine forthread_attr_getguardsize



subroutine forthread_attr_setguardsize(attr,guardsize,info)
use forthread_types
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer(size_t), intent(in)     :: guardsize
integer       , intent(out)     :: info

call thread_attr_setguardsize(attr,guardsize,info)
end subroutine forthread_attr_setguardsize



subroutine forthread_attr_getinheritsched(attr,inheritsched,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer       , intent(out)     :: inheritsched
integer       , intent(out)     :: info

call thread_attr_getinheritsched(attr,inheritsched,info)
end subroutine forthread_attr_getinheritsched



subroutine forthread_attr_setinheritsched(attr,inheritsched,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer       , intent(in)      :: inheritsched
integer       , intent(out)     :: info

call thread_attr_setinheritsched(attr,inheritsched,info)
end subroutine forthread_attr_setinheritsched



subroutine forthread_attr_getschedparam(attr,param,info)
use forthread_types
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
type(sched_param), intent(out)  :: param
integer       , intent(out)     :: info

call thread_attr_getschedparam(attr,param,info)
end subroutine forthread_attr_getschedparam



subroutine forthread_attr_setschedparam(attr,param,info)
use forthread_types
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
type(sched_param), intent(in)   :: param
integer       , intent(out)     :: info

call thread_attr_setschedparam(attr,param,info)
end subroutine forthread_attr_setschedparam



subroutine forthread_attr_getschedpolicy(attr,policy,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer       , intent(out)     :: policy
integer       , intent(out)     :: info

call thread_attr_getschedpolicy(attr,policy,info)
end subroutine forthread_attr_getschedpolicy



subroutine forthread_attr_setschedpolicy(attr,policy,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer       , intent(in)      :: policy
integer       , intent(out)     :: info

call thread_attr_setschedpolicy(attr,policy,info)
end subroutine forthread_attr_setschedpolicy



subroutine forthread_attr_getscope(attr,scope,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer       , intent(out)     :: scope
integer       , intent(out)     :: info

call thread_attr_getscope(attr,scope,info)
end subroutine forthread_attr_getscope



subroutine forthread_attr_setscope(attr,scope,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer       , intent(in)      :: scope
integer       , intent(out)     :: info

call thread_attr_setscope(attr,scope,info)
end subroutine forthread_attr_setscope



subroutine forthread_attr_getstacksize(attr,stacksize,info)
use forthread_types
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer(size_t), intent(out)    :: stacksize
integer       , intent(out)     :: info

call thread_attr_getstacksize(attr,stacksize,info)
end subroutine forthread_attr_getstacksize



subroutine forthread_attr_setstacksize(attr,stacksize,info)
use forthread_types
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer(size_t), intent(in)     :: stacksize
integer       , intent(out)     :: info

call thread_attr_setstacksize(attr,stacksize,info)
end subroutine forthread_attr_setstacksize


!*****************************************!
!*       mutex attribute routines        *!
!*****************************************!


subroutine forthread_mutexattr_destroy(attr,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer       , intent(out)     :: info

call thread_mutexattr_destroy(attr,info)
end subroutine forthread_mutexattr_destroy



subroutine forthread_mutexattr_init(attr,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer       , intent(out)     :: info

call thread_mutexattr_init(attr,info)
end subroutine forthread_mutexattr_init



subroutine forthread_mutexattr_getpshared(attr,pshared,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer       , intent(out)     :: pshared
integer       , intent(out)     :: info

call thread_mutexattr_getpshared(attr,pshared,info)
end subroutine forthread_mutexattr_getpshared



subroutine forthread_mutexattr_setpshared(attr,pshared,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer       , intent(in)      :: pshared
integer       , intent(out)     :: info

call thread_mutexattr_setpshared(attr,pshared,info)
end subroutine forthread_mutexattr_setpshared



subroutine forthread_mutexattr_getprioceiling(attr,prioceiling,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer       , intent(out)     :: prioceiling
integer       , intent(out)     :: info

call thread_mutexattr_getprioceiling(attr,prioceiling,info)
end subroutine forthread_mutexattr_getprioceiling



subroutine forthread_mutexattr_setprioceiling(attr,prioceiling,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer       , intent(in)      :: prioceiling
integer       , intent(out)     :: info

call thread_mutexattr_setprioceiling(attr,prioceiling,info)
end subroutine forthread_mutexattr_setprioceiling



subroutine forthread_mutexattr_getprotocol(attr,protocol,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer       , intent(out)     :: protocol
integer       , intent(out)     :: info

call thread_mutexattr_getprotocol(attr,protocol,info)
end subroutine forthread_mutexattr_getprotocol



subroutine forthread_mutexattr_setprotocol(attr,protocol,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer       , intent(in)      :: protocol
integer       , intent(out)     :: info

call thread_mutexattr_setprotocol(attr,protocol,info)
end subroutine forthread_mutexattr_setprotocol



subroutine forthread_mutexattr_gettype(attr,mtype,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer       , intent(out)     :: mtype
integer       , intent(out)     :: info

call thread_mutexattr_gettype(attr,mtype,info)
end subroutine forthread_mutexattr_gettype



subroutine forthread_mutexattr_settype(attr,mtype,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer       , intent(in)      :: mtype
integer       , intent(out)     :: info

call thread_mutexattr_settype(attr,mtype,info)
end subroutine forthread_mutexattr_settype



!*****************************************************!
!*    condition attriubute variable routines         *!
!*****************************************************!


subroutine forthread_condattr_destroy(attr,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer       , intent(out)     :: info

call thread_condattr_destroy(attr,info)
end subroutine forthread_condattr_destroy



subroutine forthread_condattr_init(attr,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer       , intent(out)     :: info

call thread_condattr_init(attr,info)
end subroutine forthread_condattr_init



subroutine forthread_condattr_getpshared(attr,pshared,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer       , intent(out)     :: pshared
integer       , intent(out)     :: info

call thread_condattr_getpshared(attr,pshared,info)
end subroutine forthread_condattr_getpshared



subroutine forthread_condattr_setpshared(attr,pshared,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer       , intent(in)      :: pshared
integer       , intent(out)     :: info

call thread_condattr_setpshared(attr,pshared,info)
end subroutine forthread_condattr_setpshared



#ifndef __DARWIN
subroutine forthread_condattr_getclock(attr,clock_id,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer       , intent(out)     :: clock_id
integer       , intent(out)     :: info

call thread_condattr_getclock(attr,clock_id,info)
end subroutine forthread_condattr_getclock



subroutine forthread_condattr_setclock(attr,clock_id,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer       , intent(in)      :: clock_id
integer       , intent(out)     :: info

call thread_condattr_setclock(attr,clock_id,info)
end subroutine forthread_condattr_setclock


!**************************************************!
!*    barrier attribute variable routines         *!
!**************************************************!



subroutine forthread_barrierattr_destroy(attr,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer       , intent(out)     :: info

call thread_barrierattr_destroy(attr,info)
end subroutine forthread_barrierattr_destroy



subroutine forthread_barrierattr_init(attr,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer       , intent(out)     :: info

call thread_barrierattr_init(attr,info)
end subroutine forthread_barrierattr_init



subroutine forthread_barrierattr_getpshared(attr,pshared,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer       , intent(out)     :: pshared
integer       , intent(out)     :: info

call thread_barrierattr_getpshared(attr,pshared,info)
end subroutine forthread_barrierattr_getpshared



subroutine forthread_barrierattr_setpshared(attr,pshared,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer       , intent(in)      :: pshared
integer       , intent(out)     :: info

call thread_barrierattr_setpshared(attr,pshared,info)
end subroutine forthread_barrierattr_setpshared
#endif


!**************************************************!
!*    rwlock attribute variable routines         *!
!**************************************************!


subroutine forthread_rwlockattr_destroy(attr,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer       , intent(out)     :: info

call thread_rwlockattr_destroy(attr,info)
end subroutine forthread_rwlockattr_destroy



subroutine forthread_rwlockattr_init(attr,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer       , intent(out)     :: info

call thread_rwlockattr_init(attr,info)
end subroutine forthread_rwlockattr_init



subroutine forthread_rwlockattr_getpshared(attr,pshared,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer       , intent(out)     :: pshared
integer       , intent(out)     :: info

call thread_rwlockattr_getpshared(attr,pshared,info)
end subroutine forthread_rwlockattr_getpshared



subroutine forthread_rwlockattr_setpshared(attr,pshared,info)
implicit none

#include "ciface.h"
integer       , intent(in)      :: attr
integer       , intent(in)      :: pshared
integer       , intent(out)     :: info

call thread_rwlockattr_setpshared(attr,pshared,info)
end subroutine forthread_rwlockattr_setpshared

