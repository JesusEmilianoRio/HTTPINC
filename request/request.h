#pragma once

//Init Request Line
typedef struct _requestLine {
	char *method;
	char *requestTarget;
	char *httpVersion;
} RequestLine;

// Init State
typedef enum {
	STATE_INIT,
	STATE_REQUESTHEADER,
	STATE_REQUESTBODY,
	STATE_DONE,
	STATE_ERROR,
} State;

//Init Request
typedef struct _request {
	RequestLine requestLine;
	State state;
} Request;

char *request(int fildes);
