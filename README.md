forthreads
==========

Fortran 2003 wrappers for POSIX threads

Author: Omar Awile
Date: 5.12.2012

Copyright (c) 2012 MOSAIC Group (ETH Zurich & MPI-CBG Dresden)


This library provides Fortran 2003 interfaces to almost all POSIX threads
functions. Following functions are not available in fortrheads:

— pthread\_cleanup\_push and pthread\_cleanup\_pop: These functions allow the
programmer to register callback functions into a calling thread’s cancellation
cleanup stack that will be popped and executed in order whenever the thread
exits, is cancelled, or calls pthread\_cleanup\_pop itself. These functions cannot
be wrapped, as push and pop must be called in pairs in the same scope. Hence,
the POSIX standard foresees their implementation to be done using C macros.
— Pthread thread-specific data management routines (pthread\_key\_\* and
pthread\_getspecific / pthread\_setspecific): These routines heavily rely on the C
programming language’s void pointers. Unfortunately such pointers are not
available in Fortran. Therefore, it seems difficult to provide portable and safe
wrappers to these functions.

Also, thread start routines can only take an integer and return an integer for
the same "void-pointer" limitation.
Maybe in the future a mechanism can be added to allow for more generic argument
handling.
