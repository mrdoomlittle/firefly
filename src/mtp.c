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

# define FILE_MAX 20

char *dir[PATH_MAX];
char const* ffly_mtp_errst(mdl_u8_t __err) {
	switch(__err) {
		case _ff_mtp_err_ptl: return "path too large";
		case _ff_mtp_err_mst: return "missing string terminator";
	}
	return "unknown";
}

ffly_err_t
ffly_mtp_snd_report(FF_SOCKET *__sock, ffly_mtp_repp __report) {
	ffly_err_t err;
	ff_net_send(__sock, __report, sizeof(struct ffly_mtp_rep), &err);
	if (_err(err)) {
		ffly_fprintf(ffly_err, "failed to send report.\n");
		_ret;
	}
	retok;
}	

ffly_err_t
ffly_mtp_rcv_report(FF_SOCKET *__sock, ffly_mtp_repp __report) {
	ffly_err_t err;
	ff_net_recv(__sock, __report, sizeof(struct ffly_mtp_rep), &err);
	if (_err(err)) {
		ffly_fprintf(ffly_err, "failed to recv report.\n");
		_ret;
	}
	retok;
}

ffly_err_t
ffly_mtp_snderr(ffly_err_t __err, FF_SOCKET *__sock) {
	ffly_err_t err;
	ff_net_send(__sock, &__err, sizeof(ffly_err_t), &err);
	if (_err(err)) {
		ffly_fprintf(ffly_err, "failed to send error.\n");
		_ret;
	}
	retok;
}

ffly_err_t
ffly_mtp_rcverr(ffly_err_t *__err, FF_SOCKET *__sock) {
	ffly_err_t err;
	ff_net_recv(__sock, __err, sizeof(ffly_err_t), &err);
	if (_err(err)) {
		ffly_fprintf(ffly_err, "failed to recv error.\n");
		_ret;
	}
	retok;
}

ffly_err_t
ffly_mtp_sndhdr(FF_SOCKET *__sock, ffly_mtp_hdrp __hdr) {
	ffly_err_t err;
	mdl_uint_t l = ffly_str_len(__hdr->file)+1;
	ff_net_send(__sock, &l, sizeof(mdl_uint_t), &err);
	if (_err(err))
		_ret;

	ffly_err_t fault;
	ffly_mtp_rcverr(&fault, __sock);
	if (_err(fault)) {
		ffly_fprintf(ffly_err, "recved an error.\n");
		reterr;
	}

	ff_net_send(__sock, __hdr->file, l, &err);
	if (_err(err))
		_ret;

	ffly_mtp_rcverr(&fault, __sock);
	if (_err(fault)) {
		ffly_fprintf(ffly_err, "recved an error.\n");
		reterr;
	}
	retok;
}

ffly_err_t
ffly_mtp_rcvhdr(FF_SOCKET *__sock, ffly_mtp_hdrp __hdr) {
	ffly_err_t err;
	mdl_uint_t l;
	ff_net_recv(__sock, &l, sizeof(mdl_uint_t), &err);
	if (_err(err))
		_ret;

	if (l>FILE_MAX) {
		ffly_printf("length too large.\n");
		goto _fail;
	} else
		ffly_mtp_snderr(FFLY_SUCCESS, __sock);

	ffly_printf("got: %u\n", l);

	__hdr->file = (char const*)__ffly_mem_alloc(l);
	ff_net_recv(__sock, __hdr->file, l, &err);
	if (*(__hdr->file+(l-1)) != '\0') {
		ffly_printf("string terminator missing.\n");
		goto _fail;
	} else
		ffly_mtp_snderr(FFLY_SUCCESS, __sock);

	if (_err(err)) {
		__ffly_mem_free(__hdr->file);
		_ret;
	}

	retok;
	_fail:
	ffly_mtp_snderr(FFLY_FAILURE, __sock);
	reterr;
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
	if (_err(err)) {
		ffly_fprintf(ffly_err, "failed to create socket.\n");
		_ret;
	}

	int val = 1;
	if (setsockopt(sock->fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int)) == -1) {
		ffly_printf("failed to set opt.\n");
		reterr;
	}

	if (_err(err = ff_net_bind(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)))) {
		ff_net_close(sock);
		_ret;
	}
	retok;
}

mdl_i8_t static
illegal(char const *__s){
	char const *p = __s;
	char c;
	while((c = *(p++)) != '\0') {
		if ((c<'a'||c>'z') && c != '.')
			return -1;
	}
	return 0;
}

ffly_err_t ffly_mtpd_start() {
	struct sockaddr_in cl;
	ffly_bzero(&cl, sizeof(struct sockaddr_in));

	ffly_err_t err;
	socklen_t len = sizeof(struct sockaddr_in);
	FF_SOCKET *peer;
	if (_err(err = ff_net_listen(sock))) {
		ffly_fprintf(ffly_err, "failed to listen.\n");
		_ret;
	}

	peer = ff_net_accept(sock, (struct sockaddr*)&cl, &len, &err);
	if (_err(err)) {
		ffly_fprintf(ffly_err, "failed to accept.\n");
		_ret;
	}

	struct ffly_mtp_hdr hdr;
	if (_err(err = ffly_mtp_rcvhdr(peer, &hdr))) {
		ffly_fprintf(ffly_err, "failed to recv header.\n");
		goto _end;
	}

	if (illegal(hdr.file) == -1) {
		ffly_mtp_snderr(FFLY_FAILURE, peer);
		__ffly_mem_free(hdr.file);
		ffly_fprintf(ffly_err, "file string holds illegal char/s.\n");
		goto _end;
	}

	char buf[PATH_MAX];
	char *p = buf;
	p+=ffly_str_cpy(p, dir);
	*(p++) = '/';
	ffly_str_cpy(p, hdr.file);
	__ffly_mem_free(hdr.file);

	if (access(buf, F_OK) == -1) {
		ffly_mtp_snderr(FFLY_FAILURE, peer);
		ffly_fprintf(ffly_err, "file at '%s' doesn't exist.\n", buf);
		goto _end;
	}

	int fd;
	if ((fd = open(buf, O_RDONLY, 0)) == -1) {
		ffly_printf("failed to open file.\n");
		ffly_mtp_snderr(FFLY_FAILURE, peer);
		goto _end;
	}

	struct stat st;
	fstat(fd, &st);

	void *exch = __ffly_mem_alloc(st.st_size);
	read(fd, exch, st.st_size);
	close(fd);

	ffly_mtp_snderr(FFLY_SUCCESS, peer);

	if (_err(err = ffly_mtp_snd(peer, exch, st.st_size))) {
		__ffly_mem_free(exch);
		goto _end;
	}
	__ffly_mem_free(exch);	

	ffly_printf("file: %s\n", buf);

	_end:
	ff_net_close(peer);
	if (_err(err))
		_ret;
	retok;
}

ffly_err_t ffly_mtpd_close() {
	ff_net_close(sock);
	retok;
}

ffly_err_t ffly_mtpd_cleanup() {
	retok;

}
