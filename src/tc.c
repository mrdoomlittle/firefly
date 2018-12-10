# include "tc.h"
# include "linux/types.h"
# include "linux/socket.h"
# include "inet.h"
# include "linux/unistd.h"
# include "linux/net.h"
# include "linux/in.h"
# include "linux/time.h"
# include "system/io.h"
# include "linux/ioctl.h"
# include "linux/sockios.h"
static struct timespec start;
static struct tc_spec current;
static ff_i8_t init = -1;
// precision 
#define PREC
#define SYNC_X 10
/*
	TODO:
		keep connection??
*/
void static
get(struct tc_spec *__spec) {
	ffly_printf("syncing...\n");
	int sock;
	sock = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in adr;
	adr.sin_addr.s_addr = inet_addr("192.168.0.111");
	adr.sin_family = AF_INET;
	adr.sin_port = htons(33216);
	socklen_t len;

	len = sizeof(struct sockaddr_in);

	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);

	ff_i8_t ack;
	sendto(sock, &ack, 1, 0, (struct sockaddr*)&adr, len);
	recvfrom(sock, __spec, sizeof(struct tc_spec), 0, (struct sockaddr*)&adr, &len);
	struct timeval tv;
	ioctl(sock, SIOCGSTAMP, &tv);

	ff_u64_t s, us;

	s = tv.tv_sec-ts.tv_sec;
	/*
		PREC not defined
		----------------
		24 off from 1000 but does not realy matter

		ping 192.168.0.111 = 0.250ms ish

		here we get 855us ish so 0.855ms

		so we are off about 0.605ms (ISH!)
		as stated above 'does not realy matter'

		PREC defined
		------------
		off about 0.108ms
	*/
	ff_u64_t uu, u, u0;
#ifdef PREC
	uu = ts.tv_nsec/1000;
#else
	//could do more here
	uu = ts.tv_nsec>>10;
	uu+=((uu*24)+(ts.tv_nsec-(uu<<10)))>>10;
#endif
	us = (tv.tv_usec-uu);

	// adjust for latency
	__spec->sec+=s;
	__spec->nsec+=us*1000;
	ffly_printf("network delay: %u-sec, %u-us\n", s, us);
	close(sock);
}

void ff_tc_gettime(struct tc_spec *__spec) {
	if (init == -1) {
		clock_gettime(CLOCK_MONOTONIC, &start);
		get(&current);
		init = 0;
	}
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	/*
		if x sec has passed then sync with server time
	*/
	if (now.tv_sec-start.tv_sec>SYNC_X) {
		struct tc_spec t;
		get(&t);

		// may not be 100% right
		ffly_printf("deviation: %d-sec, %d-ns\n", t.sec-(current.sec+(now.tv_sec-start.tv_sec)),
			t.nsec-(current.nsec+(now.tv_nsec-start.tv_nsec)));
		current = t;
		start = now;
	}

	/*
		+X part should be void if SYNC has taken place before getting to this ^.
	*/
	__spec->sec = current.sec+(now.tv_sec-start.tv_sec);
	__spec->nsec = current.nsec+(now.tv_nsec-start.tv_nsec);
}
