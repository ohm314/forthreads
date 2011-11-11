

subroutine forthread_init(info)

use forthread_data
implicit none

include 'ciface.h'

integer, intent(out) :: info

allocate(routine_table(init_size))
routine_table_size = init_size

call thread_init(info)

call thread_mutex_init(routine_table_mutex,-1,info)

end subroutine


subroutine forthread_destroy(info)
implicit none

include 'ciface.h'

integer, intent(out) :: info

call thread_destroy(info)
end subroutine


subroutine forthread_create(thread_id,attr_id,run,arg,info)

use iso_c_binding
use forthread_data
implicit none

include 'ciface.h'

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


end subroutine
    
subroutine forthread_detach(thread_id,info)
implicit none

include 'ciface.h'

integer, intent(in)             :: thread_id
integer, intent(out)            :: info

call thread_detach(thread_id,info)
end subroutine

subroutine forthread_equal(t1,t2,info)
implicit none

include 'ciface.h'

integer, intent(in)             :: t1
integer, intent(in)             :: t2
integer, intent(out)            :: info

call thread_equal(t1,t2,info)
end subroutine

subroutine forthread_exit(val)

use iso_c_binding
implicit none

include 'ciface.h'


integer,            pointer :: val

type(c_ptr)                 :: value_ptr

! FIXME pointer mess
call thread_exit(value_ptr)
call c_f_pointer(value_ptr,val)
end subroutine

subroutine forthread_join(thread_id,val,info)

use iso_c_binding
implicit none

include 'ciface.h'


integer, intent(in)         :: thread_id
integer,             pointer:: val
integer, intent(out)        :: info

type(c_ptr)                 :: value_ptr
! FIXME pointer mess
call thread_join(thread_id,value_ptr,info)
call c_f_pointer(value_ptr,val)
end subroutine

subroutine forthread_cancel(thread_id,info)
implicit none

include 'ciface.h'

integer, intent(in)         :: thread_id
integer, intent(out)        :: info

call thread_cancel(thread_id,info)
end subroutine

subroutine forthread_kill(thread_id,sig,info)
implicit none

include 'ciface.h'

integer, intent(in)         :: thread_id
integer, intent(in)         :: sig
integer, intent(out)        :: info

call thread_kill(thread_id,sig,info)
end subroutine

subroutine forthread_once_init(once_ctrl_id,info)
use iso_c_binding
implicit none

integer, intent(out) :: once_ctrl_id
integer, intent(out) :: info

include 'ciface.h'

call thread_once_init(once_ctrl_id,info)
end subroutine

subroutine forthread_once(once_ctrl_id,init_routine,info)
use iso_c_binding
use forthread_data
implicit none

include 'ciface.h'

integer, intent(in)                 :: once_ctrl_id
procedure(i_once), bind(c)           :: init_routine
! dangerous but works! (gfortran)
! TODO test in other compilers
integer, intent(out)                :: info


call thread_once(once_ctrl_id,c_funloc(init_routine),info)

end subroutine

! TODO implement thread_atfork

! TODO implemented thread_cleanup_pop and thread_cleanup_push

subroutine forthread_getconcurrency(currlevel,info)
implicit none

include 'ciface.h'

integer       , intent(out)     :: currlevel
integer       , intent(out)     :: info

call thread_getconcurrency(currlevel,info)

end subroutine forthread_getconcurrency

subroutine forthread_setconcurrency(newlevel,info)
implicit none

include 'ciface.h'

integer       , intent(in)      :: newlevel
integer       , intent(out)     :: info

call thread_setconcurrency(newlevel,info)

end subroutine forthread_setconcurrency




