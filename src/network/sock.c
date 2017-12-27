# include "sock.h"
# include "../system/errno.h"
# include "../system/io.h"
# include "../system/err.h"
ffly_err_t ffly_socket(struct ffly_socket *__sock, int __family, int __type, int __proto) {
    if ((__sock->fd = socket(__family, __type, __proto)) == -1) {
        ffly_fprintf(ffly_err, "failed to create socket.\n");
        return FFLY_FAILURE;
    }
    return FFLY_SUCCESS;
}

ffly_err_t ffly_sock_bind(struct ffly_socket *__sock, struct sockaddr *__addr, socklen_t __len) {
    if (bind(__sock->fd, __addr, __len) == -1) {
        ffly_fprintf(ffly_err, "failed to bind.\n");
        return FFLY_FAILURE;
    }
    return FFLY_SUCCESS;
}

ffly_err_t ffly_sock_connect(struct ffly_socket *__sock, struct sockaddr *__addr, socklen_t __len) {
    if (connect(__sock->fd, __addr, __len) == -1) {
        ffly_fprintf(ffly_err, "failed to connect.\n");
        return FFLY_FAILURE;
    }
    return FFLY_SUCCESS;
}

ffly_size_t ffly_sock_send(struct ffly_socket *__sock, void *__buf, ffly_size_t __size, int __flags, ffly_err_t *__err) {
    ssize_t res;
    if ((res = send(__sock->fd, __buf, __size, __flags)) == -1) {
        ffly_fprintf(ffly_err, "failed to send.\n");
        *__err = FFLY_FAILURE;
        return 0;
    }
    *__err = FFLY_SUCCESS;
    return res;
}

ffly_size_t ffly_sock_recv(struct ffly_socket *__sock, void *__buf, ffly_size_t __size, int __flags, ffly_err_t *__err) {
    ssize_t res;
    if ((res = recv(__sock->fd, __buf, __size, __flags)) == -1) {
        ffly_fprintf(ffly_err, "failed to recv.\n");
        *__err = FFLY_FAILURE;
        return 0;
    }
    *__err = FFLY_SUCCESS;
    return res;
}

ffly_size_t ffly_sock_sendto(struct ffly_socket *__sock, void *__buf, ffly_size_t __size, int __flags, struct sockaddr *__addr, socklen_t __len, ffly_err_t *__err) {
    ssize_t res;
    if ((res = sendto(__sock->fd, __buf, __size, __flags, __addr, __len)) == -1) {
        ffly_fprintf(ffly_err, "failed to send.\n");
        *__err = FFLY_FAILURE;
        return 0;
    }
    *__err = FFLY_SUCCESS;
    return res;
}

ffly_size_t ffly_sock_recvfrom(struct ffly_socket *__sock, void *__buf, ffly_size_t __size, int __flags, struct sockaddr *__addr, socklen_t *__len, ffly_err_t *__err) {
    ssize_t res;
    if ((res = recvfrom(__sock->fd, __buf, __size, __flags, __addr, __len)) == -1) {
        ffly_fprintf(ffly_err, "failed to recv.\n");
        *__err = FFLY_FAILURE;
        return 0;
    }
    *__err = FFLY_SUCCESS;
    return res;
}

ffly_err_t ffly_sock_close(struct ffly_socket *__sock) {
    ffly_err_t err;
    if (_err(err = ffly_close(__sock->fd))) {
        ffly_fprintf(ffly_err, "failed to close socket.\n");
        return err;
    }
    return FFLY_SUCCESS;
}
