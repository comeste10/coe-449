// Author:    Steven Comer
// Email:     sfc15@pitt.edu
// Contains:  My Malloc Header File
// Part of:   Project 4
// Used in:   mymalloc.c, mallocdrv.c
// Updated:   5 April 2013

#ifndef MYMALLOC_H
#define MYMALLOC_H

// prototypes
void * my_worstfit_malloc (int size);
void my_free (void *ptr);
void heapWalk(char * caption);
void * sbrk( int increment );

#endif
