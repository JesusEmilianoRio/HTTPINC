#include <string.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "./request.h"
#include "../utils/functionalities.h"
#include "requestLine/requestLine.h"

extern int errno;

// Initlize state
typedef enum {
	STATE_INIT,
	STATE_REQUESTHEADER,
	STATE_REQUESTBODY,
	STATE_DONE,
} State;

//Init buffer
typedef struct _buffer {
	size_t capacity;
	size_t count;
	char *items;
} Buffer;

void initBuffer(Buffer *buffer) {
	buffer->capacity = 8;
	buffer->count = 0;

	buffer->items = (char*) malloc(buffer->capacity);

	if (buffer->items == NULL) {
		printf("Error msg: %s\n", strerror(errno));
	}
}


typedef struct _request {
	RequestLine *requestLine;
	State state;
} Request;

int parseRequest(int fildes, Request *request, char* buffer, size_t bufferCount){
	request->state = STATE_INIT;

	//All my returns from my functions could be -1 if something went wrong, or 0 if CRLF is at index 0.
	size_t reader = 0;
	while(1) {
		switch (request->state) {
			case STATE_INIT:
				// I have to check pointers to pointers. GADDEMIT
				int transition = parseRequestLine(fildes, request->requestLine,  buffer, bufferCount, &reader);

				if (transition == -1) {
					return -1;
				}
				break;
			case STATE_REQUESTHEADER:
				break;
			case STATE_REQUESTBODY:
				break;
			case STATE_DONE:
				break;
		
		}
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
	size_t nBytes = 8;
	size_t currentPointerToBuffer = 0;
	//First state
	while (request.state != STATE_DONE) {
		char octetBuffer[8] = {'\0'};

		if ((n = read(fildes, octetBuffer, nBytes-1)) > 0) {

			buffer.count += n;
			if (buffer.count >= buffer.capacity) {
				buffer.capacity = buffer.capacity << 1;
				buffer.items = (char*) realloc(buffer.items, buffer.capacity);
				if (buffer.items == NULL) {
					printf("Error msg: %s\n", strerror(errno));
				}
			}

			currentPointerToBuffer = mystrcat(currentPointerToBuffer, buffer.items, octetBuffer);

		}
	}

	return buffer.items;
}
