#include <stdlib.h>
#define _GNU_SOURCE 
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <stddef.h>

#include "../request.h"
#include "../../utils/functionalities.h"

char crlf[] = "\r\n";
char *sp = " ";
int error = -2;

int splitRequestLineMethod(int clientSocket, char rqL[], char *splitReqL[], size_t size) {
	char *spRequest;
	char *token = strtok_r(rqL, sp, &spRequest);
	size_t counter = 0;

	for (size_t i = 0; i < size && token != NULL; i++) {
		splitReqL[i] = token;
		token = strtok_r(NULL, sp, &spRequest);
		counter++;
	}

	if (counter != size || token != NULL) {
		char *errorMSG = "400 (Bad Request)";
		send(clientSocket, errorMSG, strlen(errorMSG), 0);
		return error;
	}

	return 0;
}

int splitHttpVersionMethod(int clientSocket, char httpVersion[], char *splitHttpVersion[], size_t size) {
	char *spRequest;
	char *token = strtok_r(httpVersion, "/", &spRequest);
	size_t counter = 0;

	for (size_t i = 0; i < size && token != NULL; i++) {
		splitHttpVersion[i] = token;
		token = strtok_r(NULL, "/", &spRequest);
		counter++;
	}

	if (counter != size || token != NULL) {
		char *errorMSG = "400 (Bad Request)";
		send(clientSocket, errorMSG, strlen(errorMSG), 0);
		return error;
	}

	char *validateHttpVersion[] = {"HTTP", "1.1"};
	int stringCompare = strcmp(splitHttpVersion[0], validateHttpVersion[0]);

	if (stringCompare != 0) {
		char *errorMSG = "400 (Bad Request)";
		send(clientSocket, errorMSG, strlen(errorMSG), 0);
		return error;
	}

	stringCompare = strcmp(splitHttpVersion[1], validateHttpVersion[1]);

	if (stringCompare != 0) {
		char *errorMSG = "505 (HTTP Version Not Supported)";
		send(clientSocket, errorMSG, strlen(errorMSG), 0);
		return error;
	}

	return 0;
}

int initRequestLine(int fildes, RequestLine *requestLine, const char* method, const char* requestTarget, const char* httpVersion) {
	// Add method to request.method
	requestLine->method = (char*) malloc(strlen(method) + 1);
	if (requestLine->method == NULL) {
		char errorMsg[] = "500 (Internal Server Error)";
		send(fildes, errorMsg, strlen(errorMsg), 0);
		free(requestLine->method);
		return error;
	}

	strcpy(requestLine->method, method);

	// Add requestTarget to request.requestTarget
	requestLine->requestTarget = (char*) malloc(strlen(requestTarget) + 1);
	if (requestLine->requestTarget == NULL) {
		char errorMsg[] = "500 (Internal Server Error)";
		send(fildes, errorMsg, strlen(errorMsg), 0);
		free(requestLine->requestTarget);
		return error;
	}

	strcpy(requestLine->requestTarget, requestTarget);

	// Add httpVersion to request.httpVersion
	requestLine->httpVersion = (char*) malloc(strlen(httpVersion) + 1);
	if (requestLine->httpVersion == NULL) {
		char errorMsg[] = "500 (Internal Server Error)";
		send(fildes, errorMsg, strlen(errorMsg), 0);
		free(requestLine->httpVersion);
		return error;
	}

	strcpy(requestLine->httpVersion, httpVersion);

	return 0;
}

int parseRequestLine(int clientSocket, RequestLine *requestLine, char *buffer, size_t size, size_t *bytesRead) {
	char* index = (char*) memmem(buffer, size, crlf, strlen(crlf));
	
	// It will return me to my init state.
	if (index == NULL) {
		return -1;
	}

	// Total Bytes Parsed.
	size_t bytesParsed = (index - buffer) + strlen(crlf);

	// Request-line (METHOD SP REQUESTTARGET SP HTTPVERSION\0).
	size_t sizeReqLine = (index - buffer) + 1;
	char reqL[sizeReqLine];
	memset(reqL, 0, sizeReqLine);
	mystrcpy(reqL, buffer, sizeReqLine);
	
	// Split Version
	char *splitReqLine[3] = {0};
	int getError = splitRequestLineMethod(clientSocket, reqL, splitReqLine, 3);

	// Copy httpVersion before validate it, because strtok_r modifies splirReqLine[2]
	size_t lenHttpVersion = strlen(splitReqLine[2]) + 1;
	char httpVersion[lenHttpVersion];
	strcpy(httpVersion, splitReqLine[2]);

	if (getError == error) {
		return getError;
	}

	// Validate HTTP/Version
	char *splitHtttpVersion[2] = {0};
	getError = splitHttpVersionMethod(clientSocket, splitReqLine[2], splitHtttpVersion, 2);

	if (getError == error) {
		return getError;
	}
	
	// Assign all validated values into requestLine.
	getError = initRequestLine(clientSocket, requestLine, splitReqLine[0], splitReqLine[1], httpVersion);

	if (getError == error) {
		return getError;
	}
	
	*bytesRead += bytesParsed;
	return 0;
}

