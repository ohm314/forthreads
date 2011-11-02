program test01

include '../src/iface/ciface.h'

integer, parameter       :: n = 10
integer, dimension(n)    :: tid
integer                  :: arg
integer                  :: ret_val
integer                  :: thread_id
integer                  :: aid
integer                  :: i
integer                  :: info

procedure(i_start_routine), bind(c), pointer :: runp

runp => run

aid = -1

print *, '====================== test02 ======================'
print *,'initializing threads'

call forthread_init(info)
if (info.ne.0) then
    print *,'error initializing '
endif

do i=1,n
  arg = i**2
  call forthread_create(thread_id,aid,runp,arg,info)
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

contains


    type(c_ptr) function run(arg) bind(c)
    use iso_c_binding
    type(c_ptr), value, intent(in)  :: arg

    integer, pointer :: val

    call c_f_pointer(arg,val)
    print *,'hello world',val

    end function run

end program test01
