Author:   Steven Comer
Email:    sfc15@pitt.edu
Date:     8 April 2013

Contents:

  mallocdrv.c
  mymalloc.c
  mymalloc.h
  myDriver
    simpletest.c

Notes:

  This project contains my implementations of worst-fit malloc and free.
  The file simpletest.c contains my test program which allocates multiple blocks
  and then frees them in a way that demonstrates the coalescing of my free
  implementation and its ability to shrink the heap back down.

Compile with:

  gcc -Wall -m32 -std=c99 *.c

Run with:

  ./a.out