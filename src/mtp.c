# include "mtp.h"
# include "types/err_t.h"
# include <linux/limits.h>
# include "dep/str_cpy.h"
# include "dep/bzero.h"
# include "types/err_t.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/err.h"
# include "linux/fcntl.h"
# include "linux/unistd.h"
# include "linux/stat.h"
# include "system/io.h"
FF_SOCKET static *sock;

char *dir[PATH_MAX];

ffly_err_t
ffly_mtp_sndhdr(FF_SOCKET *__sock, ffly_mtp_hdrp __hdr) {
	ffly_err_t err;
	mdl_uint_t l = ffly_str_len(__hdr->file)+1;
	ff_net_send(__sock, &l, sizeof(mdl_uint_t), &err);
	if (_err(err))
		_ret;
	ff_net_send(__sock, __hdr->file, l, &err);
	if (_err(err))
		_ret;
	retok;
}

ffly_err_t
ffly_mtp_rcvhdr(FF_SOCKET *__sock, ffly_mtp_hdrp __hdr) {
	ffly_err_t err;
	mdl_uint_t l;
	ff_net_recv(__sock, &l, sizeof(mdl_uint_t), &err);
	if (_err(err))
		_ret;
	ffly_printf("got: %u\n", l);

	__hdr->file = (char const*)__ffly_mem_alloc(l);
	ff_net_recv(__sock, __hdr->file, l, &err);
	if (_err(err)) {
		__ffly_mem_free(__hdr->file);
		_ret;
	}
	retok;
}

ffly_err_t
ffly_mtp_snd(FF_SOCKET *__sock, void *__p, mdl_uint_t __size) {
	ffly_err_t err;
	ff_net_send(__sock, &__size, sizeof(mdl_uint_t), &err);
	if (_err(err))
		_ret;
	ff_net_send(__sock, __p, __size, &err);
	if (_err(err))
		_ret;
	retok;
}

void*
ffly_mtp_rcv(FF_SOCKET *__sock, mdl_uint_t *__size, ffly_err_t *__err) {
	*__err = FFLY_SUCCESS;
	ff_net_recv(__sock, __size, sizeof(mdl_uint_t), __err);
	if (_err(*__err))
		return NULL;
	void *p = __ffly_mem_alloc(*__size);
	ff_net_recv(__sock, p, *__size, __err);
	if (_err(*__err)) {
		ffly_fprintf(ffly_out, "failed to recv.\n");
		__ffly_mem_free(p);
		return NULL;
	}
	return p;
}

ffly_err_t ffly_mtpd_prepare(char const *__dir) {
	ffly_str_cpy(dir, __dir);
	retok;
}

ffly_err_t ffly_mtpd_open() {
	ffly_err_t err;
	struct sockaddr_in addr;
	ffly_bzero(&addr, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htons(INADDR_ANY);
	addr.sin_port = htons(21299);

	sock = ff_net_creat(&err, AF_INET, SOCK_STREAM, 0);
	int val = 1;
	if (setsockopt(sock->fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int)) == -1) {
		ffly_printf("failed to set opt.\n");
		reterr;
	}

	if (_err(ff_net_bind(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)))) {
		ff_net_close(sock);
		reterr;
	}
	retok;
}

ffly_err_t ffly_mtpd_start() {
	struct sockaddr_in cl;
	ffly_bzero(&cl, sizeof(struct sockaddr_in));

	ffly_err_t err;
	socklen_t len = sizeof(struct sockaddr_in);
	FF_SOCKET *peer;
	if (_err(ff_net_listen(sock)))
		reterr;
	peer = ff_net_accept(sock, (struct sockaddr*)&cl, &len, &err);
	if (_err(err))
		reterr;

	struct ffly_mtp_hdr hdr;
	if (_err(ffly_mtp_rcvhdr(peer, &hdr)))
		reterr;

	char buf[PATH_MAX];
	char *p = buf;
	p+=ffly_str_cpy(p, dir);
	*(p++) = '/';
	ffly_str_cpy(p, hdr.file);
	__ffly_mem_free(hdr.file);

	int fd;
	if ((fd = open(buf, O_RDONLY, 0)) == -1) {
		ffly_printf("failed to open file.\n");
	}

	struct stat st;
	fstat(fd, &st);

	void *exch = __ffly_mem_alloc(st.st_size);
	read(fd, exch, st.st_size);
	close(fd);

	if (_err(ffly_mtp_snd(peer, exch, st.st_size))) {
		__ffly_mem_free(exch);
		ff_net_close(peer);
		reterr;
	}
	__ffly_mem_free(exch);	

	ffly_printf("path: %s\n", buf);
	ff_net_close(peer);
	retok;
}

ffly_err_t ffly_mtpd_close() {
	ff_net_close(sock);
	retok;
}

ffly_err_t ffly_mtpd_cleanup() {
	retok;

}
