#define _GNU_SOURCE 
#include <stdio.h>
#include <string.h>
#include <stddef.h>

#include "../request.h"

char crlf[] = "\r\n";

void getRequestLine(char rqL[], char buffer[],size_t size){
	strncpy(rqL, buffer, size);
	rqL[size] = '\0';
}

void splitRequestLine(char rqL[], char *splitReqL[],size_t size) {
	char *token = strtok(rqL, " ");

	for (size_t i = 0; i < size && token != NULL; i++) {
		splitReqL[i] = token;
		token = strtok(NULL, " ");
	}
}

//Buffer from request, and size from count;
//Size without +1
// Should I return int? what about integer overflow?
int requestLine(RequestLine *requestLine, char buffer[], size_t size) {
	char* index = (char*) memmem(buffer, size, crlf, strlen(crlf));
	
	if (index == NULL) {
		return -1;
	}

	// Total Bytes Parsed.
	size_t bytesParsed = (index - buffer) + strlen(crlf);

	// Request-line.
	size_t sizeReqLine = (index - buffer) + strlen(crlf) + 1;
	char reqL[sizeReqLine];
	getRequestLine(reqL, buffer, bytesParsed);
	
	//Split Version
	


	return bytesParsed;
}

int main(){
	char *buffer = "GET / HTTP/1.1\r\n";
	RequestLine requestLineStruct = {0};

	int x = requestLine(&requestLineStruct, buffer, strlen(buffer));
	printf("%d\n", x);
}

