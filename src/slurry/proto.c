# include "proto.h"
#define PKTSHFT 11
#define PKTSIZE (1<<PKTSHFT)
void s_flush(long long);
ff_s32_t s_send(void *__buf, ff_uint_t __size, long long __arg) {
	if (s_io_send(__buf, __size, __arg)<0)
		return -1;
	s_flush(__arg);
/*
	f_u8_t *p;
	ff_uint_t n;
	n = __size>>PKTSHFT;

	p = (ff_u8_t*)__buf;
	ff_uint_t i;
	i = 0;
	while(i != n) {
		s_io_send(p, PKTSIZE, __arg);
		p+=PKTSIZE;
		i++;
	}

	ff_uint_t left;
	left = __size-(n<<PKTSHFT);
	if (left>0) {
		s_io_send(p, left, __arg);
	}
	s_flush(__arg);
*/
	return __size;
}

ff_s32_t s_recv(void *__buf, ff_uint_t __size, long long __arg) {
	if (s_io_recv(__buf, __size, __arg)<0)
		return -1;
	return __size;
/*
	ff_u8_t *p;
	ff_uint_t n;
	n = __size>>PKTSHFT;

	p = (ff_u8_t*)__buf;
	ff_uint_t i;
	i = 0;
	while(i != n) {
		s_io_recv(p, PKTSIZE, __arg);
		p+=PKTSIZE;
		i++;
	}

	ff_uint_t left;
	left = __size-(n<<PKTSHFT);
	if (left>0) {
		s_io_recv(p, left, __arg);
	}
*/
}
