#pragma once

#include <stddef.h>
#include "../request.h"

int parseRequestHeader(int clientSocket, HashTable *request, char *buffer, size_t size, size_t *bytesRead);
