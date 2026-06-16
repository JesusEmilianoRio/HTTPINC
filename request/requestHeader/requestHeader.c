#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

#include "../request.h"

char crlf[] = "\r\n";
int error = -2;


static uint64_t hashKey(const char *key) {
	uint64_t hash = FNV_OFFSET;

	for (const char *ptr = key; *ptr; ptr++) {
		hash ^= (uint64_t)ptr;
		hash *= FNV_PRIME;
	}

	return hash;
}

int initHash(int clientSocket, HashTable *requestHeader) {
	requestHeader->capacity = 0;
	requestHeader->size = 20;
	
	requestHeader->hash = (Node**) malloc(requestHeader->size * sizeof(Node*));

	if (requestHeader->hash == NULL) {
		char errorMsg[] = "500 (Internal Server Error)";
		send(clientSocket, errorMsg, strlen(errorMsg), 0);
		return error;
	}

	// Despues me preocupo de este retorno.
	// TODO:
	// 1. ARREGLAR ESTE RETORNO
	return 0;
};

int parseRequestHeader(int clientSocket, HashTable *requestHeader, char *buffer, size_t size, size_t *bytesRead) {
	char* index = (char*) memmem(buffer, size, crlf, strlen(crlf));
	
	if (index == NULL) {
		return -1;
	}

	int getError = initHash(clientSocket, requestHeader);

	if (getError == error) {
		return error;
	}

	size_t lenCRLF = strlen(crlf);
	if (index > 0) {
		
		
	}

	// Transition.
	*bytesRead += lenCRLF;
	return 0;
}
