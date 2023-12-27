#include <stdio.h>
#include <string.h> // for strcmp()
#include <ctype.h> // for isprint()

#include "hashtools.h"

/** check if the two keys are the same */
int
doKeysMatch(AAKeyType key1, size_t key1len, AAKeyType key2, size_t key2len)
{
	/** if the lengths don't match, the keys can't match */
	if (key1len != key2len)
		return 0;

	return memcmp(key1, key2, key1len) == 0;
}

/* provide the hex representation of a value */
static char toHex(int val)
{
	if (val < 10) return (char) ('0' + val);
	return (char) ('a' + (val - 10));
}

/**
 * Provide the key in a printable form.  Uses a static buffer,
 * which means that not only is this not thread-safe, but
 * even runs into trouble if called twice in the same printf().
 *
 * That said, is does provide a memory clean way to give a 
 * printable string return value to the calling code
 */
int
printableKey(char *buffer, int bufferlen, AAKeyType key, size_t printlen)
{
	int i, allChars = 1;
	char *loadptr;


	for (i = 0; allChars && i < printlen; i++) {
		if ( ! isprint(key[i])) allChars = 0;
	}

	if (allChars) {
		snprintf(buffer, bufferlen, "char key:[%s]", (char *) key);
	} else {
		snprintf(buffer, bufferlen, "hex key:[0x");
		loadptr = &buffer[strlen(buffer)];
		for (i = 0; i < printlen && loadptr - buffer < bufferlen - 4; i++) {
			*loadptr++ = toHex((key[i] & 0xf0) >> 4); // top nybble -> first hext digit
			*loadptr++ = toHex(key[i] & 0x0f);        // bottom nybble -> second digit
		}
		*loadptr++ = ']';
		*loadptr++ = 0;
	}
	return 1;
}

/**
 * Calculate a hash value based on the length of the key
 *
 * Calculate an integer index in the range [0...size-1] for
 * 		the given string key
 *
 *  @param  key  key to calculate mapping upon
 *  @param  size boundary for range of allowable return values
 *  @return      integer index associated with key
 *
 *  @see    HashAlgorithm
 */
HashIndex hashByLength(AAKeyType key, size_t keyLength, HashIndex size)
{
	return keyLength % size;
}



/**
 * Calculate a hash value based on the sum of the values in the key
 *
 * Calculate an integer index in the range [0...size-1] for
 * 		the given string key, based on the sum of the values
 *		in the key
 *
 *  @param  key  key to calculate mapping upon
 *  @param  size boundary for range of allowable return values
 *  @return      integer index associated with key
 */

HashIndex hashBySum(AAKeyType key, size_t keyLength, HashIndex size)
{
	HashIndex sum = 0;

	for(int i = 0; i<keyLength; i++)
	{
		sum += (HashIndex)key[i];
	}
	return sum % size;
}

/**
 * Calculate a hash value based on the sum of the values, flipped in biinary, in the key
 *
 * Calculate an integer index in the range [0...size-1] for
 * 		the given string key, based on the sum of the values
 *		in the key
 *
 *  @param  key  key to calculate mapping upon
 *  @param  size boundary for range of allowable return values
 *  @return      integer index associated with key
 */

HashIndex hashByXOR(AAKeyType key, size_t keyLength, HashIndex size)
{
	HashIndex value = 0;

	for(int i = 0; i<keyLength; i++)
	{
		value ^= (HashIndex)key[i];
	}
	return value % size;
}


/**
 * Locate an empty position in the given array, starting the
 * search at the indicated index, and restricting the search
 * to locations in the range [0...size-1]
 *
 *  @param  index where to begin the search
 *  @param  AssociativeArray associated AssociativeArray we are probing
 *  @param  invalidEndsSearch should the identification of a
 *				KeyDataPair marked invalid end our search?
 *				This is true if we are looking for a location
 *				to insert new data
 *  @return index of location where search stopped, or -1 if
 *				search failed
 *
 *  @see    HashProbe
 */


HashIndex linearProbe(AssociativeArray *hashTable,
		AAKeyType key, size_t keylength,
		int index, int invalidEndsSearch, int *cost
	)
{
	/**
	 * We will need to implement an algorithm
	 * that probes until it finds an "empty" slot in
	 * the hashTable.  Note that because of tombstones,
	 * there are multiple ways of a slot being empty.
	 * Additionally, the "index" value may
	 * already be valid on entry.
	 *
	 * Note that if an empty place cannot be found,
	 * we are to return (-1).
	 *
	 * For this routine, we will implement a "linear" probing
	 * strategy.
	 */

    // Initial step size for linear probing (1 means moving to the next slot)
    int start = index;

	// Note that 0 represents HASH_EMPTY, 1 representents HASH_USED, and 2 HASH_DELETED, as seen in hashtools.h
    while (hashTable->table[start].validity != 0 && (invalidEndsSearch || hashTable->table[start].validity == 2)) 
	{
        start = (start + 1) % hashTable->size;
        (*cost)++;

        if (start == index) 
		{
            fprintf(stderr, "Hash table full\n");
            return -1;
        }
    }
	return start;
}


/**
 * Locate an empty position in the given array, starting the
 * search at the indicated index, and restricting the search
 * to locations in the range [0...size-1]
 *
 *  @param  index where to begin the search
 *  @param  hashTable associated HashTable we are probing
 *  @param  invalidEndsSearch should the identification of a
 *				KeyDataPair marked invalid end our search?
 *				This is true if we are looking for a location
 *				to insert new data
 *  @return index of location where search stopped, or -1 if
 *				search failed
 *
 *  @see    HashProbe
 */

HashIndex quadraticProbe(AssociativeArray *hashTable, AAKeyType key, size_t keylen,
		int startIndex, int invalidEndsSearch,
		int *cost
	)
{
	/**
	 * We will need to implement an algorithm
	 * that probes until it finds an "empty" slot in
	 * the hashTable.  Note that because of tombstones,
	 * there are multiple ways of a slot being empty.
	 * Additionally, the "index" value may
	 * already be valid on entry.
	 *
	 * Note that if an empty place cannot be found,
	 * we are to return (-1).
	 *
	 * For this routine, we will implement a "quadratic" probing
	 * strategy.
	 */


    int start = startIndex;

	// Note that 0 represents HASH_EMPTY, 1 representents HASH_USED, and 2 HASH_DELETED, as seen in hashtools.h
    while (hashTable->table[start].validity != 0 && (invalidEndsSearch || hashTable->table[start].validity == 2)) 
	{
        start = (start *2) % hashTable->size;
        (*cost)++;

        if (start == startIndex) 
		{
            fprintf(stderr, "Hash table full\n");
            return -1;
        }
    }
	return start;
}


/**
 * Locate an empty position in the given array, starting the
 * search at the indicated index, and restricting the search
 * to locations in the range [0...size-1]
 *
 *  @param  index where to begin the search
 *  @param  hashTable associated HashTable we are probing
 *  @param  invalidEndsSearch should the identification of a
 *				KeyDataPair marked invalid end our search?
 *				This is true if we are looking for a location
 *				to insert new data
 *  @return index of location where search stopped, or -1 if
 *				search failed
 *
 *  @see    HashProbe
 */

HashIndex doubleHashProbe(AssociativeArray *hashTable, AAKeyType key, size_t keylen,
		int startIndex, int invalidEndsSearch,
		int *cost
	)
{
	/**
	 * We will need to implement an algorithm
	 * that calls a second hash function (listed
	 * in the hashTable) and uses the value obtained
	 * as a result from that as the step size.
	 *
	 * Beyond that, the algorithm proceeds as with
	 * the above strategies.
	 */

	while(1)
	{
		KeyDataPair *pair = &hashTable->table[startIndex];

        // Check if current slot is empty or deleted
		// Note that 0 represents HASH_EMPTY, 1 representents HASH_USED, and 2 HASH_DELETED, as seen in hashtools.h
        if (pair->validity == 0 || pair->validity == 2) 
		{
            return startIndex;  // Empty or deleted slot
        }

        // Check if the current slot is invalid and should end the search
        if (invalidEndsSearch && pair->validity == 1) 
		{
            return -1;  // Search should end
        }	

		startIndex = hashTable->hashAlgorithmSecondary(key, keylen, hashTable->size);
		(*cost)++;
	}

	return -1;
}
