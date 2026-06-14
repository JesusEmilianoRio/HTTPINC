#pragma once

#include <stddef.h>
#include "../request.h"

int parseRequestHeader(int clientSocket, Request *request, char *buffer, size_t size, size_t *bytesRead);
