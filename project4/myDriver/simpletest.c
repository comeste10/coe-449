// Author:    Steven Comer
// Email:     sfc15@pitt.edu
// Contains:  My Malloc Test Program
// Part of:   Project 4
// Updated:   30 March 2013

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void * sbrk(int increment);
void * base;
void heapcheck();

#include "mymalloc.h"

#define malloc(x) my_worstfit_malloc(x)
#define free(x) my_free(x)

int main ()
{
	int * w = malloc(sizeof(int));
	int * x = malloc(sizeof(int));
	int * y = malloc(sizeof(int));
	int * z = malloc(sizeof(int));

	printf("\n---------TEST PROGRAM---------\n");
	printf("Four ints have been malloced.\n\n");

	heapWalk("");	

	printf("They will now be freed.\n\n");

	free(w);
	heapWalk("");

	free(x);
	heapWalk("");

	free(y);
	heapWalk("");

	free(z);
	heapWalk("");

	return 0;
}

void heapcheck() {
	printf("sbrk(0) = %X\n", (int)sbrk(0));
	printf("base    = %X\n", (int)base);
	printf("diff    = %d\n\n",(int)(sbrk(0)-base));
}
