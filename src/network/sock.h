# ifndef __ffly__sock__h
# define __ffly__sock__h
# include "../ffint.h"
# include "../types.h"
# include "../linux/types.h"
# include "../linux/socket.h"
# include "../linux/in.h"
# include "../in.h"
# include "../linux/net.h"
# define FF_SOCKET struct ffly_socket
# define ffly_sockfd(__sock) (__sock->fd)
struct ffly_socket {
    ff_fd_t fd;
};

int ffly_sock_shutdown(struct ffly_socket*, int);
ff_err_t ffly_socket(struct ffly_socket*, int, int, int);
ff_size_t ffly_sock_send(struct ffly_socket*, void const*, ff_size_t, int, ff_err_t*);
ff_size_t ffly_sock_recv(struct ffly_socket*, void*, ff_size_t, int, ff_err_t*);
ff_size_t ffly_sock_sendto(struct ffly_socket*, void const*, ff_size_t, int, struct sockaddr*, socklen_t, ff_err_t*);
ff_size_t ffly_sock_recvfrom(struct ffly_socket*, void*, ff_size_t, int, struct sockaddr*, socklen_t*, ff_err_t*);
ff_err_t ffly_sock_listen(struct ffly_socket*);
ff_int_t ffly_sock_accept(struct ffly_socket*, struct sockaddr*, socklen_t*, ff_err_t*);
ff_err_t ffly_sock_bind(struct ffly_socket*, struct sockaddr*, socklen_t);
ff_err_t ffly_sock_connect(struct ffly_socket*, struct sockaddr*, socklen_t);
ff_err_t ffly_sock_close(struct ffly_socket*);
# endif /*__ffly__sock__h*/
