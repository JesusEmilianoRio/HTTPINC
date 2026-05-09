#include <string.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "./request.h"

extern int errno;

// Initlize state
typedef enum {
	STATE_INIT,
	STATE_REQUESTLINE,
	STATE_REQUESTHEADER,
	STATE_REQUESTBODY,
	STATE_DONE,
} State;

//Init buffer
typedef struct _buffer {
	char *items;
	size_t capacity;
	size_t count;
} Buffer;

void initBuffer(Buffer *buffer) {
	buffer->capacity = 8;
	buffer->count = 0;
}

typedef struct _request {
	RequestLine requestLine;
	State state;
} Request;

void parseRequest(Request *request, size_t bufferSize){
	
	while(1) {
		
	}
	
}

char *request(int fildes){
	//Init Request
	Request request = {0};
	request.state = STATE_INIT;

	//Declare buffer
	Buffer buffer = {0};
	initBuffer(&buffer);

	// N length of bytes read.
	size_t n = 0;
	while (request.state != STATE_DONE) {
		if ((n = read(fildes, buffer.items + buffer.count, buffer.capacity - buffer.count)) > 0) {
			buffer.count += n;
			
			if (buffer.count >= buffer.capacity) {
				buffer.capacity = buffer.capacity << 1;
				buffer.items = (char*) realloc(buffer.items, buffer.capacity);

				if (buffer.items == NULL) {
					printf("Error msg: %s\n", strerror(errno));
				}
			}
		}
	}

	return buffer.items;
}


