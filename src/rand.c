# include "rand.h"
/*
# include <stdio.h>
void print_bin(ff_u64_t __val) {
	ff_u8_t i = 0;
	while(i != 64) {
		printf("%c", (__val>>i&0x1)?'@':'.');
		i++;
	}
	printf("\n");
}
*/

/*
	for now
*/
ff_u64_t static no = 0x81886ed4;
ff_u8_t ffgen_rand8l() {
	no = no>>60|no<<4;

	ff_u64_t a0, a1, a2, a3, a4, a5, a6;
	a0 = no>>8;
	a1 = no>>16;
	a2 = no>>24;
	a3 = no>>32;
	a4 = no>>40;
	a5 = no>>48;
	a6 = no>>56;

	no += a0^a1^a2^a3^a4^a5^a6;
	return no;
}

ff_u16_t ffgen_rand16l() {
	return (ff_u16_t)ffgen_rand8l()|(ff_u16_t)ffgen_rand8l()<<8;
}

ff_u32_t ffgen_rand32l() {
	return (ff_u32_t)ffgen_rand16l()|(ff_u32_t)ffgen_rand16l()<<16;
}

ff_u64_t ffgen_rand64l() {
	return (ff_u64_t)ffgen_rand32l()|(ff_u64_t)ffgen_rand32l()<<32;
}

ff_u64_t ffly_rand() {
	return ffgen_rand64l();
}

/*
# include <unistd.h>
# include <string.h>
# include <stdlib.h>
int main() {
	ff_u32_t freq[0xff];
	bzero(freq, sizeof(freq));
	ff_u64_t i;
	while(i != 0xfffff) {
		
		//while(i++ != 16-1) {
		//	printf("%u, ", gen_randbyte());
		//}
		//gen_randbyte();
		freq[gen_randbyte()]++;
		//freq[rand()%0xff]++;
		//printf("%u\n", gen_randbyte());
		//usleep(100000);
		i++;
	}

	i = 0;
	while(i != 0xff) {
		printf("%lu: freq, %lu\n", i, freq[i]);
		i++;	
	}
}*/
