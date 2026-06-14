#include <stddef.h>
#include <stdint.h>
#include <string.h>
#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

#include "../request.h"

char crlf[] = "\r\n";
int error = -2;

// Linked List
typedef struct _node {
	char *key;
	char *value;
	struct _node *next;
} Node;

// Hash table
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
//int parseRequestLine(int clientSocket, RequestLine *requestLine, char *buffer, size_t size, size_t *bytesRead) {
//	char* index = (char*) memmem(buffer, size, crlf, strlen(crlf));
//	
//	// It will return me to my init state.
//	if (index == NULL) {
//		return -1;
//	}
//
//	// Total Bytes Parsed.
//	size_t bytesParsed = (index - buffer) + strlen(crlf);
//

// Le agregare yo el requestHeader desde aca?
int parseRequestHeader(int clientSocket, Request *request, char *buffer, size_t size, size_t *bytesRead) {
	char* index = (char*) memmem(buffer, size, crlf, strlen(crlf));
	
	if (index == NULL) {
		return -1;
	}

	size_t lenCRLF = strlen(crlf);
	if (index > 0) {
		
	}

	// Transition.
	*bytesRead += lenCRLF;
	return 0;
}
