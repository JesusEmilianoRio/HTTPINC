#include <string.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include "./request.h"
#include "../utils/functionalities.h"
#include "requestHeader/requestHeader.h"
#include "requestLine/requestLine.h"

extern int errno;

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
		return -2;
	}

	return 0;
}

int parseRequest(int fildes, Request *request, char buffer[], size_t bufferCount, size_t *pointerReader){
	int transition = 0;
	size_t reader = 0;

	while(1) {
		switch (request->state) {
			case STATE_INIT:
				transition = parseRequestLine(fildes, &request->requestLine,  buffer, bufferCount, &reader);

				if (transition == -2) {
					request->state = STATE_ERROR;
					continue;
				}

				if (transition == -1) {
					return transition;
				}

				request->state = STATE_REQUESTHEADER;
				*pointerReader += reader;
			case STATE_REQUESTHEADER:
				transition = parseRequestHeader(fildes, &request->hashTable, buffer, bufferCount, &reader);

				if (transition == -2) {
					request->state = STATE_ERROR;
					continue;
				}

				if (transition == -1) {
					return transition;
				}

				request->state = STATE_DONE;
				break;
			case STATE_REQUESTBODY:
				break;
			case STATE_DONE:
				request->state = STATE_DONE;
				return 0;
			case STATE_ERROR:
				return -2;
		
		}
	}
	
}

char *request(int fildes){
	//Init Request
	Request request = {0};
	request.state = STATE_INIT;

	//Declare buffer
	Buffer buffer = {0};
	int err = initBuffer(fildes, &buffer);

	if (err == -2) {
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

			// Simulate a sliding window.
			mystrcat(buffer.count - n, buffer.items, octetBuffer);
			
			// I need a pointer to buffer.items... I think so.
			char *pointerToBuffer = buffer.items + pointerReader;
			int transition = parseRequest(fildes, &request, pointerToBuffer, buffer.count, &pointerReader);

			if (transition == -2) {
				return NULL;
			}

			if (transition == -1) {
				continue;
			}
		
		}
	}
	
	// Despues sabre que hacer con esto.
	free(request.requestLine.method);
	free(request.requestLine.requestTarget);
	free(request.requestLine.httpVersion);


	return buffer.items;
}
