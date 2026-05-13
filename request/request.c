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
	STATE_REQUESTLINE,
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
}


typedef struct _request {
	RequestLine *requestLine;
	State state;
} Request;

// De donde obtengo mi state si es no se pued eenum?
// El problema radica en que esta funcion debe recibir un buffer[n:n]
// con un size predeterminado y devolverme mis bytesParsed para el siguiente
// request header.
// El problema es que paso un char *buffer, en vez de un buffer[].
// Nota: SI parseo hasta \r\n, entonces debo devolver mis bytes hasta
// \r\n y no despues, para que mi requestHeader empiece donde debe.
void parseRequest(int fildes, Request *request, char buffer[], size_t bufferSize){
	request->state = STATE_INIT;
	int reader = 0;
	while(1) {
		switch (request->state) {
			//This case is unnecessary, but the compile errors
			//was driving me crazy.
			case STATE_INIT:
				request->state = STATE_REQUESTLINE;
			case STATE_REQUESTLINE:
				int bytesParsed = parseRequestLine(fildes, request->requestLine,  buffer, bufferSize);
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
