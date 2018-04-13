# include "sock.h"
# include "../system/errno.h"
# include "../system/io.h"
# include "../system/err.h"
# include "../linux/unistd.h"
ff_err_t ffly_socket(struct ffly_socket *__sock, int __family, int __type, int __proto) {
    if ((__sock->fd = socket(__family, __type, __proto)) == -1) {
        ffly_fprintf(ffly_err, "failed to create socket.\n");
        return FFLY_FAILURE;
    }
    return FFLY_SUCCESS;
}

int ffly_sock_shutdown(struct ffly_socket *__sock, int __how) {
    return shutdown(__sock->fd, __how);
}

ff_int_t ffly_sock_accept(struct ffly_socket *__sock, struct sockaddr *__addr, socklen_t *__len, ff_err_t *__err) {
    ff_int_t ret;
    if ((ret = accept(__sock->fd, __addr, __len)) == -1) {
        ffly_fprintf(ffly_err, "failed to accept, %s\n", strerror(errno));
        *__err = FFLY_FAILURE;
        return -1;
    }
    return ret;
}

ff_err_t ffly_sock_listen(struct ffly_socket *__sock) {
    if (listen(__sock->fd, 24) == -1) {
		ffly_fprintf(ffly_err, "failed to listen, %s\n", strerror(errno));
		return FFLY_FAILURE;
    }
    return FFLY_SUCCESS;
}

ff_err_t ffly_sock_bind(struct ffly_socket *__sock, struct sockaddr *__addr, socklen_t __len) {
    if (bind(__sock->fd, __addr, __len) == -1) {
        ffly_fprintf(ffly_err, "failed to bind, %s\n", strerror(errno));
        return FFLY_FAILURE;
    }
    return FFLY_SUCCESS;
}

ff_err_t ffly_sock_connect(struct ffly_socket *__sock, struct sockaddr *__addr, socklen_t __len) {
    if (connect(__sock->fd, __addr, __len) == -1) {
        ffly_fprintf(ffly_err, "failed to connect, %s\n", strerror(errno));
        return FFLY_FAILURE;
    }
    return FFLY_SUCCESS;
}

ff_size_t ffly_sock_send(struct ffly_socket *__sock, void const *__buf, ff_size_t __size, int __flags, ff_err_t *__err) {
    ff_ssize_t res;
    if ((res = send(__sock->fd, __buf, __size, __flags)) == -1) {
        ffly_fprintf(ffly_err, "failed to send, %s\n", strerror(errno));
        *__err = FFLY_FAILURE;
        return 0;
    }
    *__err = FFLY_SUCCESS;
    return res;
}

ff_size_t ffly_sock_recv(struct ffly_socket *__sock, void *__buf, ff_size_t __size, int __flags, ff_err_t *__err) {
    ff_ssize_t res;
    if ((res = recv(__sock->fd, __buf, __size, __flags)) == -1) {
        ffly_fprintf(ffly_err, "failed to recv, %s\n", strerror(errno));
        *__err = FFLY_FAILURE;
        return 0;
    }
    *__err = FFLY_SUCCESS;
    return res;
}

ff_size_t ffly_sock_sendto(struct ffly_socket *__sock, void const *__buf, ff_size_t __size, int __flags, struct sockaddr *__addr, socklen_t __len, ff_err_t *__err) {
    ff_ssize_t res;
    if ((res = sendto(__sock->fd, __buf, __size, __flags, __addr, __len)) == -1) {
        ffly_fprintf(ffly_err, "failed to send, %s\n", strerror(errno));
        *__err = FFLY_FAILURE;
        return 0;
    }
    *__err = FFLY_SUCCESS;
    return res;
}

ff_size_t ffly_sock_recvfrom(struct ffly_socket *__sock, void *__buf, ff_size_t __size, int __flags, struct sockaddr *__addr, socklen_t *__len, ff_err_t *__err) {
    ff_ssize_t res;
    if ((res = recvfrom(__sock->fd, __buf, __size, __flags, __addr, __len)) == -1) {
        ffly_fprintf(ffly_err, "failed to recv,\n", strerror(errno));
        *__err = FFLY_FAILURE;
        return 0;
    }
    *__err = FFLY_SUCCESS;
    return res;
}

ff_err_t ffly_sock_close(struct ffly_socket *__sock) {
    ff_err_t err;
    if (_err(err = close(__sock->fd))) {
        ffly_fprintf(ffly_err, "failed to close socket.\n");
        return err;
    }
    return FFLY_SUCCESS;
}
