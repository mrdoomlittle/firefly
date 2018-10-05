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

struct ffly_sock_proto {
	ff_i8_t(*socket)(void*);
	ff_i8_t(*bind)(void*, struct sockaddr*, socklen_t);
	ff_i8_t(*listen)(void*);
	ff_i8_t(*accept)(void*, void*, struct sockaddr*, socklen_t*);
	ff_uint_t(*send)(void*, void*, ff_uint_t, int, ff_i8_t*);
	ff_uint_t(*recv)(void*, void*, ff_uint_t, int, ff_i8_t*);
	ff_uint_t(*sendto)(void*, void*, ff_uint_t, int, struct sockaddr*, socklen_t, ff_i8_t*);
	ff_uint_t(*recvfrom)(void*, void*, ff_uint_t, int, struct sockaddr*, socklen_t*, ff_i8_t*);
	ff_i8_t(*close)(void*);
	ff_i8_t(*shutdown)(void*, int);
	ff_i8_t(*connect)(void*, struct sockaddr*, socklen_t);
	void*(*ctx_new)(void);
	void(*ctx_destroy)(void*);
};

struct ffly_socket {
    void *prot_ctx;
	struct ffly_sock_proto prot;

	int fd;//<<-- remove
};


int ffly_sock_shutdown(struct ffly_socket*, int);
ff_err_t ffly_socket(struct ffly_socket*);
ff_size_t ffly_sock_send(struct ffly_socket*, void const*, ff_size_t, int, ff_err_t*);
ff_size_t ffly_sock_recv(struct ffly_socket*, void*, ff_size_t, int, ff_err_t*);
ff_size_t ffly_sock_sendto(struct ffly_socket*, void const*, ff_size_t, int, struct sockaddr*, socklen_t, ff_err_t*);
ff_size_t ffly_sock_recvfrom(struct ffly_socket*, void*, ff_size_t, int, struct sockaddr*, socklen_t*, ff_err_t*);
ff_err_t ffly_sock_listen(struct ffly_socket*);
ff_int_t ffly_sock_accept(struct ffly_socket*, struct ffly_socket*, struct sockaddr*, socklen_t*, ff_err_t*);
ff_err_t ffly_sock_bind(struct ffly_socket*, struct sockaddr*, socklen_t);
ff_err_t ffly_sock_connect(struct ffly_socket*, struct sockaddr*, socklen_t);
ff_err_t ffly_sock_close(struct ffly_socket*);
# endif /*__ffly__sock__h*/
