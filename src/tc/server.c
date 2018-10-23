# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <signal.h>
# include <time.h>
# include "tc.h"
int main() {
	int sock;
	sock = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in adr, cli;
	adr.sin_family = AF_INET;
	adr.sin_addr.s_addr = htons(INADDR_ANY);
	adr.sin_port = htons(33216);
	int val = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int));
	bind(sock, (struct sockaddr*)&adr, sizeof(struct sockaddr_in));

	socklen_t len;
	struct timespec start, now;
	clock_gettime(CLOCK_BOOTTIME, &start);
	struct sockaddr_in dst;
	struct tc_spec out;
_again:
	len = sizeof(struct sockaddr_in);
	ff_i8_t ack;
	recvfrom(sock, &ack, 1, 0, (struct sockaddr*)&dst, &len);
	printf("hello.\n");
	clock_gettime(CLOCK_BOOTTIME, &now);
	out.sec = now.tv_sec-start.tv_sec;
	out.nsec = now.tv_nsec-start.tv_nsec;
	sendto(sock, &out, sizeof(struct tc_spec), 0, (struct sockaddr*)&dst, len);
goto _again;
	close(sock);
}
