forthreads
==========

Fortran 2003 wrappers for POSIX threads

_Copyright (c) 2012-2017 MOSAIC Group (ETH Zurich & MPI-CBG Dresden), Omar Awile_

This project and all files pertaining to it are licensed under the BSD 3-Clause 
License. Please see the enclosed LICENSE file for the terms.

In order to ensure financial support for our project and allow further
development of this software, please cite the following publication in all your
documents and manuscripts that made use of this software. Thanks a lot!

O. Awile and I. F. Sbalzarini. A pthreads wrapper for Fortran 2003. ACM Trans.
Math. Softw., 40(3):19:1–19:15, 2014.

Introduction
------------

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


