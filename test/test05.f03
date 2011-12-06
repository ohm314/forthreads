! testing condition variables and signaling single threads

module procmod

integer, parameter :: maxcount = 100
integer :: counter
integer :: counter_print_cv
integer :: counter_mutex
logical :: printed
integer :: printed_cv

contains

    subroutine countert(arg)
    implicit none
    integer :: arg
    integer :: info

    do while(.true.)
        call forthread_mutex_lock(counter_mutex,info)
        if (counter.gt.maxcount) then
            call forthread_cond_signal(counter_print_cv,info)
            call forthread_mutex_unlock(counter_mutex,info)
            return
        endif
        counter = counter + 1
        if (printed) printed = .false.
        if (mod(counter,10).eq.0) then
            call forthread_cond_signal(counter_print_cv,info)
            do while (.not.printed)
                call forthread_cond_wait(printed_cv,counter_mutex,info)
            enddo
        endif

        call forthread_mutex_unlock(counter_mutex,info)
    enddo
    end subroutine countert
    
    subroutine printer(arg)
    implicit none
    integer :: arg
    integer :: info


    do while(.true.)
        call forthread_mutex_lock(counter_mutex,info)
        do while (.not.(mod(counter,10).eq.0).and.(counter.le.maxcount))
            call forthread_cond_wait(counter_print_cv,counter_mutex,info)
        end do
        if ((.not.printed).and.(mod(counter,10).eq.0)) then
            print *,'counter is:',counter
            printed = .true.
        endif
        if (counter.gt.maxcount) then
            call forthread_mutex_unlock(counter_mutex,info)
            return
        endif
        call forthread_cond_signal(printed_cv,info)
        
        call forthread_mutex_unlock(counter_mutex,info)
    enddo

    end subroutine printer

end module procmod

program test05

use procmod

!include '../src/iface/ciface.h'

integer, parameter       :: n = 2
integer, dimension(n)    :: tid
integer                  :: arg
integer                  :: ret_val
integer                  :: thread_id
integer                  :: aid
integer                  :: i
integer                  :: info

counter = 1
aid = -1
arg = 0
printed = .false.
print *, '====================== test05 ======================'
print *,'initializing threads'
call forthread_init(info)
if (info.ne.0) then
    print *,'error initializing '
endif

call forthread_mutex_init(counter_mutex,aid,info)
call forthread_cond_init(counter_print_cv,aid,info)
call forthread_cond_init(printed_cv,aid,info)

call forthread_create(thread_id,aid,printer,arg,info)
tid(1) = thread_id
do i=2,n
  call forthread_create(thread_id,aid,countert,arg,info)
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
call forthread_cond_destroy(counter_print_cv,info)
call forthread_destroy(info)
if (info.ne.0) then
    print *,'error destroying.'
endif

print *,'counter (101):',counter

print *, '==================== end test05 ===================='


end program test05
