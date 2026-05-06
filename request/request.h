#pragma once

typedef struct _requestLine {
	char *method;
	char *requestTarget;
	char *httpVersion;
} RequestLine;

char *request(int fildes);
