#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


#include "hashtools.h"

/** forward declaration */
static HashAlgorithm lookupNamedHashStrategy(const char *name);
static HashProbe lookupNamedProbingStrategy(const char *name);

/**
 * Create a hash table of the given size,
 * which will use the given algorithm to create hash values,
 * and the given probing strategy
 *
 *  @param  hash  the HashAlgorithm to use
 *  @param  probingStrategy algorithm used for probing in the case of
 *				collisions
 *  @param  newHashSize  the size of the table (will be rounded up
 *				to the next-nearest larger prime, but see exception)
 *  @see         HashAlgorithm
 *  @see         HashProbe
 *  @see         Primes
 *
 *  @throws java.lang.IndexOutOfBoundsException if no prime number larger
 *				than newHashSize can be found (currently only primes
 *				less than 5000 are known)
 */
AssociativeArray *
aaCreateAssociativeArray(
		size_t size,
		char *probingStrategy,
		char *hashPrimary,
		char *hashSecondary
	)
{
	AssociativeArray *newTable;

	newTable = (AssociativeArray *) malloc(sizeof(AssociativeArray));

	newTable->hashAlgorithmPrimary = lookupNamedHashStrategy(hashPrimary);
	newTable->hashNamePrimary = strdup(hashPrimary);
	newTable->hashAlgorithmSecondary = lookupNamedHashStrategy(hashSecondary);
	newTable->hashNameSecondary = strdup(hashPrimary);
	newTable->hashProbe = lookupNamedProbingStrategy(probingStrategy);
	newTable->probeName = strdup(probingStrategy);

	newTable->size = getLargerPrime(size);

	if (newTable->size < 1) {
		fprintf(stderr, "Cannot create table of size %ld\n", size);
		free(newTable);
		return NULL;
	}

	newTable->table = (KeyDataPair *) malloc(newTable->size * sizeof(KeyDataPair));

	/** initialize everything with zeros */
	memset(newTable->table, 0, newTable->size * sizeof(KeyDataPair));

	newTable->nEntries = 0;

	newTable->insertCost = newTable->searchCost = newTable->deleteCost = 0;

	return newTable;
}

/**
 * deallocate all the memory in the store -- the keys (which we allocated),
 * and the store itself.
 * The user * code is responsible for managing the memory for the values
 */
void
aaDeleteAssociativeArray(AssociativeArray *aarray)
{
	/**
	 * TO DO:  clean up the memory managed by our utility
	 *
	 * Note that memory for keys are managed, values are the
	 * responsibility of the user
	 */

	 if (aarray == NULL) 
	{
        return; //nothing to delete
    }

    //free dynamically allocated strings
    free(aarray->hashNamePrimary);
    free(aarray->hashNameSecondary);
    free(aarray->probeName);

    //free table of KeyDataPairs
    free(aarray->table);

    //free the AssociativeArray
    free(aarray);
}

/**
 * iterate over the array, calling the user function on each valid value
 */
int aaIterateAction(
		AssociativeArray *aarray,
		int (*userfunction)(AAKeyType key, size_t keylen, void *datavalue, void *userdata),
		void *userdata
	)
{
	int i;

	for (i = 0; i < aarray->size; i++) {
		if (aarray->table[i].validity == HASH_USED) {
			if ((*userfunction)(
					aarray->table[i].key,
					aarray->table[i].keylen,
					aarray->table[i].value,
					userdata) < 0) {
				return -1;
			}
		}
	}
	return 1;
}

/** utilities to change names into functions, used in the function above */
static HashAlgorithm lookupNamedHashStrategy(const char *name)
{
	if (strncmp(name, "sum", 3) == 0) {
		return hashBySum;
	} else if (strncmp(name, "len", 3) == 0) {
		return hashByLength;

		// TO DO: add in your own strategy here
	}

	fprintf(stderr, "Invalid hash strategy '%s' - using 'sum'\n", name);
	return hashBySum;
}

static HashProbe lookupNamedProbingStrategy(const char *name)
{
	if (strncmp(name, "lin", 3) == 0) {
		return linearProbe;
	} else if (strncmp(name, "qua", 3) == 0) {
		return quadraticProbe;
	} else if (strncmp(name, "dou", 3) == 0) {
		return doubleHashProbe;
	}

	fprintf(stderr, "Invalid hash probe strategy '%s' - using 'linear'\n", name);
	return linearProbe;
}

/**
 * Add another key and data value to the table, provided there is room.
 *
 *  @param  key  a string value used for searching later
 *  @param  value a data value associated with the key
 *  @return      the location the data is placed within the hash table,
 *				 or a negative number if no place can be found
 */
int aaInsert(AssociativeArray *aarray, AAKeyType key, size_t keylen, void *value)
{
	/**
	 * TO DO:  Search for a location where this key can go, stopping
	 * if we find a value that has been delete and reuse it.
	 *
	 * If a suitable location is found, we then initialize that
	 * slot with the new key and data
	 */


	int index = aarray->hashProbe(aarray, key, keylen, 0, 0, &aarray->insertCost);

    if (index < 0) 
	{
        fprintf(stderr, "Failed to insert key-value pair. Hash table is full.\n");
        return -1;
    }

	while(1)
	{
		// Check if the current slot is empty or marked as deleted
		KeyDataPair *pair = &aarray->table[index];
		if (pair->validity == 0 || pair->validity == 2) 
		{
			// Initialize the slot with the new key and value
			pair->key = key;
			pair->keylen = keylen;
			pair->value = value;
			pair->validity = 0; // Set it as a valid entry
			aarray->nEntries++;
			
			return index;
		}
	 
	
		index = aarray->hashAlgorithmSecondary(key, keylen, aarray->size);
		aarray->insertCost++;

    	return index; // Return the index where the data is placed within the hash table
	}
}


/**
 * Locates the KeyDataPair associated with the given key, if
 * present in the table.
 *
 *  @param  key  the key to search for
 *  @return      the KeyDataPair containing the key, if the key
 *				 was present in the table, or NULL, if it was not
 *  @see         KeyDataPair
 */
void *aaLookup(AssociativeArray *aarray, AAKeyType key, size_t keylen
	)
{
	/**
	 * TO DO: perform a similar search to the insert, but here a
	 * deleted location means we have not found the key
	 */

	int index = aarray->hashProbe(aarray, key, keylen, 0, 0, &aarray->searchCost);
    int size = aarray->size;

    if (index < 0) 
	{
        return NULL; // Table is full or no available slot found
    }

    while (1) 
	{
        KeyDataPair *pair = &aarray->table[index];

        // Check if the current slot is empty
        if (pair->validity == 0) 
		{
            return NULL; // Key is not in the table
        }

        // Check if the current slot is marked as deleted
        if (pair->validity == 2) 
		{
            //means the key is not in the table
            index++;
            aarray->searchCost++; // Increase the probing cost
            continue;
        }

        // Check if the current slot matches the key
        if (pair->keylen == keylen && memcmp(pair->key, key, keylen) == 0) 
		{
            return pair->value; //key found so return the associated value
        }

        index = aarray->hashAlgorithmSecondary(key, keylen, size);
        aarray->searchCost++; // Increase the probing cost
    }
}


/**
 * Locates the KeyDataPair associated with the given key, if
 * present in the table.
 *
 *  @param  key  the key to search for
 *  @return      the index of the KeyDataPair containing the key,
 *				 if the key was present in the table, or (-1),
 *				 if no key was found
 *  @see         KeyDataPair
 */
void *aaDelete(AssociativeArray *aarray, AAKeyType key, size_t keylen)
{
	/**
	 * TO DO: Deletion is closely related to lookup;
	 * you must find where the key is stored before
	 * you delete it, after all.
	 *
	 * Implement a deletion algorithm based on tombstones,
	 * as described in class
	 */

	int index = aarray->hashProbe(aarray, key, keylen, 0, 0, &aarray->searchCost);
    int size = aarray->size;

    if (index < 0) {
        return NULL; // Table is full or no available slot found
    }

    while (1) {
        KeyDataPair *pair = &aarray->table[index];

        // Check if the current slot is empty
        if (pair->validity == 0) 
		{
            return NULL; // Key is not in the table
        }

        // Check if slot is marked as deleted
        if (pair->validity == 2) 
		{
            index++;
            aarray->searchCost++; // Increase the probing cost
            continue;
        }

        //check if the current slot matches key
        if (pair->keylen == keylen && memcmp(pair->key, key, keylen) == 0) 
		{
            //key found so mark it tombstone
            pair->validity = 2;
            aarray->nEntries--; //decrement number of entries
            return pair->value; //return associated value 
        }

        index= aarray->hashAlgorithmSecondary(key, keylen, size);
        aarray->searchCost++;
    }

	return NULL;
}

/**
 * Print out the entire aarray contents
 */
void aaPrintContents(FILE *fp, AssociativeArray *aarray, char * tag)
{
	char keybuffer[128];
	int i;

	fprintf(fp, "%sDumping aarray of %d entries:\n", tag, aarray->size);
	for (i = 0; i < aarray->size; i++) {
		fprintf(fp, "%s  ", tag);
		if (aarray->table[i].validity == HASH_USED) {
			printableKey(keybuffer, 128,
					aarray->table[i].key,
					aarray->table[i].keylen);
			fprintf(fp, "%d : in use : '%s'\n", i, keybuffer);
		} else {
			if (aarray->table[i].validity == HASH_EMPTY) {
				fprintf(fp, "%d : empty (NULL)\n", i);
			} else if ( aarray->table[i].validity == HASH_DELETED) {
				printableKey(keybuffer, 128,
						aarray->table[i].key,
						aarray->table[i].keylen);
				fprintf(fp, "%d : empty (deleted - was '%s')\n", i, keybuffer);
			} else {
				fprintf(fp, "%d : invalid validity state %d\n", i,
						aarray->table[i].validity);
			}
		}
	}
}


/**
 * Print out a short summary
 */
void aaPrintSummary(FILE *fp, AssociativeArray *aarray)
{
	fprintf(fp, "Associative array contains %d entries in a table of %d size\n",
			aarray->nEntries, aarray->size);
	fprintf(fp, "Strategies used: '%s' hash, '%s' secondary hash and '%s' probing\n",
			aarray->hashNamePrimary, aarray->hashNameSecondary, aarray->probeName);
	fprintf(fp, "Costs accrued due to probing:\n");
	fprintf(fp, "  Insertion : %d\n", aarray->insertCost);
	fprintf(fp, "  Search    : %d\n", aarray->searchCost);
	fprintf(fp, "  Deletion  : %d\n", aarray->deleteCost);
}

