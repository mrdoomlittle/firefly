# include "net.h"
# include "system/io.h"
# include "dep/bzero.h"
# include "dep/str_len.h"
# include "dep/str_cpy.h"
# include "system/nanosleep.h"
ff_err_t ffmain(int __argc, char const *__argv[]) {
	ff_err_t err;
	FF_SOCKET *sock = ff_net_creat(&err, _NET_PROT_TCP);
	struct sockaddr_in addr;
	ffly_bzero(&addr, sizeof(struct sockaddr_in));
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(21299);
	ff_net_connect(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr));

	char s[2048];
	ffly_bzero(s, 2048);
	ff_net_recv(sock, s, 4, 0, &err); 
	ffly_printf("%s\n", s);
	ff_net_close(sock);

/*
	ff_err_t err;
	FF_SOCKET *sock = ff_net_creat(&err, AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in to;
	ffly_bzero(&to, sizeof(struct sockaddr_in));
	socklen_t len = sizeof(struct sockaddr_in);

	to.sin_addr.s_addr = inet_addr("127.0.0.1");
	to.sin_family = AF_INET;
	to.sin_port = htons(21299);

	char buf[200];
	ffly_bzero(buf, 200);
	ffly_str_cpy(buf, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
	ff_net_sendto(sock, (struct sockaddr*)&to, len, (void*)buf, 200, &err);
	ffly_nanosleep(1, 0);
	
	ff_net_close(sock);
*/
}
