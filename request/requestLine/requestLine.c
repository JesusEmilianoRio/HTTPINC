#define _GNU_SOURCE 
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

#include "../request.h"

extern int errno;

char crlf[] = "\r\n";
char *sp = " ";

void getRequestLine(char rqL[], char buffer[], size_t size){
	strncpy(rqL, buffer, size);
	rqL[size] = '\0';
}

void splitRequestLine(char rqL[], char *splitReqL[],size_t size) {
	// What if there is no 3 lines. I need to make it malloc
	// so I can append dynamic values.
	char *token = strtok(rqL, sp);
	size_t counter = 0;

	for (size_t i = 0; i < size && token != NULL; i++) {
		splitReqL[i] = token;
		token = strtok(NULL, sp);
		counter++;
	}

	if (counter != size || token != NULL) {
		printf("Error msg: %s\n", strerror(errno));
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

	// Request-line (METHOD SP REQUESTTARGET SP HTTPVERSION\0).
	size_t sizeReqLine = (index - buffer) + 1;
	char reqL[sizeReqLine];
	memset(reqL, 0, sizeReqLine);
	getRequestLine(reqL, buffer, sizeReqLine-1);
	
	//Split Version
	


	return bytesParsed;
}

int main(){
	char *buffer = "GET / HTTP/1.1\r\n";
	RequestLine requestLineStruct = {0};

	int x = requestLine(&requestLineStruct, buffer, strlen(buffer));
	printf("%d\n", x);
}

