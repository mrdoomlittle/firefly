#include <stdio.h>
#include <stdint.h>

/*
	i cant be bothered to number ports
	soo put list of digits in and get port number out


*/
int main(int __argc, char const *__argv[]) {
	uint16_t v;
	v = 0;

	char const *s = __argv[1];
	char c;
	uint8_t off;
	while((c = *(s++)) != '\0') {
		off = c-'0';
		v ^= 1<<(15-off);	
	}
	printf("%u\n", v);

}
