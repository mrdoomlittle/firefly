# include "proto.h"
#define PKTSHFT 14
#define PKTSIZE (1<<PKTSHFT)
/*
	TODO:
		use iovec
*/
void s_send(void *__buf, ff_uint_t __size, long long __arg) {
	ff_u8_t *p;
	ff_uint_t n;
	n = __size>>PKTSHFT;

/*
	for now till fix
*/
	ff_u8_t ack;

	p = (ff_u8_t*)__buf;
	ff_uint_t i;
	i = 0;
	while(i != n) {
		s_io_send(p, PKTSIZE, __arg);
		s_io_recv(&ack, 1, __arg);
		p+=PKTSIZE;
		i++;
	}

	ff_uint_t left;
	left = __size-(n*PKTSIZE);
	if (left>0) {
		s_io_send(p, left, __arg);
		s_io_recv(&ack, 1, __arg);
	}
}

void s_recv(void *__buf, ff_uint_t __size, long long __arg) {
	ff_u8_t *p;
	ff_uint_t n;
	n = __size>>PKTSHFT;

	ff_u8_t ack;

	p = (ff_u8_t*)__buf;
	ff_uint_t i;
	i = 0;
	while(i != n) {
		s_io_recv(p, PKTSIZE, __arg);
		s_io_send(&ack, 1, __arg);
		p+=PKTSIZE;
		i++;
	}

	ff_uint_t left;
	left = __size-(n*PKTSIZE);
	if (left>0) {
		s_io_recv(p, left, __arg);
		s_io_send(&ack, 1, __arg);
	}
}
