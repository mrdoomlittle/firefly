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


ff_i8_t static error;
void static
udp_recv(struct udp_context *__ctx, void *__seg, int __flags) {
	__ctx->sock->prot.recvfrom(__ctx->sock->prot_ctx, __seg, FF_MTU, __flags, __ctx->adr, __ctx->len, &error);
}

void static
tcp_recv(struct tcp_context *__ctx, void *__seg, int __flags) {
	__ctx->sock->prot.recv(__ctx->sock->prot_ctx, __seg, FF_MTU, __flags, &error);
}

ff_uint_t static
__in(void(*__func)(void*, void*, int), void *__ctx, int __flags, ff_uint_t __size, void *__buf) {
	ff_u8_t *buf;
	ff_uint_t bsz;
	ff_uint_t rt;
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
	if (error == -1) {
		rt = 0;
		goto _fail;	
	}
	ffly_mem_cpy(buf+(si->seg*FF_STC), st, si->len);
	if (!si->seg) {
		hh = 0;
		hdr = (FF_NET_HDR*)buf;
	}

	n+=si->len;
	if (!hh) {
		if (n >= hdr->size+sizeof(FF_NET_HDR)) {
			goto _end;
		}
	}
	goto _again;
_end:
	/*
		i dont like this way but i dont see anyother way YET!
	*/
	ffly_mem_cpy(__buf, buf+sizeof(FF_NET_HDR), hdr->size);
	rt = hdr->size;
_fail:
	free(buf);
	return rt;
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

	ff_uint_t rcd;
	rcd = __in((void(*)(void*, void*, int))udp_recv, (void*)&ctx, __flags, __size, __buf);
	*__err = error;
	return rcd;
}

ff_size_t
ff_net_recv(FF_SOCKET *__sock, void *__buf,
	ff_uint_t __size, int __flags, ff_err_t *__err)
{
	ff_i8_t err;
	struct tcp_context ctx = {
		.sock = __sock
	};
	ff_uint_t rcd;
	rcd = __in((void(*)(void*, void*, int))tcp_recv, (void*)&ctx, __flags, __size, __buf);
	*__err = error;
	return rcd;
}
