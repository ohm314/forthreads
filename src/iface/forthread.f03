

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

integer, intent(inout)              :: thread_id
integer, intent(in)                 :: attr_id
procedure(i_start_routine), pointer :: start_routine
integer                   , target  :: arg
integer, intent(out)                :: info

call thread_create(thread_id,attr_id,c_funloc(start_routine),c_loc(arg),info)
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
