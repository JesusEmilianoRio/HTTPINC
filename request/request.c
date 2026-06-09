#include <string.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
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

int initBuffer(int fildes, Buffer *buffer) {
	buffer->capacity = 8;
	buffer->count = 0;

	buffer->items = (char*) malloc(buffer->capacity);

	if (buffer->items == NULL) {
		char errorMsg[] = "500 (Internal Server Error)";
		send(fildes, errorMsg, strlen(errorMsg), 0);
		return 0;
	}

	return 1;
}


typedef struct _request {
	RequestLine requestLine;
	State state;
} Request;

int parseRequest(int fildes, Request *request, char buffer[], size_t bufferCount, size_t *pointerReader){
	char *readFromNewPosition = NULL;
	int transition = 0;
	size_t reader = 0;

	while(1) {
		switch (request->state) {
			case STATE_INIT:
				transition = parseRequestLine(fildes, &request->requestLine,  buffer, bufferCount, &reader);

				if (transition == 0) {
					return transition;
				}

				if (transition == -1) {
					return transition;
				}

				request->state = STATE_DONE;
				*pointerReader += reader;
			case STATE_REQUESTHEADER:
				readFromNewPosition = buffer + *pointerReader;
				break;
			case STATE_REQUESTBODY:
				break;
			case STATE_DONE:
				goto end_loop;
				break;
		
		}
	}

	// Aqui tambien devuelvo 0. Maldita sea.
	// Necesito reconstruir mi automata finito ultra machine learning delux edition.
	end_loop:
		return 0;
	
}

char *request(int fildes){
	//Init Request
	Request request = {0};
	request.state = STATE_INIT;

	//Declare buffer
	Buffer buffer = {0};
	int err = initBuffer(fildes, &buffer);

	if (err == 0) {
		return NULL;
	}

	// N length of bytes read.
	size_t n = 0;
	size_t octetLength = 8;
	size_t currentPointerToBuffer = 0;
	size_t pointerReader = 0;
	//First state
	while (request.state != STATE_DONE) {
		char octetBuffer[8] = {'\0'};

		if ((n = read(fildes, octetBuffer, octetLength-1)) > 0) {

			buffer.count += n;
			if (buffer.count >= buffer.capacity) {
				buffer.capacity = buffer.capacity << 1;
				buffer.items = (char*) realloc(buffer.items, buffer.capacity);
				if (buffer.items == NULL) {
					char errorMsg[] = "500 (Internal Server Error)";
					send(fildes, errorMsg, strlen(errorMsg), 0);

					// Free others pointers. REMEMBER
					free(buffer.items);
					return NULL;
				}
			}

			currentPointerToBuffer = mystrcat(currentPointerToBuffer, buffer.items, octetBuffer);
			int transition = parseRequest(fildes, &request, buffer.items, buffer.count, &pointerReader);

			if (transition == 0) {
				return NULL;
			}

			if (transition == -1) {
				continue;
			}

		}
	}

	return buffer.items;
}
