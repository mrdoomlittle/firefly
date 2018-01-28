# include "net.h"
# include "system/errno.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/err.h"
# include "system/io.h"
FF_SOCKET* ff_net_creat(ffly_err_t *__err, int __family, int __type, int __proto) {
    *__err = FFLY_SUCCESS;
    FF_SOCKET* sock = (FF_SOCKET*)__ffly_mem_alloc(sizeof(struct ffly_socket));
    ffly_err_t err;
    if (_err(err = ffly_socket(sock, __family, __type, __proto))) {
        ffly_fprintf(ffly_out, "failed to create socket.\n");
        *__err = err;
        return NULL;
    }
    return sock;
}

ffly_err_t ff_net_connect(FF_SOCKET *__sock, struct sockaddr *__addr, socklen_t __len) {
    ffly_err_t err;
    if (_err(err = ffly_sock_connect(__sock, __addr, __len))) {
        ffly_fprintf(ffly_out, "failed to connect.\n");
        return err;
    }
    return FFLY_SUCCESS;
}

FF_SOCKET* ff_net_accept(FF_SOCKET *__sock, struct sockaddr *__addr, socklen_t *__len, ffly_err_t *__err) {
    *__err = FFLY_SUCCESS;
    FF_SOCKET* sock = (FF_SOCKET*)__ffly_mem_alloc(sizeof(struct ffly_socket));
    sock->fd = ffly_sock_accept(__sock, __addr, __len, __err);
    if (_err(*__err)) {
        ffly_fprintf(ffly_out, "failed to accept.\n");
        return NULL;
    } 
    return sock;
}

ffly_err_t ff_net_listen(FF_SOCKET *__sock) {
    ffly_err_t err;
    if (_err(err = ffly_sock_listen(__sock))) {
        return err;
    }
    return FFLY_SUCCESS;
}

ffly_err_t ff_net_bind(FF_SOCKET *__sock, struct sockaddr *__addr, socklen_t __len) {
    ffly_err_t err;
    if (_err(err = ffly_sock_bind(__sock, __addr, __len))) {
        ffly_fprintf(ffly_out, "failed to bind.\n");
        return err;
    }
    return FFLY_SUCCESS;
}

ffly_err_t ff_net_sndhdr(FF_SOCKET *__sock, FF_NET_HDR *__hdr) {
    ffly_err_t err;
    ffly_size_t res;
    if ((res = ffly_sock_send(__sock, __hdr, sizeof(FF_NET_HDR), 0, &err)) < sizeof(FF_NET_HDR)) {
        return FFLY_FAILURE;
    }

    if (_err(err))
        return FFLY_FAILURE;
    return FFLY_SUCCESS;
}

ffly_err_t ff_net_rcvhdr(FF_SOCKET *__sock, FF_NET_HDR *__hdr) {
    ffly_err_t err;
    ffly_size_t res;
    if ((res = ffly_sock_recv(__sock, __hdr, sizeof(FF_NET_HDR), 0, &err)) < sizeof(FF_NET_HDR)) {
        return FFLY_FAILURE;
    }
    if (_err(err))
        return FFLY_FAILURE;
    return FFLY_SUCCESS;
}

ffly_err_t ff_net_close(FF_SOCKET *__sock) {
    ffly_sock_close(__sock);
    __ffly_mem_free(__sock);
    return FFLY_SUCCESS;
}
/*
int main() {
    ffly_err_t err;
    ffly_net_peerp peer = ff_net_creat(&err);
    

    
    ff_net_close(peer);
}*/
