# include "rand.h"
//# define testing
# ifdef testing

# include <stdio.h>
void print_bin(ff_u64_t __val) {
	ff_u8_t i = 0;
	while(i != 64) {
		printf("%c", (__val>>(63-i)&0x1)?' ':'#');
//		printf("%u", __val>>(63-i)&0x1);
		i++;
	}
	printf(" - %lu\n", __val);
}
# endif

/*
	for rvw
*/

// not 100% random but will do for now

// random value
ff_u64_t static rv = 0x355A3A4B1C311A4A;
//# define a
//# define v
/*
	TODO:
		make use of hardware random generation

		remove #l bit
*/
ff_u8_t ffgen_rand8l(void) {
	rv = rv>>60|rv<<4;

	ff_u64_t a0, a1, a2, a3, a4, a5, a6, a7;
	a0 = rv;
	a1 = rv>>8;
	a2 = rv>>16;
	a3 = rv>>24;
	a4 = rv>>32;
	a5 = rv>>40;
	a6 = rv>>48;
	a7 = rv>>56;

	ff_u64_t b0, b1, b2, b3, b4, b5, b6;
	b0 = (a0^a1)^((rv&0xff)<<56);
	b1 = (a1^a2)^(((rv&0xffff))<<48);
	b2 = (a2^a3)^(((rv&0xffffff))<<40);
	b3 = (a3^a4)^(((rv&0xffffffff))<<32);
	b4 = (a4^a5)^(((rv&0xffffffffff))<<24);
	b5 = (a5^a6)^(((rv&0xffffffffffff))<<16);
	b6 = (a6^a7)^(((rv&0xffffffffffffff))<<8);

	rv = (b0^b1^b2^b3^b4^b4^b5^b6)+1;
//	print_bin(rv);
	return rv;
}

ff_u16_t ffgen_rand16l(void) {
	return (ff_u16_t)ffgen_rand8l()|(ff_u16_t)ffgen_rand8l()<<8;
}

ff_u32_t ffgen_rand32l(void) {
	return (ff_u32_t)ffgen_rand8l()|(ff_u32_t)ffgen_rand8l()<<8
		|(ff_u32_t)ffgen_rand8l()<<16|(ff_u32_t)ffgen_rand8l()<<24;
}

ff_u64_t ffgen_rand64l(void) {
	return (ff_u64_t)ffgen_rand8l()|(ff_u64_t)ffgen_rand8l()<<8
		|(ff_u64_t)ffgen_rand8l()<<16|(ff_u64_t)ffgen_rand8l()<<24
		|(ff_u64_t)ffgen_rand8l()<<32|(ff_u64_t)ffgen_rand8l()<<40
		|(ff_u64_t)ffgen_rand8l()<<48|(ff_u64_t)ffgen_rand8l()<<56;
}

/*
	TODO:
		add 128, 512, 1024 ...
*/

ff_u64_t ffly_rand() {
	return ffgen_rand64l();
}

# ifdef testing
# include <unistd.h>
# include <string.h>
# include <stdlib.h>
int main() {
/*
	ff_u32_t freq[0xff];
	bzero(freq, sizeof(freq));
	ff_u64_t i;
	while(i != 0xff) {
		freq[ffgen_rand8l()]++;
		i++;
	}

	i = 0;
	while(i != 0xff) {
		printf("%lu: %ld\n", i, freq[i]);
		i++;	
	}
*/
# ifdef a
	while(1) {
		ffgen_rand8l();
		usleep(100000);
	}
# endif
# ifdef v
	ff_u32_t table[0x100];
	bzero(table, sizeof(table));	


	ff_u64_t const n = 0xffffff;
	ff_u8_t start;
	start = ffgen_rand8l();
	ff_u64_t c;
	ff_u64_t i;

	i = 0;
	while(i != n) {
		if (ffgen_rand8l() == start) {
			ff_u32_t *p = table+start;
			*p+=c;
			start = ffgen_rand8l();
			c = 0;
		} else
			c++;
		i++;
	}

	i = 0;
	while(i != 0xff) {
		printf("%u %u : : %f\n", i, table[i], ((float)table[i])/1000);
		usleep(100000);
		i++;
	}
# endif
/*
	ff_uint_t i;
	while(1) {
		i = 0;
		while(i != 200) {
			printf("%c", !(ffgen_rand8l()%2)?'#':' ');
			i++;
		}
		printf("\n");
		usleep(100000);
	}
*/
}
# endif
