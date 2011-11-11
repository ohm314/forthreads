



! pthread interfaces for fortran

interface
    subroutine thread_init(info) bind(c)
    use iso_c_binding
    integer(c_int), intent(out)      :: info
    end subroutine thread_init
end interface


interface
    subroutine thread_destroy(info) bind(c)
    use iso_c_binding
    integer(c_int), intent(out)      :: info
    end subroutine thread_destroy
end interface

abstract interface
    function i_start_routine(arg) bind(c)
    use iso_c_binding
    type(c_ptr)                     :: i_start_routine
    type(c_ptr), value, intent(in)  :: arg
    end function i_start_routine
end interface



interface
    subroutine thread_alloc(thread_id,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(out)      :: thread_id
    integer(c_int), intent(out)      :: info
    end subroutine thread_alloc
end interface

interface
    subroutine thread_create(thread_id,attr_id,start_routine,arg,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(in)      :: thread_id
    integer(c_int), intent(in)       :: attr_id
    type(c_funptr), intent(in)       :: start_routine
    type(c_ptr), value, intent(in)   :: arg
    integer(c_int), intent(out)      :: info
    end subroutine thread_create
end interface

interface
    subroutine thread_detach(thread_id,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(in)       :: thread_id
    integer(c_int), intent(out)      :: info
    end subroutine thread_detach
end interface

interface 
    subroutine thread_equal(t1,t2,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(in)       :: t1
    integer(c_int), intent(in)       :: t2
    integer(c_int), intent(out)      :: info
    end subroutine thread_equal
end interface

interface
    subroutine thread_exit(value_ptr) bind(c)
    use iso_c_binding
    type(c_ptr),      intent(out)    :: value_ptr
    end subroutine thread_exit
end interface

interface
    subroutine thread_join(thread_id,value_ptr,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(in)      :: thread_id
    type(c_ptr),    intent(out)     :: value_ptr
    integer(c_int), intent(out)     :: info
    end subroutine thread_join
end interface

interface
    subroutine thread_cancel(thread_id,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(in)      :: thread_id
    integer(c_int), intent(out)     :: info
    end subroutine thread_cancel
end interface

interface
    subroutine thread_kill(thread_id,sig,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(in)      :: thread_id
    integer(c_int), intent(in)      :: sig
    integer(c_int), intent(out)     :: info
    end subroutine thread_kill
end interface

interface
    subroutine thread_once_init(once_ctrl,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(out)   :: once_ctrl
    integer(c_int), intent(out)   :: info
    end subroutine thread_once_init
end interface

interface
    subroutine thread_once(once_ctrl_id,routine,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(in)       :: once_ctrl_id
    type(c_funptr), intent(in)       :: routine
    integer(c_int), intent(out)      :: info
    end subroutine thread_once
end interface

interface
    subroutine thread_atfork(prepare,parent,child,info) bind(c)
    use iso_c_binding
    type(c_funptr), intent(in)       :: prepare
    type(c_funptr), intent(in)       :: parent
    type(c_funptr), intent(in)       :: child
    integer(c_int), intent(out)      :: info
    end subroutine thread_atfork
end interface

! TODO implemented thread_cleanup_pop and thread_cleanup_push

interface
    subroutine thread_getconcurrency(currlevel,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(out)     :: currlevel
    integer(c_int), intent(out)     :: info
    end subroutine thread_getconcurrency
end interface

interface
    subroutine thread_setconcurrency(newlevel,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(in)      :: newlevel
    integer(c_int), intent(out)     :: info
    end subroutine thread_setconcurrency
end interface

interface
    subroutine thread_getcpuclockid(thread,clock_id,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(in)      :: thread
    integer(c_int), intent(out)     :: clock_id
    integer(c_int), intent(out)     :: info
    end subroutine thread_getcpuclockid
end interface


interface
    subroutine thread_getschedparam(thread,policy,param,info) bind(c)
    use iso_c_binding
    use forthread_types
    integer(c_int), intent(in)      :: thread
    integer(c_int), intent(out)     :: policy
    type(sched_param), intent(out)  :: param
    integer(c_int), intent(out)     :: info
    end subroutine thread_getschedparam
end interface

interface
    subroutine thread_setschedparam(thread,policy,param,info) bind(c)
    use iso_c_binding
    use forthread_types
    integer(c_int), intent(in)      :: thread
    integer(c_int), intent(in)      :: policy
    type(sched_param), intent(in)   :: param
    integer(c_int), intent(out)     :: info
    end subroutine thread_setschedparam
end interface

interface
    subroutine thread_setschedprio(thread,prio,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(in)      :: thread
    integer(c_int), intent(in)      :: prio
    integer(c_int), intent(out)     :: info
    end subroutine thread_setschedprio
end interface

interface
    subroutine thread_setcancelstate(state,oldstate,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(in)      :: state
    integer(c_int), intent(in)      :: oldstate
    integer(c_int), intent(out)     :: info
    end subroutine thread_setcancelstate
end interface

interface
    subroutine thread_setcanceltype(ctype,oldctype,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(in)      :: ctype
    integer(c_int), intent(in)      :: oldctype
    integer(c_int), intent(out)     :: info
    end subroutine thread_setcanceltype
end interface

!*****************************************!
!*   sharing private data in threads     *!
!*****************************************!

! TODO

!*****************************************!
!*             mutex routines            *!
!*****************************************!

interface
    subroutine thread_mutex_destroy(mutex_id,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(in)      :: mutex_id
    integer(c_int), intent(out)     :: info
    end subroutine thread_mutex_destroy
end interface

interface
    subroutine thread_mutex_init(mutex_id,attr_id,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(out)     :: mutex_id
    integer(c_int), intent(in)      :: attr_id
    integer(c_int), intent(out)     :: info
    end subroutine thread_mutex_init
end interface

interface
    subroutine thread_mutex_lock(mutex_id,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(in)      :: mutex_id
    integer(c_int), intent(out)     :: info
    end subroutine thread_mutex_lock
end interface

interface
    subroutine thread_mutex_trylock(mutex_id,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(in)      :: mutex_id
    integer(c_int), intent(out)     :: info
    end subroutine thread_mutex_trylock
end interface

interface
    subroutine thread_mutex_unlock(mutex_id,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(in)      :: mutex_id
    integer(c_int), intent(out)     :: info
    end subroutine thread_mutex_unlock
end interface

interface
    subroutine thread_mutex_getprioceiling(mutex,prioceiling,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(in)      :: mutex
    integer(c_int), intent(out)     :: prioceiling
    integer(c_int), intent(out)     :: info
    end subroutine thread_mutex_getprioceiling
end interface

interface
    subroutine thread_mutex_setprioceiling(mutex,prioceiling,old_ceiling,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(in)      :: mutex
    integer(c_int), intent(in)      :: prioceiling
    integer(c_int), intent(out)     :: old_ceiling
    integer(c_int), intent(out)     :: info
    end subroutine thread_mutex_setprioceiling
end interface

interface
    subroutine thread_mutex_timedlock(mutex,abs_timeout,info) bind(c)
    use iso_c_binding
    use forthread_types
    integer(c_int), intent(in)      :: mutex
    type(timespec), intent(in)      :: abs_timeout
    integer(c_int), intent(out)     :: info
    end subroutine thread_mutex_timedlock
end interface

!*****************************************!
!*    condition variable routines        *!
!*****************************************!

interface
    subroutine thread_cond_destroy(cond_id,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(in)      :: cond_id
    integer(c_int), intent(out)     :: info
    end subroutine thread_cond_destroy
end interface

interface
    subroutine thread_cond_init(cond_id,attr_id,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(out)     :: cond_id
    integer(c_int), intent(in)      :: attr_id
    integer(c_int), intent(out)     :: info
    end subroutine thread_cond_init
end interface

interface
    subroutine thread_cond_timedwait(mutex,abstime,info) bind(c)
    use iso_c_binding
    type, bind(c) :: timespec
        integer(c_int)  :: tv_sec  ! seconds
        integer(c_long) :: tv_nsec ! nanoseconds
    end type timespec
    integer(c_int), intent(in)      :: mutex
    type(timespec), intent(in)      :: abstime
    integer(c_int), intent(out)     :: info
    end subroutine thread_cond_timedwait
end interface

interface
    subroutine thread_cond_wait(cond_id,mutex_id,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(in)      :: cond_id
    integer(c_int), intent(in)      :: mutex_id
    integer(c_int), intent(out)     :: info
    end subroutine thread_cond_wait
end interface

interface
    subroutine thread_cond_broadcast(cond_id,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(in)      :: cond_id
    integer(c_int), intent(out)     :: info
    end subroutine thread_cond_broadcast
end interface

interface
    subroutine thread_cond_signal(cond_id,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(in)      :: cond_id
    integer(c_int), intent(out)     :: info
    end subroutine thread_cond_signal
end interface

!****************************************!
!*    barrier variable routines         *!
!****************************************!


interface
    subroutine thread_barrier_destroy(barrier_id,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(in)      :: barrier_id
    integer(c_int), intent(out)     :: info
    end subroutine thread_barrier_destroy
end interface

interface
    subroutine thread_barrier_init(barrier_id,attr_id,tcount,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(out)     :: barrier_id
    integer(c_int), intent(in)      :: attr_id
    integer(c_int), intent(in)      :: tcount
    integer(c_int), intent(out)     :: info
    end subroutine thread_barrier_init
end interface

interface
    subroutine thread_barrier_wait(barrier_id,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(in)      :: barrier_id
    integer(c_int), intent(out)     :: info
    end subroutine thread_barrier_wait
end interface

!*************************************!
!*    spin variable routines         *!
!*************************************!

interface
    subroutine thread_spin_destroy(spinlock_id,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(in)      :: spinlock_id
    integer(c_int), intent(out)     :: info
    end subroutine thread_spin_destroy
end interface

interface
    subroutine thread_spin_init(spinlock_id,pshared,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(out)     :: spinlock_id
    integer(c_int), intent(in)      :: pshared
    integer(c_int), intent(out)     :: info
    end subroutine thread_spin_init
end interface

interface
    subroutine thread_spin_lock(lock_id,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(in)      :: lock_id
    integer(c_int), intent(out)     :: info
    end subroutine thread_spin_lock
end interface

interface
    subroutine thread_spin_trylock(lock_id,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(in)      :: lock_id
    integer(c_int), intent(out)     :: info
    end subroutine thread_spin_trylock
end interface

interface
    subroutine thread_spin_unlock(lock_id,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(in)      :: lock_id
    integer(c_int), intent(out)     :: info
    end subroutine thread_spin_unlock
end interface

!*************************************!
!*    rwlock variable routines       *!
!*************************************!

interface
    subroutine thread_rwlock_destroy(rwlock_id,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(in)      :: rwlock_id
    integer(c_int), intent(out)     :: info
    end subroutine thread_rwlock_destroy
end interface

interface
    subroutine thread_rwlock_init(rwlock_id,attr_id,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(out)     :: rwlock_id
    integer(c_int), intent(in)      :: attr_id
    integer(c_int), intent(out)     :: info
    end subroutine thread_rwlock_init
end interface

interface
    subroutine thread_rwlock_rdlock(lock_id,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(in)      :: lock_id
    integer(c_int), intent(out)     :: info
    end subroutine thread_rwlock_rdlock
end interface

interface
    subroutine thread_rwlock_tryrdlock(lock_id,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(in)      :: lock_id
    integer(c_int), intent(out)     :: info
    end subroutine thread_rwlock_tryrdlock
end interface

interface
    subroutine thread_rwlock_wrlock(lock_id,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(in)      :: lock_id
    integer(c_int), intent(out)     :: info
    end subroutine thread_rwlock_wrlock
end interface

interface
    subroutine thread_rwlock_trywrlock(lock_id,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(in)      :: lock_id
    integer(c_int), intent(out)     :: info
    end subroutine thread_rwlock_trywrlock
end interface

interface
    subroutine thread_rwlock_unlock(lock_id,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(in)      :: lock_id
    integer(c_int), intent(out)     :: info
    end subroutine thread_rwlock_unlock
end interface

interface
    subroutine thread_rwlock_timedrdlock(lock_id,abs_timeout,info) bind(c)
    use iso_c_binding
    type, bind(c) :: timespec
        integer(c_int)  :: tv_sec  ! seconds
        integer(c_long) :: tv_nsec ! nanoseconds
    end type timespec
    integer(c_int), intent(in)      :: lock_id
    type(timespec), intent(in)      :: abs_timeout
    integer(c_int), intent(out)     :: info
    end subroutine thread_rwlock_timedrdlock
end interface

interface
    subroutine thread_rwlock_timedwrlock(lock_id,abs_timeout,info) bind(c)
    use iso_c_binding
    type, bind(c) :: timespec
        integer(c_int)  :: tv_sec  ! seconds
        integer(c_long) :: tv_nsec ! nanoseconds
    end type timespec
    integer(c_int), intent(in)      :: lock_id
    type(timespec), intent(in)      :: abs_timeout
    integer(c_int), intent(out)     :: info
    end subroutine thread_rwlock_timedwrlock
end interface







