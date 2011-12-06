module procmod


integer :: counter
integer :: counter_mutex


contains

    subroutine example(arg)
    implicit none
    integer :: arg
    integer :: info

    integer :: i
    integer :: lcnt

    do i=1,100
        call forthread_mutex_lock(counter_mutex,info)
        lcnt = counter
        lcnt = lcnt + 1
        counter = lcnt
        call forthread_mutex_unlock(counter_mutex,info)
    end do
    end subroutine example
end module procmod

program test04

use procmod

!include '../src/iface/ciface.h'

integer, parameter       :: n = 100
integer, dimension(n)    :: tid
integer                  :: arg
integer                  :: ret_val
integer                  :: thread_id
integer                  :: aid
integer                  :: i
integer                  :: info


aid = -1
arg = 0
print *, '====================== test04 ======================'
print *,'initializing threads'
call forthread_init(info)
if (info.ne.0) then
    print *,'error initializing '
endif

call forthread_mutex_init(counter_mutex,aid,info)

do i=1,n
  call forthread_create(thread_id,aid,example,arg,info)
  tid(i) = thread_id
  if (info.ne.0) then
    print *,'error creating thread ',i
  endif
enddo

do i=1,n
  call forthread_join(tid(i),ret_val,info)
  if (info.ne.0) then
    print *,'error joining thread ',i
  endif
enddo

call forthread_mutex_destroy(counter_mutex,info)
call forthread_destroy(info)
if (info.ne.0) then
    print *,'error destroying.'
endif

print *,'counter (10000):',counter

print *, '==================== end test04 ===================='


end program test04
