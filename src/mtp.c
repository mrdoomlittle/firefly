# include "mtp.h"
# include <linux/limits.h>
# include "dep/str_cpy.h"
# include "dep/bzero.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/err.h"
# include "linux/fcntl.h"
# include "linux/unistd.h"
# include "linux/stat.h"
# include "system/io.h"
# include "malloc.h"
# include "dep/mem_cpy.h"
# include "dep/mem_set.h"
FF_SOCKET static *sock;

# define FILE_MAX 20

typedef struct tape {
	void *text;
	ff_uint_t len;
} *tapep;

tapep tape_new(ff_uint_t __sz) {
	tapep t;
	t = (tapep)malloc(sizeof(struct tape));
	t->text = malloc(__sz);
	t->len = __sz;
	return t;
}

void tape_destroy(tapep __t) {
	free(__t->text);
	free(__t);
}

static char *dir[PATH_MAX];
char const* ffly_mtp_errst(ff_u8_t __err) {
	switch(__err) {
		case _ff_mtp_err_ptl: return "path too large";
		case _ff_mtp_err_mst: return "missing string terminator";
	}
	return "unknown";
}
/*
ff_err_t
ffly_mtp_snd_report(FF_SOCKET *__sock, ffly_mtp_repp __report) {
	ff_err_t err;
	ff_net_send(__sock, __report, sizeof(struct ffly_mtp_rep), 0, &err);
	if (_err(err)) {
		ffly_fprintf(ffly_err, "failed to send report.\n");
		_ret;
	}
	retok;
}	

ff_err_t
ffly_mtp_rcv_report(FF_SOCKET *__sock, ffly_mtp_repp __report) {
	ff_err_t err;
	ff_net_recv(__sock, __report, sizeof(struct ffly_mtp_rep), 0, &err);
	if (_err(err)) {
		ffly_fprintf(ffly_err, "failed to recv report.\n");
		_ret;
	}
	retok;
}

ff_err_t
ffly_mtp_snderr(ff_err_t __err, FF_SOCKET *__sock) {
	ff_err_t err;
	ff_net_send(__sock, &__err, sizeof(ff_err_t), 0, &err);
	if (_err(err)) {
		ffly_fprintf(ffly_err, "failed to send error.\n");
		_ret;
	}
	retok;
}

ff_err_t
ffly_mtp_rcverr(ff_err_t *__err, FF_SOCKET *__sock) {
	ff_err_t err;
	ff_net_recv(__sock, __err, sizeof(ff_err_t), 0, &err);
	if (_err(err)) {
		ffly_fprintf(ffly_err, "failed to recv error.\n");
		_ret;
	}
	retok;
}
*/
ff_err_t
ffly_mtp_snd(FF_SOCKET *__sock, void *__p, ff_uint_t __size) {
	ff_err_t err;
	ff_net_send(__sock, &__size, sizeof(ff_uint_t), 0, &err);
	if (_err(err))
		_ret;
	ff_net_send(__sock, __p, __size, 0, &err);
	if (_err(err))
		_ret;
	retok;
}

void*
ffly_mtp_rcv(FF_SOCKET *__sock, ff_uint_t *__size, ff_err_t *__err) {
	*__err = FFLY_SUCCESS;
	ff_net_recv(__sock, __size, sizeof(ff_uint_t), 0, __err);
	if (_err(*__err))
		return NULL;
	void *p = __ffly_mem_alloc(*__size);
	ff_net_recv(__sock, p, *__size, 0, __err);
	if (_err(*__err)) {
		ffly_fprintf(ffly_out, "failed to recv.\n");
		__ffly_mem_free(p);
		return NULL;
	}
	return p;
}

ff_err_t ffly_mtpd_prepare(char const *__dir) {
	ffly_str_cpy(dir, __dir);
	retok;
}

ff_err_t ffly_mtpd_open() {
	ff_err_t err;
	struct sockaddr_in addr;
	ffly_bzero(&addr, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htons(INADDR_ANY);
	addr.sin_port = htons(21299);

	sock = ff_net_creat(&err, _NET_PROT_TCP);
	if (_err(err)) {
		ffly_fprintf(ffly_err, "failed to create socket.\n");
		_ret;
	}

	if (_err(err = ff_net_bind(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)))) {
		ff_net_close(sock);
		_ret;
	}
	retok;
}

// illegal charset
ff_i8_t static
illegal(char const *__s){
	char const *p = __s;
	char c;
	while((c = *(p++)) != '\0') {
		if ((c<'a'||c>'z') && c != '.')
			return -1;
	}
	return 0;
}

#define STACKSZ 256
ff_u8_t static stack[STACKSZ];
#define stackat(__ad) \
	(stack+(__ad))

ff_u8_t static *cc;
FF_SOCKET static *peer;
void static
get_rq(void) {
	ff_err_t err;
	ff_u16_t fa;
	fa = *(ff_u16_t*)cc;
	ff_u16_t len;
	len = *(ff_u16_t*)(cc+2);

	char file[128];
	ffly_mem_cpy(file, stackat(fa), len);
	*(file+len) = '\0';

	if (illegal(file) == -1) {
		return;
	}

	char buf[PATH_MAX];
	char *p = buf;
	p+=ffly_str_cpy(p, dir);
	*(p++) = '/';
	ffly_str_cpy(p, file);

	if (access(buf, F_OK) == -1) {
		ffly_fprintf(ffly_err, "file at '%s' doesn't exist or is inaccessible.\n", buf);
		return;
	}

	int fd;
	if ((fd = open(buf, O_RDONLY, 0)) == -1) {
		ffly_printf("failed to open file.\n");
		return;
	}

	struct stat st;
	fstat(fd, &st);

	void *exch = __ffly_mem_alloc(st.st_size);
	read(fd, exch, st.st_size);
	close(fd);

	if (_err(err = ffly_mtp_snd(peer, exch, st.st_size))) {
		__ffly_mem_free(exch);
		return;
	}
	__ffly_mem_free(exch);	

	ffly_printf("file: %s\n", buf);
}

void static
store(void) {
	ff_u16_t dst, n;
	dst = *(ff_u16_t*)cc;
	n = *(ff_u16_t*)(cc+2);

	if (dst+n>STACKSZ)
		return;
	ff_err_t err;
	ff_net_recv(peer, stackat(dst), n, 0, &err);
}

void static
load(void) {
	ff_u16_t src, n;
	src = *(ff_u16_t*)cc;
	n = *(ff_u16_t*)(cc+2);

	if (src+n>STACKSZ)
		return;
	ff_err_t err;
	ff_net_send(peer, stackat(src), n, 0, &err);
}

void static
report(void) {

}

static void(*op[])(void) = {
	get_rq,
	store,
	load,
	report
};

ff_u8_t static osz[] = {
	4,
	4,
	4
};

static ff_i8_t run = -1;
void static sig(int __no) {
    run = 0;
}


ff_err_t ffly_mtpd_start() {
	struct sigaction sa;
	ffly_mem_set(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = sig;
	sigaction(SIGINT, &sa, NULL);

	struct sockaddr_in cl;
	ffly_bzero(&cl, sizeof(struct sockaddr_in));

	ff_err_t err;
	socklen_t len = sizeof(struct sockaddr_in);
	tapep t;
	ff_u32_t tsz;
_back:
	if (_err(err = ff_net_listen(sock))) {
		ffly_fprintf(ffly_err, "failed to listen.\n");
		_ret;
	}

	peer = ff_net_accept(sock, (struct sockaddr*)&cl, &len, &err);
	if (_err(err)) {
		ffly_fprintf(ffly_err, "failed to accept.\n");
		_ret;
	}

	ff_net_recv(peer, &tsz, sizeof(ff_u32_t), 0, &err);
	t = tape_new(tsz);
	ff_net_recv(peer, t->text, len, 0, &err);
	ff_u8_t *end;
	cc = (ff_u8_t*)t->text;
	end = cc+tsz;
	ff_u8_t on;
_again:
	on = *(cc++);
	ffly_printf("op: %u\n", on);
	op[on]();
	cc+=osz[on];
	if (cc<end)
		goto _again;
	tape_destroy(t);
	ff_net_close(peer);

	if (run == -1)
		goto _back;
_end:
	if (_err(err))
		_ret;
	retok;
}

ff_err_t ffly_mtpd_close() {
	ff_net_close(sock);
	retok;
}

ff_err_t ffly_mtpd_cleanup() {
	retok;

}
