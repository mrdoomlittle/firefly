#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
# include <unistd.h>
# include <errno.h>
# include "../ffint.h"

/*
	ignore..
*/

ff_u16_t cksum(void *__buf, ff_uint_t __len) {
	ff_u64_t sum;
	int nleft;
	nleft = __len;

	ff_u16_t *p;
	p = (ff_u16_t*)__buf;
	sum = 0;
	while(nleft>1) {
		sum+=*(p++);
		nleft-=2;
	}

	if (nleft>0) {
		sum+=*(ff_u8_t*)p;
	}

	sum = (sum>>16)+(sum&0xffff);
	sum+=sum>>16;
	return ~sum;
}

int main() {
	int sock;
	sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sock<0) {
		printf("error, %s\n", strerror(errno));
	}

	int on = 1;
	setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on));

	char buf[256];
	struct ip *i = (struct ip*)buf;
	struct icmp *c = (struct icmp*)(i+1);

	i->ip_v = 4;
	i->ip_hl = 5;
	i->ip_tos = 0;
	i->ip_len = htons(256);
	i->ip_id = htons(0);
	i->ip_off = htons(0);
	i->ip_ttl = 30;
	i->ip_p = IPPROTO_ICMP;
	i->ip_sum = 0;
	inet_aton("192.168.0.46", &i->ip_src);
	inet_aton("192.168.0.111", &i->ip_dst);

	c->icmp_type = ICMP_ECHO;
	c->icmp_code = 0;
	c->icmp_id = 0;
	c->icmp_seq = 0;
	c->icmp_cksum = 0;

	struct sockaddr_in dst;
	bzero(&dst, sizeof(struct sockaddr_in));
	dst.sin_addr.s_addr = inet_addr("192.168.0.111");
	dst.sin_family = AF_INET;

	i->ip_sum = cksum(i, i->ip_hl);
	c->icmp_cksum = cksum(c, 256-sizeof(struct ip));
	socklen_t len;
	len = sizeof(struct sockaddr_in);
	int err;
	err = sendto(sock, buf, 256, 0, (struct sockaddr*)&dst, len);
	if (err<0) {
		printf("error, %s\n", strerror(errno));
	}

	bzero(buf, 256);
	printf("waiting for responce.\n");
	err = recvfrom(sock, buf, 256, 0, (struct sockaddr*)&dst, &len);
	if (err<0) {
		printf("error, %s\n", strerror(errno));
	}
	printf("yes got it!. %s\n", inet_ntoa(i->ip_src));
	close(sock);
}
