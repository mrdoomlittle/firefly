# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <time.h>
# include <sys/ioctl.h>
# include "../tc.h"
int main() {
	int sock;
	sock = socket(AF_INET, SOCK_DGRAM, 0);

	socklen_t len;
	len = sizeof(struct sockaddr_in);
	struct sockaddr_in adr;
	adr.sin_addr.s_addr = inet_addr("192.168.0.111");
	adr.sin_family = AF_INET;
	adr.sin_port = htons(33216);
	ff_i8_t ack;

	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	struct tc_spec t;
	sendto(sock, &ack, 1, 0, (struct sockaddr*)&adr, len);
	recvfrom(sock, &t, sizeof(struct tc_spec), 0, (struct sockaddr*)&adr, &len);
	struct timeval tv;
	int err = ioctl(sock, SIOCGSTAMP, &tv);
	if (err<0) {
		printf("error.\n");
	}
	ff_u64_t h, m, s;
	h = t.sec/3600;
	m = t.sec/60;
	s = t.sec;

    printf("time: %u-hour : %u-min %u-sec : %u\n", h, m, s, t.nsec);
//	printf("sec: %u, nsec: %u, %d:%f, %u\n", t.sec, t.nsec, tv.tv_sec-ts.tv_sec, ((float)(tv.tv_usec-(ts.tv_nsec/1000)))/1000000.0, tv.tv_sec);

	close(sock);
}
