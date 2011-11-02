program test01

integer, parameter       :: n = 10
integer, dimension(n)    :: tid
integer                  :: arg
integer                  :: thread_id
integer                  :: aid
integer                  :: i
integer                  :: info


aid = -1

print *,'initializing threads'

call forthread_init(info)
if (info.ne.0) then
    print *,'error initializing '
endif

do i=1,n
  arg = i**2
  call forthread_create(thread_id,aid,run,arg,info)
  tid(i) = thread_id
  if (info.ne.0) then
    print *,'error creating thread ',i
  else
    print *,'successfully created thread ',i
  endif
enddo

do i=1,n
  print *,'join thread',i
  call forthread_join(tid(i),0,info)
  if (info.ne.0) then
    print *,'error joining thread ',i
  endif
enddo

call forthread_destroy(info)
if (info.ne.0) then
    print *,'error destroying.'
endif

contains


    type(c_ptr) function run(arg)
    use iso_c_binding
    type(c_ptr), value, intent(in)  :: arg
    
    print *,'hello world'

    end function run

end program test01
