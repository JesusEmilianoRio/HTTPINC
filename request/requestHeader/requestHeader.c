#include <stdint.h>
#include <stdio.h>
#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

typedef struct _hash {
	char* key;
	char* value;
} Hash;

typedef struct _hashTable {
	Hash *ht;
	int capacity;
	int size;
} HashTable;

static uint64_t hash_key(const char *key) {
	uint64_t hash = FNV_OFFSET;

	for (const char *ptr = key; *ptr; ptr++) {
		hash ^= (uint64_t)ptr;
		hash *= FNV_PRIME;
	}

	return hash;
}

int parseRequestHeader() {
	uint64_t h = hash_key("emi");

	printf("%zu\n", h);

	return 0;
}
