/*
  project-1.c

  MY NAME: Steven Comer
  MY PITT EMAIL: sfc15@pitt.edu

  As you develop and test this file:

  use this command to compile: (you can name the executable whatever you like)
  gcc -W -Wall -Wextra -O2   project-1.c   -o project-1.exe

  use this command to execute:  (you will of course test on both input files)

  ./project-1.exe sine-1.txt  sine-1.bin # you can name the binary file anything you like
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( int argc, char *argv[])
{
  FILE * inFile= NULL;

  // DECLARE ALL ADDITIONAL NEEDED VARIABLES
  short prev = 0;
  short current = 0;
  int totalCount = 0;
  int tempCount = 0;
  int crossedZero = 0;
  int waveCount = 0;
  short max = 0;
  short min = 0;

  /* recommend declaring two short int i.e prev & current so that you can compare
     them to look for zero crossings or changes in direction and sign.
     You will also need some int counters etc.
  */

  if (argc < 2)
    {
      fprintf(stderr,"\nMust specify a binary file on the command line. Please try again.\n");
      exit(EXIT_FAILURE);
    }
  if  ((inFile = fopen(argv[1],"rb")) == NULL)
    {
      fprintf(stderr,"Can't open %s for input\n", argv[1] );
      exit(EXIT_FAILURE);
    }

  // YOUR CODE HERE - READ EACH VALUE FROM THE BINARY FILE ONE AT A TIME AND LOOK FOR ZERO CROSSINGS TO DETECT WAVES

  /* recommended strategy:
     read in the first value of the file (prev) before the loop starts.
     Then in the while loop read next short in.
     In the loop you are to be looking for critical events:  zero crossings or sign changes.
     Before you start writing that code start out just echoing each value and the sample # associated with it.
     Once that is right start printing an alert whenever zero is touched/crossed OR or the direction changes.
     Zero crossings and direction changes are the critical events you must correctly detect.
     Once that is right then try to detect the start of the first wave.
     Once that is right add code to  detect the end of the first wave.
     Then start coding to detet every wave - counting samples per wave and
     keeping track of the highest and lowest value in that wave.
  */
  
  // get first sample
  fread(&prev, sizeof(short), 1, inFile);
  
  // count first sample
  tempCount++;
  totalCount++;
  
  while(fread(&current, sizeof(short), 1, inFile) == 1) {
	// update max and min
	if(current > max) max = current;
	if(current < min) min = current;

	// update counters
	tempCount++;
	totalCount++;

	// check for sign change or zero crossing
	if(prev*current < 0 || current == 0) {
		crossedZero++;
	}

	// special case for first wave
	if(waveCount == 0 && prev < 0 && current >= 0) {
		crossedZero = 2;	
	}

	// check for new wave
	if(crossedZero == 2 && current >= 0) {
		if(waveCount > 0 && current > 0) {
			printf("%d\t%hi\t%d\t%hi\t%hi\n",totalCount-1,prev,tempCount,max,min);
		}
		else if(waveCount > 0 && current == 0) {
			printf("%d\t%hi\t%d\t%hi\t%hi\n",totalCount,current,tempCount+1,max,min);
		}
		waveCount++;
		tempCount = 0;
		crossedZero = 0;
		max = 0;
		min = 0;
	}
	prev = current;
  }
	
  fclose(inFile); /* after the read loop is done. close file */

  return EXIT_SUCCESS;  // this is just a zero value. In C a zero return often indicates no error
}
