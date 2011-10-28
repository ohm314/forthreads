
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
    function start_routine(arg) bind(c)
    use iso_c_binding
    type(c_ptr), value, intent(in)  :: arg
    type(c_ptr),        intent(out) :: start_routine
    end function start_routine
end interface

interface
    subroutine thread_create(thread_id,attr_id,start_routine,arg,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(out)      :: thread_id
    integer(c_int), intent(in)       :: attr_id
    type(c_funptr), intent(in)       :: start_routine
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
    type(c_ptr),        intent(out)  :: value_ptr
    end subroutine thread_exit
end interface

interface
    subroutine thread_join(thread_id,value_ptr,info) bind(c)
    use iso_c_binding
    integer(c_int), intent(in)   :: thread_id
    type(c_ptr),    intent(out)  :: value_ptr
    integer(c_int), intent(out)      :: info
    end subroutine thread_join
end interface
