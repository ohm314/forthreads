module procmod
contains
    subroutine example(arg)
    implicit none
    integer :: arg

    print *,'hello world',arg

    end subroutine example
end module procmod

program test01

use procmod

!include '../src/iface/ciface.h'

integer, parameter       :: n = 10
integer, dimension(n)    :: tid
integer, dimension(n)    :: arg
integer                  :: ret_val
integer                  :: thread_id
integer                  :: aid
integer                  :: i
integer                  :: info

aid = -1
print *, '====================== test02 ======================'
print *,'initializing threads'

call forthread_init(info)
if (info.ne.0) then
    print *,'error initializing '
endif

do i=1,n
  arg(i) = i**2
  call forthread_create(thread_id,aid,example,arg(i),info)
  tid(i) = thread_id
  if (info.ne.0) then
    print *,'error creating thread ',i
  else
    print *,'successfully created thread ',i
  endif
enddo

do i=1,n
  print *,'join thread',i
  call forthread_join(tid(i),ret_val,info)
  if (info.ne.0) then
    print *,'error joining thread ',i
  endif
enddo

call forthread_destroy(info)
if (info.ne.0) then
    print *,'error destroying.'
endif

print *, '==================== end test02 ===================='


end program test01
