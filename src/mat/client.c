# include "../mtp.h"
# include "../mat.h"
# include "../net.h"
# include "../inet.h"
# include "../dep/bzero.h"
# include "../types/err_t.h"
# include "../memory/mem_free.h"
# include "../system/err.h"
# include "../system/io.h"
ffly_err_t ffmain(int __argc, char const *__argv[]) {
	if (__argc<2) {
		ffly_printf("please provide file.\n");
		retok;
	}

	ffly_err_t err;
	FF_SOCKET *sock = ff_net_creat(&err, AF_INET, SOCK_STREAM, 0);
	if (_err(err)) {
		ffly_fprintf(ffly_err, "failed to crate socket.\n");
		_ret;
	}

	struct sockaddr_in addr;
	ffly_bzero(&addr, sizeof(struct sockaddr_in));
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	addr.sin_family = AF_INET;
	addr.sin_port = htons(21299);
	if (_err(err = ff_net_connect(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)))) {
		ffly_fprintf(ffly_err, "failed to connect.\n");
		goto _end;
	}

	struct ffly_mtp_hdr hdr;
	hdr.file = __argv[1];

	if (_err(err = ffly_mtp_sndhdr(sock, &hdr))) {
		ffly_fprintf(ffly_err, "failed to recv header.\n");
		goto _end;
	}

	ffly_err_t fault;
	if (_err(err = ffly_mtp_rcverr(&fault, sock))) {
		ffly_fprintf(ffly_err, "failed to recv error.\n");
		goto _end;
	}

	if (_err(fault)) {
		ffly_fprintf(ffly_err, "got error.\n");
		goto _end;
	}

	mdl_uint_t size;
	void *p;

	p = ffly_mtp_rcv(sock, &size, &err);
	if (_err(err)) {
		ffly_fprintf(ffly_err, "failed to recv.\n");
		if (p != NULL)
			__ffly_mem_free(p);
		goto _end;
	}

	if (!p) {
		err = FFLY_FAILURE;
		ffly_fprintf(ffly_err, "got null pointer.\n");
		goto _end;
	}

	struct ffly_mat mat;
	mat.p = p;
	mat.end = mat.p+size;

	ffly_matact(&mat);

	__ffly_mem_free(p);	
	_end:
	ff_net_close(sock);
	_ret;
}
