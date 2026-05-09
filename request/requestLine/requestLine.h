#pragma once
#include <cstddef>
#include "../request.h"

int requestLine(int clientSocket, RequestLine *requestLine, char buffer[], size_t size);
