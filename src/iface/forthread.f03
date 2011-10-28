

subroutine forthread_init(info)
implicit none

include 'ciface.h'

integer, intent(out) :: info

call thread_init(info)
end subroutine

