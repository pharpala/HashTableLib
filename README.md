Pranay Raju Harpalani Dubey
1249973

This assignment implements code from the files "data-reader.c", "mainline.c" and "primes.c". The changes made are on files "hash-table.c" and "hash-functions.c". Its dependencies are:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h> 
#include "hashtools.h"

The code shows the implementation of 3 algorithms in a hash Table that has been tested to check their cost and effectiveness.

The first one is a hash by length, given by the professor, which performs poorly. Due to the length of the key always being a small number compared to the size of the table, we will always have collisions between the first spaces in our hash table.

The second one is a hash by sum, given in the lecture slides, which performs quite well. The function sums all the bytes up and then mods it by the size of the table, since the sum is a great value, when modding it by the size we will be able to receive a number anywhere in between the table which is great! 

The third one is a hash by xor, made by me, which performs the same as sum. This function just sums the byte values up and mods it, but the twist is that it flips the bits, therefore we get a number we would normally not expect or be able to decipher easily, as it is using the keys bits, we will get the same key for the same value. The cost is similar to sum, as if two numbers produce the same hash value in the sum, once we flip the bits of those numbers we may still get the same value, it will depend on the size of the table, so we should be filling up the table. Ideally, this will avoid clusters but that will depend on our data. Finally, this algorithm will result in a cost similar to hash by sum, but with a little fun twist and a tougher to decipher key.


COST TABLE: (command line argument: ./a3 -n 240 -H <algorithm> -P qua -p  data-byname.txt)
algo        value

len         Insertion : 961
            Search    : 0
            Deletion  : 0

sum         Insertion : 153
            Search    : 0
            Deletion  : 0

xor         Insertion : 221
            Search    : 0
            Deletion  : 0


All functions in the assignment description have been tested and implemented to work. Assignment is complete.

11/12/2023
