# Hash Table Library

## Abstraction

This program provides a back-end Hash Table library for efficiently handling data insertion, querying, and deletion

## Summary

The primary objective of this program is to offer programmers a convenient header file containing a pre-built Hash Table, eliminating the need for manual code implementation. Similar to Object Oriented Programming's inheritance concept, this program functions as a parent class, allowing users to utilize all methods (in C, functions) without delving into the underlying mechanics. Adopting the principles of "information hiding," the code exposes only the essential information needed for utilizing the provided tools, keeping implementation details concealed within the code library.

## Algorithms

The library supports three algorithms:

1. **Hash by Length:**
   - Performance: Generally poor.
   - Description: The algorithms return value is the data´s length size. Collisions are inevitable due to the      key's length being much smaller than the table size.

2. **Hash by Sum:**
   - Performance: Generally efficient.
   - Description: The algorithm sums all bytes and then calculates the modulus by the table size, providing a well-distributed hash value.

3. **Hash by XOR:**
   - Performance: Similar to hash by sum.
   - Description: This algorithm involves summing byte values, applying bitwise XOR to flip bits, resulting in a less predictable hash value. While similar in cost to hash by sum, it adds a layer of complexity to the key.

## Project Specifications

- This project was created and tested using MinGW and the mingw32-make package.

## Testing

1. Download [Download HashTableLibrary-main.zip](https://github.com/pharpala/HashTableLibrary/archive/main.zip)
2. Extract the zip file to a location of your choosing.
3. Open your local computer terminal, and navigate to the HashTableLibrary-main folder location
4. To test the Hash Table, input the following command line arguments into the terminal:

```bash
./runner -n 240 -H sum -P qua -p data-byname.txt
```

or

```bash
./runner -p -n 200 -H sum -d deletefile.txt -q querybyname.txt -P qua ./data-byname.txt
```

For additional information on command line arguments, type:
```bash
./runner -h
```

## Cost Table: 
command line argument: ./runner -n 240 -H &lt;algorithm&gt; -P qua -p  data-byname.txt

| algo  | value        |
|-------|--------------|
| len   | Insertion: 961, Search: 0, Deletion: 0 |
| sum   | Insertion: 153, Search: 0, Deletion: 0 |
| xor   | Insertion: 221, Search: 0, Deletion: 0 |


## Cost Table: 
command line argument: ./runner -p -n 200 -H &lt;algorithm&gt; -d deletefile.txt -q querybyname.txt -P qua ./data-byname.txt

| algo  | value                            |
|-------|----------------------------------|
| len   | Insertion: 1424, Search: 582, Deletion: 429 |
| sum   | Insertion: 185, Search: 48, Deletion: 12   |
| xor   | Insertion: 236, Search: 123, Deletion: 26  |

11/12/2023
