# include "br.h"
# include <linux/limits.h>
# include "dep/str_cpy.h"
# include "dep/bzero.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/error.h"
# include "linux/fcntl.h"
# include "linux/unistd.h"
# include "linux/stat.h"
# include "system/io.h"
# include "inet.h"

void ffly_br_put(ffly_brp __br, ff_u32_t __b, ff_uint_t __off) {
	*(__br->bricks+__off) = __b;
}

void ffly_br_prep(ffly_brp __br, ff_u8_t __bsz, ff_uint_t __n) {
	__br->bricksize = __bsz;
	__br->bricks = (ff_u32_t*)__ffly_mem_alloc(__n*sizeof(ff_u32_t));
	__br->brick_c = __n;
}

/*
	NOTE:
		only a sketch
*/

// for testing
void ffly_br_retrieve(ff_u8_t __bsz, ff_uint_t __n, void *__buf) {
	ff_err_t err;
	FF_SOCKET *sock = ff_net_creat(&err, _NET_PROT_TCP);
	struct sockaddr_in addr;
	ffly_bzero(&addr, sizeof(struct sockaddr_in));
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	addr.sin_family = AF_INET;
	addr.sin_port = htons(21299);

	ff_net_connect(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));

	ff_net_send(sock, &__n, sizeof(ff_uint_t), 0, &err);
	if (_err(err)) {
		ffly_printf("failed to send.\n");
	}

	ff_uint_t i = 0;

	ff_uint_t bsz = 1<<__bsz;
	while(i != __n) {
		/*
			TODO:
				replace, get brick number from list
		*/
		ff_net_send(sock, &i, sizeof(ff_uint_t), 0, &err);
		ff_net_recv(sock, (ff_u8_t*)__buf+(i*bsz), bsz, 0, &err);
		i++;
	}

	ff_net_close(sock);
}

void ffly_br_free(ffly_brp __br) {
	__ffly_mem_free(__br->bricks);
}

void ffly_br_open(ffly_brp __br) {
	ff_err_t err;
	struct sockaddr_in addr;
	ffly_bzero(&addr, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htons(INADDR_ANY);
	addr.sin_port = htons(21299);
	FF_SOCKET *sock;

	sock = ff_net_creat(&err, _NET_PROT_TCP);
	if (_err(err))
		return;

	if (_err(err = ff_net_bind(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)))) {
		return;
	}
	__br->sock = sock;
}

void ffly_br_close(ffly_brp __br) {
	ff_net_close(__br->sock);
}

ff_i8_t static shutd = -1;
void ffly_br_shutdown(ffly_brp __br) {
	shutd = 0;
	ff_net_shutdown(__br->sock, SHUT_RDWR);
}

void ffly_br_start(ffly_brp __br) {
	struct sockaddr_in cl;

	ff_err_t err;
	socklen_t len;

	FF_SOCKET *peer;

	FF_SOCKET *sock = __br->sock;

	ff_uint_t n, b, bsz;
_again:
	ffly_bzero(&cl, sizeof(struct sockaddr_in));
	len = sizeof(struct sockaddr_in);
	if (_err(err = ff_net_listen(sock))) {
		return;
	}

	peer = ff_net_accept(sock, (struct sockaddr*)&cl, &len, &err);
	if (_err(err))	
		return;

	ff_net_recv(peer, &n, sizeof(ff_uint_t), 0, &err);
	ffly_printf("client requested %u bricks.\n", n);

	bsz = 1<<__br->bricksize;
	void *p;
	while(n>0) {
/*
		might be better to use a more flexible method later
*/
		ff_net_recv(peer, &b, sizeof(ff_uint_t), 0, &err);
		ffly_printf("sending out brick %u\n", b);
		if (b>__br->brick_c) {
			ffly_printf("error.\n");
			break;
		}
		p = ffly_brick_get(*(__br->bricks+b));
		if (!p) {
			ffly_printf("error.\n");
			break;
		}
		ff_net_send(peer, p, bsz, 0, &err);
		n--;
	}

	ff_net_close(peer);
	goto _again;
}
