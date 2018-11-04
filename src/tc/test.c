# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <time.h>
# include <sys/ioctl.h>
# include "../tc.h"
# include "math.h"
# include <signal.h>
ff_i8_t run = -1;
void sig(int __sig) {
	run = 0;
}
int main() {
	struct sigaction sa;
	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = sig;
	sigaction(SIGINT, &sa, NULL); 

	int sock;
	sock = socket(AF_INET, SOCK_DGRAM, 0);

	socklen_t len;
	len = sizeof(struct sockaddr_in);
	struct sockaddr_in adr;
	adr.sin_addr.s_addr = inet_addr("192.168.0.111");
	adr.sin_family = AF_INET;
	adr.sin_port = htons(33216);
	ff_i8_t ack;
	struct timeval tv;
	struct tc_spec t;
_again:
	sendto(sock, &ack, 1, 0, (struct sockaddr*)&adr, len);
	recvfrom(sock, &t, sizeof(struct tc_spec), 0, (struct sockaddr*)&adr, &len);
	int err = ioctl(sock, SIOCGSTAMP, &tv);
	if (err<0) {
		printf("error.\n");
	}

	float v;
	v = t.sec;
	float d, h, m, s;
	d = v/86400.;
	v-=floor(d)*86400.;
	h = v/3600.;
	v-=floor(h)*3600.;
	m = v/60.;
	v-=floor(m)*60;
	s = v;

	printf("\e[2Jcurrent time: %u-%u-%u-%u or %u\n", (ff_u64_t)d, (ff_u64_t)h, (ff_u64_t)m, (ff_u64_t)s, t.sec);
	if (run == -1) {
		usleep(10000);
		goto _again;
	}
	printf("goodbye.\n");
	close(sock);
}
