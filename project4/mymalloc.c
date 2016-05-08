// Author:    Steven Comer
// Email:     sfc15@pitt.edu
// Contains:  My Malloc Implementation File
// Part of:   Project 4
// Used in:   mymalloc.c, mallocdrv.c
// Updated:   8 April 2013

#include <stdio.h>
#include "mymalloc.h"

void * base;

int firstTime = 1;

void * my_worstfit_malloc(int size) 
{
	if(firstTime == 1) base = sbrk(0);
	firstTime = 0;
	
	int blocksize = size + 2*sizeof(int);
	if(blocksize % 2 != 0) blocksize++;

	int * currheader;
	int * bestheader;
	int currsize;
	int bestsize;
	int currstatus;

	currheader = base;
	bestheader = base;
	bestsize = 0;

        while((void *)currheader < sbrk(0)) {
                currsize = *currheader & 0xFFFFFFFE;
                currstatus = *currheader & 0x00000001;
                if(currstatus == 0 && currsize > bestsize && currsize >= blocksize) {
        	       	bestsize = currsize;
                	bestheader = currheader;
                }
                currheader = (int *)((char *)currheader + currsize);
        }

	int * blockheader;
	int * blockfooter;
	int xsize;

	if(bestsize == 0) {
		xsize = 0;
		blockheader = sbrk(0);
		sbrk(blocksize);
	}
	else {
		xsize = bestsize - blocksize;
		blockheader = (int *)((char *)bestheader + xsize);
	}

	blockfooter = (int *)((char *)blockheader + (blocksize - sizeof(int)));
	*blockheader = blocksize | 0x00000001;
	*blockfooter = *blockheader;

	if(xsize != 0) {
		int * xheader;
	        int * xfooter;
		xheader = bestheader;
		xfooter = (int *)((char *)xheader + xsize);
		*xheader = xsize | 0xFFFFFFFE;
		*xfooter = *xheader;
	}
	
	return blockheader + 1;
}

void my_free(void * ptr)
{

	if(ptr == NULL) return;	
	
	int * blockheader;
	int * blockfooter;
	int blockstatus;
	int blocksize;

	blockheader = ((int *)ptr) - 1;
	blockstatus = *blockheader & 0x00000001;
	blocksize = *blockheader & 0xFFFFFFFE;

	if(blockstatus == 0) return;

	if((void *)blockheader > base) {
		int * prevheader;
	        int * prevfooter;
        	int prevstatus;
        	int prevsize;	
		prevfooter = (int *)((char *)blockheader - sizeof(int));
		prevstatus = *prevfooter & 0x00000001;
		prevsize = *prevfooter & 0xFFFFFFFE;
		prevheader = (int *)((char *)prevfooter - (prevsize - sizeof(int)));
		if(prevstatus == 0) {
			blockheader = prevheader;
			blocksize += prevsize;
		}
	}

	if((void *)(blockheader + blocksize) < sbrk(0)) {
	        int * nextheader;
        	int * nextfooter;
        	int nextstatus;
        	int nextsize;
		nextheader = (int *)((char *)blockheader + blocksize);
		nextstatus = *nextheader & 0x00000001;
		nextsize = *nextheader & 0xFFFFFFFE;
		nextfooter = (int *)((char *)nextheader + (nextsize - sizeof(int)));
		if(nextstatus == 0) {
			blocksize += nextsize;
		}
	}

	if((void *)((char *)blockheader + blocksize) == sbrk(0)) {
		sbrk(-blocksize);
		return;
	}

	blockfooter = (int *)((char *)blockheader + (blocksize - sizeof(int)));
	*blockheader = blocksize & 0xFFFFFFFE;
	*blockfooter = *blockheader;
}

void heapWalk(char * caption)
{
        if (base == sbrk(0))                                    // base == top means no blocks exist on the HEAP
        {
                printf("The size of the heap is 0. There are no allocated blocks in the heap.\n\n");
                return;
        }

        int blockNum=0;                                         // counter for which block we are one  1,2,3,4,5 etc.
        char * blockstart = base;                               // mem addr of first byte in heap
        int baseoffset = 0;                                     // since we are at the very base of HEAP, the bytes offest is 0

        printf("    HEAPBASE: %d\n",(int)base );
        printf("BLOCK# STATUS STARTADDR\tBASE_OFFS PAYLOAD    HDR/FTR\tBLOCKSIZE\n");
        do
        {
                blockNum++;
                int blockheader = *(int *)blockstart;
                int status = blockheader & 0x00000001;
                int blocksize = blockheader & 0xFFFFFFFE;
                int payload = blocksize - 8;
                if(status == 1) printf("%5d  USED   %9u %-9d %d bytes    8 bytes       %d\n", blockNum, (int)blockstart, baseoffset, payload, blocksize);
                else printf("%5d  FREE   %9u %-9d %d bytes    8 bytes       %d\n", blockNum, (int)blockstart, baseoffset, payload, blocksize);

                blockstart += blocksize;
                baseoffset += blocksize;

        } while ( (void *)blockstart < sbrk(0) );  // While we have not reached TOP of the HEAP

        printf("    HEAP TOP: %d\n\n",(int)sbrk(0) );
}
