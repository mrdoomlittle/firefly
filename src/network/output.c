# include "../ffint.h"
# include "../types.h"
# include "../net.h"
# include "../system/io.h"
# include "../system/err.h"
# include "../dep/mem_cpy.h"
# include "../malloc.h"
# include "../linux/time.h"
# include "../tc.h"
struct udp_context {
	FF_SOCKET *sock;
	struct sockaddr *adr;
	socklen_t len;
};

struct tcp_context {
	FF_SOCKET *sock;
};

struct context {
	ff_u8_t seg[FF_MTU];
	struct ff_net_sins *si;
	ff_uint_t sd;
	ff_u8_t *st;
	ff_uint_t sn;
};

ff_i8_t static error;
void static
udp_send(struct udp_context *__ctx, void *__seg, int __flags) {
	__ctx->sock->prot.sendto(__ctx->sock->prot_ctx, __seg, FF_MTU, __flags, __ctx->adr, __ctx->len, &error);
}

void static
tcp_send(struct tcp_context *__ctx, void *__seg, int __flags) {
	__ctx->sock->prot.send(__ctx->sock->prot_ctx, __seg, FF_MTU, __flags, &error);
}

void static
__out(void(*__func)(void*, void*, int), void *__ctx0, int __flags, ff_uint_t __size, void *__buf, struct context *__ctx) {
	ff_i8_t err;
	ff_uint_t ri;
	ff_uint_t n;
	n = __size;
	ff_u8_t *bufp;

	bufp = (ff_u8_t*)__buf;
	struct ff_net_sins *si;

	si = __ctx->si;
_again:
	ri = FF_STC-__ctx->sd;
	if (ri<=n) {
		ffly_mem_cpy(__ctx->st+__ctx->sd, bufp, ri);
		__ctx->sd = 0;
		si->seg = __ctx->sn++;
		si->len = FF_STC;
		__func(__ctx0, __ctx->seg, __flags);
		if (error == -1) {
			return;
		}
		n-=ri;
		bufp+=ri;
		goto _again;
	}

	if (n>0) {
		ffly_mem_cpy(__ctx->st+__ctx->sd, bufp, n);
		__ctx->sd+=__size;
	}
}

void static
__bleed(void(*__func)(void*, void*, int), void *__ctx0, int __flags, struct context *__ctx) {
	__ctx->si->seg = __ctx->sn;
	__ctx->si->len = __ctx->sd;
	__func(__ctx0, __ctx->seg, __flags);
}
// function cast
#define FC (void(*)(void*, void*, int))
#define UDP (FC udp_send)
#define TCP (FC tcp_send)

ff_size_t
ff_net_sendto(FF_SOCKET *__sock, struct sockaddr *__addr, socklen_t __len,
	void const *__buf, ff_uint_t __size, int __flags, ff_err_t *__err)
{
	ff_i8_t err;
	// TODO
}

ff_size_t
ff_net_send(FF_SOCKET *__sock, void const *__buf,
	ff_uint_t __size, int __flags, ff_err_t *__err)
{
	ff_i8_t err;
	FF_NET_HDR hdr = {
		.size = __size
	};

	struct tc_spec t;
	ff_tc_gettime(&t);
	hdr.ts.tv_sec = t.sec;
	hdr.ts.tv_nsec = t.nsec;
	ffly_printf("sec %u\n", t.sec);

	struct context ctx;
	ctx.si = (struct ff_net_sins*)ctx.seg;
	ctx.st = ctx.seg+sizeof(struct ff_net_sins);
	ctx.sd = 0;
	ctx.sn = 0;

	struct tcp_context ctx0 = {
		.sock = __sock
	};

	__out(TCP, (void*)&ctx0, __flags, sizeof(FF_NET_HDR), &hdr, &ctx);
	if (error == -1) {
		*__err = FFLY_FAILURE;
		return;
	}

	__out(TCP, (void*)&ctx0, __flags, __size, __buf, &ctx);
	if (error == -1) {
		*__err = FFLY_FAILURE;
		return;
	}

	if (ctx.sd>0) {
		__bleed(TCP, (void*)&ctx0, __flags, &ctx);
		if (error == -1) {
			*__err = FFLY_FAILURE;
			return;
		}
	}
	*__err = FFLY_SUCCESS;
}
