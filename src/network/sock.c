# include "sock.h"
# include "../system/errno.h"
# include "../system/io.h"
# include "../system/err.h"
# include "../linux/unistd.h"
ff_err_t ffly_socket(struct ffly_socket *__sock) {
	__sock->prot_ctx = __sock->prot.ctx_new();
	
    __sock->prot.socket(__sock->prot_ctx);
    return FFLY_SUCCESS;
}

int ffly_sock_shutdown(struct ffly_socket *__sock, int __how) {
	__sock->prot.shutdown(__sock->prot_ctx, __how);
	return 0;
}

ff_int_t ffly_sock_accept(struct ffly_socket *__sock, struct ffly_socket *__sock0,
	struct sockaddr *__addr, socklen_t *__len, ff_err_t *__err)
{
	__sock0->prot = __sock->prot;
	__sock0->prot_ctx = __sock->prot.ctx_new();
	__sock->prot.accept(__sock->prot_ctx, __sock0->prot_ctx, __addr, __len);
    return FFLY_SUCCESS;
}

ff_err_t ffly_sock_listen(struct ffly_socket *__sock) {
    __sock->prot.listen(__sock->prot_ctx);
    return FFLY_SUCCESS;
}

ff_err_t ffly_sock_bind(struct ffly_socket *__sock, struct sockaddr *__addr, socklen_t __len) {
 	__sock->prot.bind(__sock->prot_ctx, __addr, __len);
    return FFLY_SUCCESS;
}

ff_err_t ffly_sock_connect(struct ffly_socket *__sock, struct sockaddr *__addr, socklen_t __len) {
	__sock->prot.connect(__sock->prot_ctx, __addr, __len);
    return FFLY_SUCCESS;
}

ff_size_t ffly_sock_send(struct ffly_socket *__sock, void const *__buf, ff_size_t __size, int __flags, ff_err_t *__err) {
	ff_i8_t err;
	__sock->prot.send(__sock->prot_ctx, __buf, __size, __flags, &err);
	*__err = FFLY_SUCCESS;
    return __size;
}

ff_size_t ffly_sock_recv(struct ffly_socket *__sock, void *__buf, ff_size_t __size, int __flags, ff_err_t *__err) {
	ff_i8_t err;
	__sock->prot.recv(__sock->prot_ctx, __buf, __size, __flags, &err);
	*__err = FFLY_SUCCESS;
    return __size;
}

ff_size_t ffly_sock_sendto(struct ffly_socket *__sock, void const *__buf, ff_size_t __size, int __flags, struct sockaddr *__addr, socklen_t __len, ff_err_t *__err) {
	ff_i8_t err;
	__sock->prot.sendto(__sock->prot_ctx, __buf, __size, __flags, __addr, __len, &err);
	*__err = FFLY_SUCCESS;
    return __size;
}

ff_size_t ffly_sock_recvfrom(struct ffly_socket *__sock, void *__buf, ff_size_t __size, int __flags, struct sockaddr *__addr, socklen_t *__len, ff_err_t *__err) {
	ff_i8_t err;
	__sock->prot.recvfrom(__sock->prot_ctx, __buf, __size, __flags, __addr, __len, &err);
	*__err = FFLY_SUCCESS;
    return __size;
}

ff_err_t ffly_sock_close(struct ffly_socket *__sock) {
    __sock->prot.close(__sock->prot_ctx);
    return FFLY_SUCCESS;
}
