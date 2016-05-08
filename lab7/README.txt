Steven Comer
sfc15
CS449 Lab 7
12 April 2013

In the freader.c program, the bufCnt value was not always 8000 as it
should have been. This was due to the fact that bufCnt was a global
variable. Multiple threads were able to increment its value at the same
time. The final value of bufCnt was nondeterministic because it is not
possible to say with certainty in what order the threads will execute.

The important thing to realize here is that "bufCnt++" is not an atomic
operation. It must first read the current value and then place that value
plus one back into the memory that contains bufCnt. The read operation may
occur for a given thread and then before that thread is able to store the
new value, a different thread may start its increment operation. When the
first thread writes to bufCnt, it will have essentially nullified the second
increment operation. See example below.

bufCnt starts at 0
Thread 1 reads bufCnt value of 0
Thread 2 reads bufCnt value of 0
Thread 2 stores (0+1) into bufCnt
Thread 1 stores (0+1) into bufCnt

After these operations the value of bufCnt will be 1, not 2 as expected.

