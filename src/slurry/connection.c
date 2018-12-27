# include "connection.h"
# ifndef __fflib
# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <malloc.h>
#else
# include "../linux/types.h"
# include "../linux/socket.h"
# include "../inet.h"
# include "../linux/unistd.h"
# include "../stdio.h"
# include "../string.h"
# include "../malloc.h"
# include "../linux/net.h"
# include "../linux/in.h"
# endif
s_connp s_open(void) {
	s_connp con;

	con = (s_connp)malloc(sizeof(struct s_conn));
	if ((con->sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("failed to create socket.\n");
	}

	return con;
}

void s_connect(s_connp __conn, ff_u16_t __port, char const *__addr) {
	bzero(&__conn->addr, sizeof(struct sockaddr_in));
	__conn->addr.sin_addr.s_addr = inet_addr(__addr);

	__conn->addr.sin_family = AF_INET;
	__conn->addr.sin_port = htons(__port);

	if (connect(__conn->sock, (struct sockaddr*)&__conn->addr, sizeof(struct sockaddr_in)) == -1) {
		printf("failed to connect.\n");
	}
}

void s_close(s_connp __conn) {
	shutdown(__conn->sock, SHUT_RDWR);
	close(__conn->sock);
}
