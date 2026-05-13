#define _GNU_SOURCE 
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <stddef.h>

#include "../request.h"


char crlf[] = "\r\n";
char *sp = " ";

void getRequestLine(char rqL[], char buffer[], size_t size){
	strncpy(rqL, buffer, size);
	rqL[size] = '\0';
}

void splitRequestLineMethod(int clientSocket, char rqL[], char *splitReqL[], size_t size) {
	char *token = strtok(rqL, sp);
	size_t counter = 0;

	for (size_t i = 0; i < size && token != NULL; i++) {
		splitReqL[i] = token;
		token = strtok(NULL, sp);
		counter++;
	}

	if (counter != size || token != NULL) {
		char errorMSG[] = "400 (Bad Request)";
		send(clientSocket, errorMSG, sizeof(errorMSG)/sizeof(errorMSG[0]), 0);
	}
}

void splitHttpVersionMethod(int clientSocket, char httpVersion[], char *splitHttpVersion[], size_t size) {
	char *token = strtok(httpVersion, "/");
	size_t counter = 0;

	for (size_t i = 0; i < size && token != NULL; i++) {
		splitHttpVersion[i] = token;
		token = strtok(NULL, "/");
		counter++;
	}

	if (counter != size || token != NULL) {
		char errorMSG[] = "400 (Bad Request)";
		send(clientSocket, errorMSG, sizeof(errorMSG)/sizeof(errorMSG[0]), 0);
	}

	char *validateHttpVersion[] = {"HTTP", "1.1"};
	int stringCompare = strcmp(splitHttpVersion[0], validateHttpVersion[0]);

	if (stringCompare != 0) {
		char errorMSG[] = "400 (Bad Request)";
		send(clientSocket, errorMSG, sizeof(errorMSG)/sizeof(errorMSG[0]), 0);
	}

	stringCompare = strcmp(splitHttpVersion[1], validateHttpVersion[1]);

	if (stringCompare != 0) {
		char errorMSG[] = "505 (HTTP Version Not Supported)";
		send(clientSocket, errorMSG, sizeof(errorMSG)/sizeof(errorMSG[0]), 0);
	}
}

//Buffer from request, and size from count;
//Size without +1
// Should I return int? what about integer overflow?
int parseRequestLine(int clientSocket, RequestLine *requestLine, char buffer[], size_t size) {
	char* index = (char*) memmem(buffer, size, crlf, strlen(crlf));
	
	if (index == NULL) {
		return -1;
	}

	// Total Bytes Parsed.
	size_t bytesParsed = (index - buffer) + strlen(crlf);

	// Request-line (METHOD SP REQUESTTARGET SP HTTPVERSION\0).
	size_t sizeReqLine = (index - buffer) + 1;
	char reqL[sizeReqLine];
	memset(reqL, 0, sizeReqLine);
	getRequestLine(reqL, buffer, sizeReqLine-1);
	
	//Split Version
	char *splitReqLine[3] = {0};
	splitRequestLineMethod(clientSocket, reqL, splitReqLine, 3);

	// Validate HTTP/Version
	char *splitHtttpVersion[2] = {0};
	splitHttpVersionMethod(clientSocket, splitReqLine[2], splitHtttpVersion, 2);
	
	//Add all values to RequestLine
	requestLine->method = splitReqLine[0];
	requestLine->requestTarget = splitReqLine[1];
	requestLine->method = splitReqLine[2];
	return bytesParsed;
}

