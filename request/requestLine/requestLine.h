#pragma once
#include <stddef.h>
#include "../request.h"

int parseRequestLine(int clientSocket, RequestLine *requestLine, char *buffer, size_t size, size_t *bytesRead);
