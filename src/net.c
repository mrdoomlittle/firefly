# include "net.h"
# include "system/errno.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/err.h"
# include "system/io.h"
# ifdef __ffly_debug
# include "location.h"
# endif
/*
# define MAX 20
FF_SOCKET *old[MAX];
FF_SOCKET *next = old;
*/
ff_err_t ff_net_shutdown(FF_SOCKET *__sock, int __how) {
	ffly_sock_shutdown(__sock, __how);
	retok;
} 
void ffly_tcp_prot(struct ffly_sock_proto*);
void ffly_udp_prot(struct ffly_sock_proto*);
FF_SOCKET* ff_net_creat(ff_err_t *__err, ff_u8_t __proto) {
# ifdef __ffly_debug
	ff_location_push(_ff_loc_net_creat);
# endif
	*__err = FFLY_SUCCESS;
	FF_SOCKET* sock;
	ff_err_t err;
	if (!(sock = (FF_SOCKET*)__ffly_mem_alloc(sizeof(struct ffly_socket)))) {
		*__err = FFLY_FAILURE;
		goto _fail;
	}

	switch(__proto) {
		case _NET_PROT_TCP:
			ffly_tcp_prot(&sock->prot);
		break;
		case _NET_PROT_UDP:
			ffly_udp_prot(&sock->prot);
		break;
	}

	if (_err(err = ffly_socket(sock))) {
		ffly_fprintf(ffly_out, "failed to create socket.\n");
		*__err = err;
		goto _fail;
	}
	goto _succ;
_fail:
	if (sock != NULL)
		__ffly_mem_free(sock);
	sock = NULL;
_succ:
# ifdef __ffly_debug
	ff_location_pop();
# endif
	return sock;
}

ff_err_t ff_net_connect(FF_SOCKET *__sock, struct sockaddr *__addr, socklen_t __len) {
# ifdef __ffly_debug
	ff_location_push(_ff_loc_net_connect);
# endif
	ff_err_t err;
	if (_err(err = ffly_sock_connect(__sock, __addr, __len))) {
		ffly_fprintf(ffly_out, "failed to connect.\n");
		goto _fail;
	}

_fail:
# ifdef __ffly_debug
	ff_location_pop();
# endif
	return err;
}

FF_SOCKET* ff_net_accept(FF_SOCKET *__sock, struct sockaddr *__addr, socklen_t *__len, ff_err_t *__err) {
	*__err = FFLY_SUCCESS;
	FF_SOCKET *sock;
	if (!(sock = (FF_SOCKET*)__ffly_mem_alloc(sizeof(struct ffly_socket)))) {
		*__err = FFLY_FAILURE;
		return NULL;
	}

	*__err = ffly_sock_accept(__sock, sock, NULL, NULL);
	if (_err(*__err)) {
		__ffly_mem_free(sock);
		ffly_fprintf(ffly_out, "failed to accept.\n");
		return NULL;
	} 
	return sock;
}

ff_err_t ff_net_listen(FF_SOCKET *__sock) {
	ff_err_t err;
	if (_err(err = ffly_sock_listen(__sock))) {
		return err;
	}
	retok;
}

ff_err_t ff_net_bind(FF_SOCKET *__sock, struct sockaddr *__addr, socklen_t __len) {
	ff_err_t err;
	if (_err(err = ffly_sock_bind(__sock, __addr, __len))) {
		ffly_fprintf(ffly_out, "failed to bind.\n");
		return err;
	}
	retok;
}

ff_err_t ff_net_close(FF_SOCKET *__sock) {
	ffly_sock_close(__sock);
	__ffly_mem_free(__sock);
	retok;
}
/*
int main() {
	ff_err_t err;
	ffly_net_peerp peer = ff_net_creat(&err);
	

	
	ff_net_close(peer);
}*/
