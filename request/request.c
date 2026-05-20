#include <string.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "./request.h"
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
	memset(buffer->items, '\0', buffer->capacity);
}


typedef struct _request {
	RequestLine *requestLine;
	State state;
} Request;

int parseRequest(int fildes, Request *request, char* buffer, size_t bufferSize){
	request->state = STATE_INIT;

	int reader = 0;
	while(1) {
		switch (request->state) {
			case STATE_INIT:
				int bytesParsed = parseRequestLine(fildes, request->requestLine,  buffer, bufferSize);

				if (bytesParsed == -1) {
					return -1;
				}
				reader += bytesParsed;
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

//Empece 6:30 pm
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
	//Firs state
	while (request.state != STATE_DONE) {
		char octetBuffer[8] = {'\0'};

		//I have a conflict with NUL. It seems that I have to learn more about index.
		if ((n = read(fildes, octetBuffer, nBytes-1)) > 0) {

			buffer.count += n;
			if (buffer.count >= buffer.capacity) {
				buffer.capacity = buffer.capacity << 1;
				buffer.items = (char*) realloc(buffer.items, buffer.capacity);
				if (buffer.items == NULL) {
					printf("Error msg: %s\n", strerror(errno));
				}
				char *ptr = buffer.items + buffer.count;
				memset(ptr, '\0', buffer.capacity - buffer.count);
			}

			// Know I know why strcat is inneficient.
			strcat(buffer.items, octetBuffer);
			int byte = parseRequest(fildes, &request, buffer.items, buffer.count);

			if (byte == -1) {
				continue;
			}

		}
	}

	return buffer.items;
}
