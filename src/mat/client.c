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
	ffly_err_t err;
	FF_SOCKET *sock = ff_net_creat(&err, AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr;
	ffly_bzero(&addr, sizeof(struct sockaddr_in));
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	addr.sin_family = AF_INET;
	addr.sin_port = htons(21299);
	if (_err(ff_net_connect(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)))) {
		ff_net_close(sock);
		return 0;
	}
	struct ffly_mtp_hdr hdr;
	hdr.file = "test.mat";

	if (_err(ffly_mtp_sndhdr(sock, &hdr))) {
		ff_net_close(sock);
		return -1;
	}

	mdl_uint_t size;
	void *p;

	p = ffly_mtp_rcv(sock, &size, &err);
	if (_err(err) || !p) {
		ff_net_close(sock);
		return -1;
	}

	struct ffly_mat mat;
	mat.p = p;
	mat.end = mat.p+size;

	ffly_matact(&mat);

	__ffly_mem_free(p);	
	ff_net_close(sock);
}
