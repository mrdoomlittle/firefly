# include "../mtp.h"
# include "../mat.h"
# include "../net.h"
# include "../inet.h"
# include "../dep/bzero.h"
# include "../memory/mem_free.h"
# include "../system/err.h"
# include "../system/io.h"
# include "../dep/str_len.h"

void* ffly_mtp_req(FF_SOCKET *__sock, char const *__file, ff_uint_t *__size) {
	ff_uint_t len;
	ff_err_t err;
	len = ffly_str_len(__file);

	ff_u8_t code[10];
	*code = 0x01;
	*(ff_u16_t*)(code+1) = 0;
	*(ff_u16_t*)(code+3) = len;
	*(code+5) = 0x00;
	*(ff_u16_t*)(code+6) = 0;
	*(ff_u16_t*)(code+8) = len;

	ff_u32_t tsz;
	tsz = 10;
	ff_net_send(__sock, &tsz, sizeof(ff_u32_t), 0, &err);
	ff_net_send(__sock, code, 10, 0, &err);

	ff_net_send(__sock, __file, len, 0, &err);
	return ffly_mtp_rcv(__sock, __size, &err);
}

ff_err_t ffmain(int __argc, char const *__argv[]) {
	if (__argc<2) {
		ffly_printf("please provide file.\n");
		retok;
	}

	ff_err_t err;
	FF_SOCKET *sock = ff_net_creat(&err, _NET_PROT_TCP);
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

	void *p;
	ff_uint_t size;
	
	p = ffly_mtp_req(sock, __argv[1], &size);

	struct ffly_mat mat;
	mat.p = p;
	mat.end = mat.p+size;

	ffly_matact(&mat);

	__ffly_mem_free(p);	
_end:
	ff_net_close(sock);
	_ret;
}
