# include "sock.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../linux/unistd.h"
# include "../linux/fcntl.h"
struct udp_context {
	int fd;
};

ff_i8_t static
udp_socket(struct udp_context *__ctx) {
	__ctx->fd = socket(AF_INET, SOCK_DGRAM, 0);

	int val = 1;
	setsockopt(__ctx->fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int));
	return 0;
}

ff_i8_t static
udp_bind(struct udp_context *__ctx, struct sockaddr *__addr, socklen_t __len) {
	bind(__ctx->fd, __addr, __len);
	return 0;
}

ff_uint_t static
udp_sendto(struct udp_context *__ctx, void *__buf, ff_uint_t __size, int __flags, struct sockaddr *__dst, socklen_t __len, ff_i8_t *__err) {
	sendto(__ctx->fd, __buf, __size, __flags, __dst, __len);
	return 0;
}

ff_uint_t static
udp_recvfrom(struct udp_context *__ctx, void *__buf, ff_uint_t __size, int __flags, struct sockaddr *__src, socklen_t *__len, ff_i8_t *__err) {
	recvfrom(__ctx->fd, __buf, __size, __flags, __src, __len);
	return 0;
}

ff_i8_t static
udp_close(struct udp_context *__ctx) {
	close(__ctx->fd);
	return 0;
}

ff_i8_t static
udp_shutdown(struct udp_context *__ctx, int __how) {
	shutdown(__ctx->fd, __how);
	return 0;
}

void static*
ctx_new(void) {
	return __ffly_mem_alloc(sizeof(struct udp_context));
}

void static
ctx_destroy(void *__ctx) {
	__ffly_mem_free(__ctx);
}

void ffly_udp_prot(struct ffly_sock_proto *__prot) {
	__prot->ctx_new = ctx_new;
	__prot->ctx_destroy = ctx_destroy;
	__prot->socket = (ff_i8_t(*)(void*))udp_socket;
	__prot->bind = (ff_i8_t(*)(void*, struct sockaddr*, socklen_t))udp_bind;
	__prot->sendto = (ff_uint_t(*)(void*, void*, ff_uint_t, int, struct sockaddr*, socklen_t, ff_i8_t*))udp_sendto;
	__prot->recvfrom = (ff_uint_t(*)(void*, void*, ff_uint_t, int, struct sockaddr*, socklen_t*, ff_i8_t*))udp_recvfrom;
	__prot->close = (ff_i8_t(*)(void*))udp_close;
	__prot->shutdown = (ff_i8_t(*)(void*, int))udp_shutdown;
}
