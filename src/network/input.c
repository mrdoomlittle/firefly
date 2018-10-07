# include "../net.h"
# include "../system/io.h"
# include "../system/err.h"
# include "../dep/mem_cpy.h"
# include "../malloc.h"

struct udp_context {
	FF_SOCKET *sock;
	struct sockaddr *adr;
	socklen_t *len;
};

struct tcp_context {
	FF_SOCKET *sock;
};

void static
udp_recv(struct udp_context *__ctx, void *__seg, int __flags) {
	ff_i8_t err;
	__ctx->sock->prot.recvfrom(__ctx->sock->prot_ctx, __seg, FF_MTU, __flags, __ctx->adr, __ctx->len, &err);
}

void static
tcp_recv(struct tcp_context *__ctx, void *__seg, int __flags) {
	ff_i8_t err;
	__ctx->sock->prot.recv(__ctx->sock->prot_ctx, __seg, FF_MTU, __flags, &err);
}

ff_uint_t static
__in(void(*__func)(void*, void*, int), void *__ctx, int __flags, ff_uint_t __size, void *__buf) {
	ff_u8_t *buf;
	ff_uint_t bsz;

	/*
		TODO:
			allocate pointers to pages of x size,
			and only allocate them when written to.

			buffer size may not match x amount of data comming in
			so theres no point in allocating space unless needed.
	*/
	buf = (ff_u8_t*)malloc(__size+sizeof(FF_NET_HDR));
	ff_u8_t seg[FF_MTU];

	struct ff_net_sins *si = (struct ff_net_sins*)seg;
	ff_u8_t *st = seg+sizeof(struct ff_net_sins);

	FF_NET_HDR *hdr;
	ff_i8_t hh;
	hh = -1;
	ff_uint_t n;
	n = 0;
_again:
	__func(__ctx, seg, __flags);
	ffly_mem_cpy(buf+(si->seg*FF_STC), st, si->len);
	if (!si->seg) {
		hh = 0;
		hdr = (FF_NET_HDR*)buf;
	}

	n+=si->len;
	if (!hh) {
		if (n >= hdr->size) {
			goto _end;
		}
	}
	goto _again;
_end:
	/*
		i dont like this way but i dont see anyother way YET!
	*/
	ffly_mem_cpy(__buf, buf+sizeof(FF_NET_HDR), hdr->size);
	return hdr->size;
}

ff_size_t
ff_net_recvfrom(FF_SOCKET *__sock, struct sockaddr *__addr, socklen_t *__len,
    void *__buf, ff_uint_t __size, int __flags, ff_err_t *__err)
{
	ff_i8_t err;
	struct udp_context ctx = {
		.sock = __sock,
		.adr = __addr,
		.len = __len
	};
	*__err = FFLY_SUCCESS;
	return __in((void(*)(void*, void*, int))udp_recv, (void*)&ctx, __flags, __size, __buf);
}

ff_size_t
ff_net_recv(FF_SOCKET *__sock, void *__buf,
	ff_uint_t __size, int __flags, ff_err_t *__err)
{
	ff_i8_t err;
	struct tcp_context ctx = {
		.sock = __sock
	};
	// to appease the higher levels
	*__err = FFLY_SUCCESS;
	return __in((void(*)(void*, void*, int))tcp_recv, (void*)&ctx, __flags, __size, __buf);
}
