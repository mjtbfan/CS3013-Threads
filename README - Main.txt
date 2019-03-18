Nicholas Delli Carpini

----- Running -----
./threads [Number of Users (>0)] [Number of Times Per User (>0)] [Average Arrival Time Per Use] [Average Time Needed Per Use]
The program runs as expected as specified in the project instructions.

----- Header -----
The header was used to keep all structs needed for each .c file, along with any functions that needed to be shared between the
two .c files. It also keeps all the includes for both .c files, as it was seemed easier to do it this way. Finally, there are a
number of locks kept in each of the structs because this seemed like the easiest way to have individual locks per thread. Whether or
not this caused issues with the program I do not know.

----- Functions -----
int stdDeviation(int value) { // helper function to get rand std deviation +/- argv value
void *runUser(void* userVoid) { // runs the thread and all functions needed from module

----- Issues -----
No issues were noticed that were specific to the main file; however, there were a couple of issues found in the module. The details
of these issues can be seen in 'README - Module.txt'.

----- Notes -----
This code barely borrows anything from the checkpoint, besides the header and some general functions. This is because after watching
the lecture emailed to use by Prof Lauer and rereading the project instructions, I came to the realization that I was doing the whole 
thing wrong. Luckily for me, up to 24 hours late is only a 10% penalty, and the fact that we had Thursday off allowed me to practically
rewrite the whole thing.