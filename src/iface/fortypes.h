
type, bind(c) :: sched_param
    integer(c_int) :: sched_priority
end type sched_param
    
type, bind(c) :: timespec
    integer(c_int)  :: tv_sec  ! seconds
    integer(c_long) :: tv_nsec ! nanoseconds
end type timespec

integer, parameter :: size_t = c_size_t
