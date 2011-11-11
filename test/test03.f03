module procmod

integer :: is_initialized = 0

integer :: somevar

contains

    subroutine initializer()

    implicit none

    somevar = 42
    print *,'initialized',somevar

    end subroutine initializer

    subroutine example(arg)
    implicit none
    integer :: arg
    integer :: info

    print *, 'hello world from',arg

    call forthread_once(is_initialized,initializer,info)

    end subroutine example
end module procmod

program test03

use procmod

!include '../src/iface/ciface.h'

integer, parameter       :: n = 3
integer, dimension(n)    :: tid
integer, dimension(n)    :: arg
integer                  :: ret_val
integer                  :: thread_id
integer                  :: aid
integer                  :: i
integer                  :: info

aid = -1
print *, '====================== test03 ======================'
print *,'initializing threads'
call forthread_init(info)
if (info.ne.0) then
    print *,'error initializing '
endif

call forthread_once_init(is_initialized,info)
print *,is_initialized

do i=1,n
  arg(i) = i
  call forthread_create(thread_id,aid,example,arg(i),info)
  tid(i) = thread_id
  if (info.ne.0) then
    print *,'error creating thread ',i
  endif
enddo

do i=1,n
  print *,'join thread',tid(i)
  call forthread_join(tid(i),ret_val,info)
  if (info.ne.0) then
    print *,'error joining thread ',i
  endif
enddo

call forthread_destroy(info)
if (info.ne.0) then
    print *,'error destroying.'
endif

print *,'somevar:',somevar

print *, '==================== end test03 ===================='


end program test03
