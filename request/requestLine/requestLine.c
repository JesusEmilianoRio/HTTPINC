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

int splitRequestLineMethod(int clientSocket, char rqL[], char *splitReqL[], size_t size) {
	char *spRequest;
	char *token = strtok_r(rqL, sp, &spRequest);
	size_t counter = 0;

	for (size_t i = 0; i < size && token != NULL; i++) {
		splitReqL[i] = token;
		token = strtok_r(NULL, sp, &spRequest);
		counter++;
	}

	// Que error devuelvo?
	// Aqui no puede haber 0.
	if (counter != size || token != NULL) {
		char *errorMSG = "400 (Bad Request)";
		send(clientSocket, errorMSG, strlen(errorMSG), 0);
		return 0;
	}

	// This is some shitty logic.
	// But, that's my current level. I can't complain.
	return 1;
}

void splitHttpVersionMethod(int clientSocket, char *httpVersion, char *splitHttpVersion[], size_t size) {
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
	}

	char *validateHttpVersion[] = {"HTTP", "1.1"};
	int stringCompare = strcmp(splitHttpVersion[0], validateHttpVersion[0]);

	if (stringCompare != 0) {
		char *errorMSG = "400 (Bad Request)";
		send(clientSocket, errorMSG, strlen(errorMSG), 0);
	}

	stringCompare = strcmp(splitHttpVersion[1], validateHttpVersion[1]);

	if (stringCompare != 0) {
		char *errorMSG = "505 (HTTP Version Not Supported)";
		send(clientSocket, errorMSG, strlen(errorMSG), 0);
	}
}

void initRequestLine(RequestLine *requestLine, const char* method, const char* requestTarget, const char* httpVersion) {
	requestLine->method = (char*) malloc(strlen(method) + 1);
	strcpy(requestLine->method, method);
	
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
	// Creo que mi splitReqLine debe ser una memoria en el heap.
	char *splitReqLine[3] = {0};
	int error = splitRequestLineMethod(clientSocket, reqL, splitReqLine, 3);

	if (error == 0) {
		return error;
	}

	// Validate HTTP/Version
	char *splitHtttpVersion[2] = {0};
	splitHttpVersionMethod(clientSocket, splitReqLine[2], splitHtttpVersion, 2);
	
	//Add all values to RequestLine
	// Aqui debo crear una funcion que inicialice mis requestLine y... los pase al heap.
	requestLine->method = splitReqLine[0];
	requestLine->requestTarget = splitReqLine[1];
	requestLine->httpVersion = splitReqLine[2];
	*bytesRead += bytesParsed;
	return 0;
}

