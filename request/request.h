#pragma once

//Init Request Line
typedef struct _requestLine {
	char *method;
	char *requestTarget;
	char *httpVersion;
} RequestLine;

// Linked List
typedef struct _node {
	char *key;
	char *value;
	struct _node *next;
} Node;

// Hash table
typedef struct _hashTable {
	Node **hash;
	int capacity;
	int size;
} HashTable;


// Init State
typedef enum {
	STATE_INIT,
	STATE_REQUESTHEADER,
	STATE_REQUESTBODY,
	STATE_DONE,
	STATE_ERROR,
} State;

//Init Request
typedef struct _request {
	RequestLine requestLine;
	HashTable hashTable;
	State state;
} Request;

char *request(int fildes);
