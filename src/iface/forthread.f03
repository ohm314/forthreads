

subroutine forthread_init(info)
implicit none

include 'ciface.h'

integer, intent(out) :: info

call thread_init(info)

end subroutine


subroutine forthread_destroy(info)
implicit none

include 'ciface.h'

integer, intent(out) :: info

call thread_destroy(info)
end subroutine


subroutine forthread_create(thread_id,attr_id,start_routine,arg,info)

use iso_c_binding
implicit none

include 'ciface.h'

integer, intent(out)                :: thread_id
integer, intent(in)                 :: attr_id
procedure(i_run)                    :: start_routine
integer                   , target  :: arg
integer, intent(out)                :: info

type(t_run), dimension(:), pointer,save :: routine_table => NULL()
procedure(i_start_routine), bind(c), pointer :: runp

if (.not.associated(routine_table)) then
    allocate(routine_table(100))
endif

call thread_alloc(thread_id,info)
routine_table(thread_id)%run => start_routine
routine_table(thread_id)%runarg => arg
runp => run

call thread_create(thread_id,attr_id,c_funloc(runp),&
                   c_loc(routine_table(thread_id)),info)
contains

    type(c_ptr) function run(arg) bind(c)
    use iso_c_binding
    implicit none

    include 'ciface.h'

    type(c_ptr), value, intent(in)  :: arg

    type(t_run), pointer :: exec
    integer      :: ret

    call c_f_pointer(arg,exec)
    call exec%run(exec%runarg)

    end function run

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

call thread_join(thread_id,value_ptr,info)
call c_f_pointer(value_ptr,val)
end subroutine
