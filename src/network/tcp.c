# include "sock.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../linux/unistd.h"
# include "../linux/fcntl.h"
# include "../system/error.h"
# include "../system/io.h"
# include "../system/errno.h"
struct tcp_context {
	int fd;
};

ff_i8_t static
tcp_socket(struct tcp_context *__ctx) {
	if ((__ctx->fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		return FFLY_FAILURE;

	ffly_printf("net socket: %d\n", __ctx->fd);
// remove only needed on serve side
	int val = 1;
	setsockopt(__ctx->fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int));
	return FFLY_SUCCESS;
}

ff_i8_t static
tcp_bind(struct tcp_context *__ctx, struct sockaddr *__addr, socklen_t __len) {
	if (bind(__ctx->fd, __addr, __len) == -1) {
		ffly_printf("tcp; failed to bind.\n");
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}

ff_i8_t static
tcp_listen(struct tcp_context *__ctx) {
	ffly_printf("listen. %d\n", __ctx->fd);
	if (listen(__ctx->fd, 24) == -1) {
		ffly_printf("tcp; failed to listen.\n");
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}

ff_i8_t static
tcp_accept(struct tcp_context *__ctx, struct tcp_context *__ctx0,
	struct sockaddr *__addr, socklen_t *__len)
{
	if ((__ctx0->fd = accept(__ctx->fd, __addr, __len)) == -1) {
		ffly_printf("tcp; failed to accept.\n");
		return FFLY_FAILURE;
	}

	return FFLY_SUCCESS;
}

ff_uint_t static
tcp_send(struct tcp_context *__ctx, void *__buf,
	ff_uint_t __size, int __flags, ff_i8_t *__err)
{
	ff_i32_t res;
	if ((res = send(__ctx->fd, __buf, __size, __flags)) <= 0) {
		ffly_printf("tcp; failed to send.\n");
		*__err = FFLY_FAILURE;
		return 0;
	}
	*__err = FFLY_SUCCESS;
	return res;
}

ff_uint_t static
tcp_recv(struct tcp_context *__ctx, void *__buf,
	ff_uint_t __size, int __flags, ff_i8_t *__err)
{
	ff_i32_t res;
	if ((res = recv(__ctx->fd, __buf, __size, __flags)) <= 0) {
		ffly_printf("tcp; failed to recv.\n");
		*__err = FFLY_FAILURE;
		return 0;
	}
	*__err = FFLY_SUCCESS;
	return res;
}

ff_i8_t static
tcp_close(struct tcp_context *__ctx) {
	close(__ctx->fd);
	return 0;
}

ff_i8_t static
tcp_shutdown(struct tcp_context *__ctx, int __how) {
	shutdown(__ctx->fd, __how);
	return 0;
}

ff_i8_t static
tcp_connect(struct tcp_context *__ctx, struct sockaddr *__addr, socklen_t __len) {
	connect(__ctx->fd, __addr, __len);
	return 0;
}

void static*
ctx_new(void) {
	return __ffly_mem_alloc(sizeof(struct tcp_context));
}

void static
ctx_destroy(void *__ctx) {
	__ffly_mem_free(__ctx);
}

void static
get(ff_u8_t __what, long long __arg, struct tcp_context *__ctx) {
	switch(__what) {
		case 0x00:
			*(int*)__arg = __ctx->fd;
		break;
	}
}

void ffly_tcp_prot(struct ffly_sock_proto *__prot) {
	__prot->ctx_new = ctx_new;
	__prot->ctx_destroy = ctx_destroy;
	__prot->socket = (ff_i8_t(*)(void*))tcp_socket;
	__prot->bind = (ff_i8_t(*)(void*, struct sockaddr*, socklen_t))tcp_bind;
	__prot->send = (ff_uint_t(*)(void*, void*, ff_uint_t, int, ff_i8_t*))tcp_send;
	__prot->recv = (ff_uint_t(*)(void*, void*, ff_uint_t, int, ff_i8_t*))tcp_recv;
	__prot->close = (ff_i8_t(*)(void*))tcp_close;
	__prot->listen = (ff_i8_t(*)(void*))tcp_listen;
	__prot->accept = (ff_i8_t(*)(void*, void*, struct sockaddr*, socklen_t*))tcp_accept;
	__prot->shutdown = (ff_i8_t(*)(void*, int))tcp_shutdown;
	__prot->connect = (ff_i8_t(*)(void*, struct sockaddr*, socklen_t))tcp_connect;
	__prot->get = (void(*)(ff_u8_t, long long, void*))get;
}
