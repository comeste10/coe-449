Steven Comer
CS 449
Project 3
8 March 2013

sfc15_1

Passphrase(s): 
“DJSnhpWztmrOwbFpwtSLLdjrhSWvi”

Notes:
I followed procedures used in the puzzle lab. First, I looked
at the disassembly of main. Then I looked for a call of strcmp
as described in the puzzle lab. I did not see this directly but
I did notice a keyword “cmpsb”. This appeared to be some sort of
comparison operation. I looked this up in an x86 reference manual.
This operation compares two bytes. On the same line as “cmpsb”
was “repz”. I looked this up as well and it seems to repeat an
operation. At this point, I determined that “repz cmpsb” was the
important line for me to examine. It repeatedly compared bytes in memory.
I set breakpoints at main and at the “mov” instruction prior to the
“cmpsb” instruction, which set the arguments for the function call.
During execution, I entered the string “hello”. This appeared in
the esi register at the comparison breakpoint. The other register
used in the comparison is the edi register. Upon examination, it
 contained the string “DJSnhpWztmrOwbFpwtSLLdjrhSWvi”. At this point,
I removed the breakpoints and ran the program again. I entered
“DJSnhpWztmrOwbFpwtSLLdjrhSWvi” when prompted. The program exited correctly.
Later, I went back and ran mystrings on this file. The passphrase
appeared near the top of the string dump.



sfc15_2

Passphrase(s): 
Any string with the following two characteristics: sequence
of repeated identical characters, and length greater than or equal to 6
characters. My first correct solution was “aaaaaaa”. Other acceptable
passwords include: “******************”, “$$$$$$$$$$$$$”, 
“kkkkkkkkkkkkkkkkkkkkk”, etc.

Notes:
Again, I followed the procedures outlined in the puzzle lab.
After running “gdb disas” on main, the first thing that I noticed was
how short the main function was. I also saw what appeared to be a custom
function call in the main function. My first step was to place a break
point at the address called in this function. This took me to a different
function. This is where I assumed most of the operation was taking place.
I ran “disas” again on this function. This appeared similar to the
previous main functions I had examined. The first instruction that jumped
out at me was “cmp $0x5, %eax”. Because this comparison involved a constant,
my first attempt at the answer was the constant used in the comparison.
When 5 did not work as an answer, my second guess was that the answer must
be 5 characters in length and that the comparison checked the string length.
I tried entering a string with 5 characters (“hello”). This also did not work.
I now returned to look at the disassembly of the first function which was
called by main. This function contained 3 more obscurely named function calls.
They were named c, r, and s respectively. I set a breakpoint at c to examine
it more closely. Function c: This function contains a similar instruction to
the one I had seen in the previous executable. It is 
“cmpb $0xa, -0x1(%ebx, %eax,1)”. Upon further research, 
this instruction takes an immediate argument, in this case hexadecimal 0xA,
and compares it to the contents of the memory address listed as the second
argument. I discovered that the memory address given by “-0x1(%ebx, %eax, 1)”
uses GAS syntax and that %ebx is the base register. I decided to set a
breakpoint at this instruction and examine the contents of %ebx. At this point
in the program, %ebx contained my input string of “hello\n”. I noticed that
this string could have length 6 because of the newline at the end. I decided
to try my length 5 answer again, but entering 4 characters to attempt to end
up with a string of length 5 including the newline. I entered “hell” 
(read as “hell\n”). This did not work. My next thought was to try entering the
character a, because this was the immediate value used in the comparison. 
This also did not work. Next, I looked deeper into the other function calls.
Function s: This function is called from within function c. This function
contains an interesting instruction “cmpb $0x0, (%edx)” At this point, I
returned to the first instruction I examined, “cmp $0x5, %eax”. I had already
tried entering a string with fewer than 5 characters, and one with exactly
5 characters. I now tried entering more than 5 characters. I entered 
“aaaaaaaaaaaa” and it worked. After trying other combinations, I determined
that any string of ASCII characters which are identical and has length greater
than 5 characters will work. Several solutions are “aaaaaa”, “xxxxxxxxxxxx”,
and “&&&&&&&&&”. Attempts that don’t work are “asdfasdf”, “asdf”, “hello”, etc.



sfc15_3

Passphrase(s):  Any sequence of exactly 5 characters in the range [a-e],
regardless of case. The program captures up to 10 carriage returns, 5 single
characters each followed by a carriage return or combinations of multiple
characters per line and carriage returns. Acceptable sequences include: 
"AAAAA [ENTER] [ENTER] [ENTER] [ENTER] [ENTER]” and 
“b [ENTER] b [ENTER] c [ENTER] d [ENTER] a [ENTER]”.

Notes:
First, I attempted to follow the procedure given in the puzzle lab.
I entered in seemingly random characters and got the correct answer.
My sequence of input was “a [RETURN] b [RETURN] c [RETURN] d [RETURN] e [RETURN]”.
At this point the program unlocks and says that I entered the correct
passphrase of “a”. I experimentally determined that the program can
take up to 9 characters on a single line input. Another option is to
individually enter in 5 separate single characters (as in my working solution).
Each character is separated by a carriage return. The single most
important command for finding the true solution to this program was
“objdump –S sfc15_3”. This listed all the assembly instructions,
not necessarily in order of execution. The most important line I
found here was “sub $0x61, %eax” immediately followed by a comparison
“cmp $0x4, %eax”. The subtraction finds the difference from the value
of the letter ‘a’ because 0x61 is the ASCII representation of ‘a’.
The comparison checks to see if the difference is less than 4.
A difference of less than 4, when compared to the letter ‘a’,
means that the character is in the range [a-e]. The tolower
function is called on input character to make it case insensitive.
For a correct solution, exactly 5 characters must be entered.
