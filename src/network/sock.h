# ifndef __ffly__sock__h
# define __ffly__sock__h
# include "../types/fd_t.h"
# include "../types/err_t.h"
# include "../types/size_t.h"
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# define FF_SOCKET struct ffly_socket
# define ffly_sockfd(__sock) (__sock->fd)
struct ffly_socket {
    ffly_fd_t fd;
};

int ffly_sock_shutdown(struct ffly_socket*, int);
ffly_err_t ffly_socket(struct ffly_socket*, int, int, int);
ffly_size_t ffly_sock_send(struct ffly_socket*, void const*, ffly_size_t, int, ffly_err_t*);
ffly_size_t ffly_sock_recv(struct ffly_socket*, void*, ffly_size_t, int, ffly_err_t*);
ffly_size_t ffly_sock_sendto(struct ffly_socket*, void const*, ffly_size_t, int, struct sockaddr*, socklen_t, ffly_err_t*);
ffly_size_t ffly_sock_recvfrom(struct ffly_socket*, void*, ffly_size_t, int, struct sockaddr*, socklen_t*, ffly_err_t*);
ffly_err_t ffly_sock_listen(struct ffly_socket*);
mdl_int_t ffly_sock_accept(struct ffly_socket*, struct sockaddr*, socklen_t*, ffly_err_t*);
ffly_err_t ffly_sock_bind(struct ffly_socket*, struct sockaddr*, socklen_t);
ffly_err_t ffly_sock_connect(struct ffly_socket*, struct sockaddr*, socklen_t);
ffly_err_t ffly_sock_close(struct ffly_socket*);
# endif /*__ffly__sock__h*/
