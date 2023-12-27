# Associative Arrays

We can think of a regular array as being a data structure that
associates keys with values, where the keys all have to be consecutive
integers starting at 0.

An "associative array" is a key/value store where we can access data
using keys of arbitrary types.

We are going to build an associative array tool that is backed
by a hash table.  We will implement this in a *library*.

## Library based design

When distributing code to clients or users, it is common to compile
our code and distribute it in a single file called a library, and
accompany this library with whatever header files are required to
access the library API (application programmer interface).

This follows the good practice of "information hiding" which ensures
that code that **should** not care about specific implementation
details **cannot** care about these details because they can't even
see them.

We will therefore compile our code into a library called `libaa.a`
(which stands for *library AA archive*).  This *library file*
will contain all the compiled code for our tool.

The commands to build this library are built into the `makefile`.

### The nuts and bolts

The actual work goes on inside the code in the `aalib` directory.

A breakdown of the helper files for this project are:

* `hashtools.h` -- a header file that does talk about hash tables, and
 provides the data types and tools that we will use in this project.

* `hash-functions.c` -- a source file containing the implementations
 of the various hashing and probing functions we will use.

* `hash-table.c` -- a source file containing the actual work of operating
 the hash table: creating and destroying the table itself, and inserting,
 deleting and querying the table.

* `primes.c` -- a source file with a function to find a prime number for
 you.  This should be used to create your hashtable's memory allocation
 based on a prime number slightly larger than whatever size the user
 asked for.

# User code testing

Code using the API described in `aarray.h` has been provided in `mainline.c`.

This code supports a number of operations.  Details on how to access these are
provided in the help text.

Key operations are:

* interpreting integer keys as binary integers -- this lets us build an associative
 array where the keys have a mixture of types, which is one of the most valuable
 attributes of this kind of tool

* exploring different sizes of storage table

* choosing the hashing and probing algorithms to use

* performing queries on your table

* deleting values from your table (before any query is performed)

Using these options will allow us to exercise our associative array to ensure that
it works robustly.

# Testing data

The file format for the data to load is simply lines using a tab character
as a divider between the key and the value.  If there is more than one tab
on a line, it is simply stored as part of the value.

If the key begins with a digit and the option to interpret integer keys as
binary integers is used, these are converted to binary ints and stored as
keys of length 4 bytes (sizeof(int)).

Query and deletion files are simply lists of keys, one per line.  The same
rule applies regarding integer values.