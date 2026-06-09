#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

//External Functions
#include "request/request.h"



int main() {
	// Create Socket
	int newSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (newSocket < 0) {
		perror("socket");
		exit(1);
	}

	// Bind Socket
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8080);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	size_t lenAddr = sizeof(addr);

	if (addr.sin_addr.s_addr == 0) {
		perror("port");
		exit(1);
	}

	if (bind(newSocket, (struct sockaddr*) &addr, lenAddr) == -1){
		perror("bind");
		exit(1);
	}

	// After binding socket, listen.
	int listenSocket = listen(newSocket, 3);
	if (listenSocket == -1) {
		perror("listen");
		exit(1);
	}

	printf("Listening on PORT: %hd\n", ntohs(addr.sin_port));

	char* ptr = NULL;
	while (1) {
		// Accept connection.
		int clientSocket = accept(newSocket, (struct sockaddr*) &addr, (socklen_t*)&lenAddr);
		if (clientSocket == -1) {
			perror("Couldn't accept connection.\n");
			continue;
		}

		ptr = request(clientSocket);
		if (ptr != NULL) {
			printf("%s\n", ptr);
		}

		// Close socket
		close(clientSocket);
		free(ptr);
		printf("Connection closed.\n");

	}
	// Close listening.
	close(listenSocket);


	return 0;
}
