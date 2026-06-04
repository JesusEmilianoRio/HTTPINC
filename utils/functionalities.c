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

void mystrcpy(char rqL[], char *buffer, size_t size){
	size_t i = 0;

	while (1) {
		if (i == size - 1) {
			rqL[i] = '\0';
			break;
		}
		rqL[i] = buffer[i];
		i++;
	}

}

//void getRequestLine(char *rqL, char *buffer, size_t size){
//	//This returns a value. Fuck. Fuck this shit man.
//	strncpy(rqL, buffer, size);
//	rqL[size] = '\0';
//}
