# include "net.h"
# include "system/io.h"
# include "dep/str_len.h"
# include "dep/bzero.h"
ff_err_t ffmain(int __argc, char const *__argv[]) {

	ff_err_t err;
	struct sockaddr_in addr;
	ffly_bzero(&addr, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htons(INADDR_ANY);
	addr.sin_port = htons(21299);
	FF_SOCKET *sock = ff_net_creat(&err, _NET_PROT_TCP);
	ff_net_bind(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
	ff_net_listen(sock);
	FF_SOCKET *peer = ff_net_accept(sock, NULL, NULL, &err);

	char const *s = "hhh\n";
	ff_net_send(peer, s, ffly_str_len(s)+1, 0, &err);
	ff_net_close(peer);
	ff_net_close(sock);

/*
	ff_err_t err;
	FF_SOCKET *sock = ff_net_creat(&err, AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in addr;
	ffly_bzero(&addr, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htons(INADDR_ANY);
	addr.sin_port = htons(21299);
	ff_net_bind(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)); 

	struct sockaddr from;
	ffly_bzero(&from, sizeof(struct sockaddr));
	socklen_t len;

	char buf[200];
	ff_net_recvfrom(sock, (struct sockaddr*)&from, &len, buf, 200, &err);
	ffly_printf("%s\n", buf);

	ff_net_close(sock);
*/
}
