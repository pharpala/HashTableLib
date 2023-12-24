## explicitly add debugger support to each file compiled,
## and turn on all warnings.

CFLAGS = -g -Wall -Iaalib -I.

##
## We can define variables for values we will use repeatedly below
##

## define the executables we want to build
A3EXE = a3


## define the set of object files we need to build each executable
A3OBJS		= \
			data-reader.o \
			mainline.o

AALIB = libAA.a

AALIBOBJS	= \
			aalib/hash-functions.o \
			aalib/hash-table.o \
			aalib/primes.o

##
## TARGETS: below here we describe the target dependencies and rules
##
all: $(A3EXE)

$(A3EXE): $(A3OBJS) $(AALIB)
	$(CC) $(CFLAGS) -o $(A3EXE) $(A3OBJS) $(AALIB)


## The ar(1) tool is used to create static libraries.  On Linux
## this is still the tool to use, equivalent to libtool(1). 
$(AALIB): $(AALIBOBJS)
	ar rcs $(AALIB) $(AALIBOBJS)
	

## convenience target to remove the results of a build
clean :
	- rm -f $(A3OBJS) $(A3EXE)
	- rm -f $(AALIBOBJS) $(AALIB)


## tags -- editor support for function definitions
tags : dummy
	ctags *.c aalib/*.c

## a "dummy" dependency forces its parent to run always
dummy :

