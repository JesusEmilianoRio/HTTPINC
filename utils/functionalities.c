#include <stddef.h>
#include <unistd.h>

size_t mystrcat(size_t pointer, char *dest, char *src) {
	while(*src) {
		*(dest + pointer) = *src;
		pointer++;
		src++;
	}

	return pointer;
}

