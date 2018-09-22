# include "bh.h"
# include "system/io.h"
# include "signal.h"
# include "system/error.h"
# include "brick.h"
# include "storage/reservoir.h"
# include "dep/bzero.h"
# include "inet.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "memory/mem_realloc.h"

/*
	store bricks in there own cistern
*/
struct ff_bh bh;
/*
	TODO:
		way to create x amount of bricks
*/
ff_i8_t static to_shut = -1;
void sig(int __sig) {
	to_shut = 0;
	ff_net_shutdown(bh.sock, SHUT_RDWR);
}

void ff_bhs_prep(void) {
	/*
		capture signal of ctl-c
	*/
	struct sigaction sa;
	ffly_bzero(&sa, sizeof(struct sigaction));
	sa.sa_handler = sig;
	sigaction(SIGINT, &sa, NULL);
}


void ff_bhs_close(void) {
	ff_net_close(bh.sock);
}

void ff_bhs_open(void) {
	ff_err_t err;
	struct sockaddr_in addr;
	ffly_bzero(&addr, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htons(INADDR_ANY);
	addr.sin_port = htons(21299);
	FF_SOCKET *sock;

	sock = ff_net_creat(&err, AF_INET, SOCK_STREAM, 0);
	if (_err(err))
		return;

	int val = 1;
	if (setsockopt(sock->fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int)) == -1) {
		return;
	}

	if (_err(err = ff_net_bind(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)))) {
		return;
	}

	bh.sock = sock;
}

void static
bread(long __arg, void *__p, ff_u8_t __bhz) {
	ffly_reservoir_read(&__ffly_reservoir__, (void*)__arg, __p, 0, bricksz(__bhz));
}

void static
bwrite(long __arg, void *__p, ff_u8_t __bhz) {
	ffly_reservoir_write(&__ffly_reservoir__, (void*)__arg, __p, 0, bricksz(__bhz));
}

void static
bdel(long __arg) {
	ffly_reservoir_free(&__ffly_reservoir__, (void*)__arg);
}

ff_err_t ff_bh_bnewm(ff_bhp __bh, ff_u8_t __sz, ff_u32_t *__b, ff_uint_t __n) {
	struct bhop op = {_bhop_bnewm};
	ff_bh_opsnd(__bh->sock, &op);

	ff_err_t err;
	ff_net_send(__bh->sock, &__sz, sizeof(ff_u8_t), &err);
	ff_net_send(__bh->sock, &__n, sizeof(ff_uint_t), &err);

	ff_net_recv(__bh->sock, __b, __n*sizeof(ff_u32_t), &err);
	return err;
}

ff_err_t ff_bh_bridm(ff_bhp __bh, ff_u32_t *__b, ff_uint_t __n) {
	struct bhop op = {_bhop_bridm};
	ff_bh_opsnd(__bh->sock, &op);

	ff_err_t err;
	ff_net_send(__bh->sock, &__n, sizeof(ff_uint_t), &err);
	ff_net_send(__bh->sock, __b, __n*sizeof(ff_u32_t), &err);
	return err;
}

ff_u32_t ff_bh_bnew(ff_bhp __bh, ff_u8_t __sz, ff_err_t *__err) {
	struct bhop op = {_bhop_bnew};
	ff_bh_opsnd(__bh->sock, &op);

	ff_net_send(__bh->sock, &__sz, sizeof(ff_u8_t), __err);

	ff_u32_t b;
	ff_net_recv(__bh->sock, &b, sizeof(ff_u32_t), __err);
	return b;
}

ff_err_t ff_bh_brid(ff_bhp __bh, ff_u32_t __b) {
	struct bhop op = {_bhop_brid};
	ff_bh_opsnd(__bh->sock, &op);

	ff_err_t err;
	ff_net_send(__bh->sock, &__b, sizeof(ff_u32_t), &err);
	return err;
}

ff_err_t ff_bh_bopen(ff_bhp __bh, ff_u32_t __b) {
	struct bhop op = {_bhop_bopen};
	ff_bh_opsnd(__bh->sock, &op);

	ff_err_t err;
	ff_net_send(__bh->sock, &__b, sizeof(ff_u32_t), &err);
	return err;
}

ff_err_t ff_bh_bwrite(ff_bhp __bh, ff_u32_t __b, void *__buf, ff_uint_t __len, ff_uint_t __offset) {
	struct bhop op = {_bhop_bwrite};
	ff_bh_opsnd(__bh->sock, &op);

	ff_err_t err;
	ff_net_send(__bh->sock, &__b, sizeof(ff_u32_t), &err);
	ff_net_send(__bh->sock, &__len, sizeof(ff_uint_t), &err);
	ff_net_send(__bh->sock, &__offset, sizeof(ff_uint_t), &err);
	ff_net_send(__bh->sock, __buf, __len, &err);
	return err;
}

ff_err_t ff_bh_bread(ff_bhp __bh, ff_u32_t __b, void *__buf, ff_uint_t __len, ff_uint_t __offset) {
	struct bhop op = {_bhop_bread};
	ff_bh_opsnd(__bh->sock, &op);

	ff_err_t err;
	ff_net_send(__bh->sock, &__b, sizeof(ff_u32_t), &err);
	ff_net_send(__bh->sock, &__len, sizeof(ff_uint_t), &err);
	ff_net_send(__bh->sock, &__offset, sizeof(ff_uint_t), &err);
	ff_net_recv(__bh->sock, __buf, __len, &err);
	return err;
}

ff_err_t ff_bh_bclose(ff_bhp __bh, ff_u32_t __b) {
	struct bhop op = {_bhop_bclose};
	ff_bh_opsnd(__bh->sock, &op);

	ff_err_t err;
	ff_net_send(__bh->sock, &__b, sizeof(ff_u32_t), &err);
	return err;
}

void static
bh_bnewm(FF_SOCKET *__sock) {
	ff_err_t err;
	ff_u8_t sz;
	ff_uint_t n;
	ff_net_recv(__sock, &sz, 1, &err);
	ff_net_recv(__sock, &n, sizeof(ff_uint_t), &err);

	ff_u32_t *b;
	b = (ff_u32_t*)__ffly_mem_alloc(n*sizeof(ff_u32_t));

	ff_uint_t i;
	i = 0;

	while(i != n)
		b[i++] = ffly_brick_new(sz, bread, bwrite, bdel, ffly_reservoir_alloc(&__ffly_reservoir__, bricksz(sz)));
	ff_net_send(__sock, b, n*sizeof(ff_u32_t), &err);
	__ffly_mem_free(b);
	ffly_printf("created new brick/s - %u, size: %u\n", n, bricksz(sz));
}

void static
bh_bridm(FF_SOCKET *__sock) {
	ff_err_t err;
	ff_uint_t n;
	ff_net_recv(__sock, &n, sizeof(ff_uint_t), &err);

	ff_u32_t *b;
	b = __ffly_mem_alloc(n*sizeof(ff_u32_t));
	ff_net_recv(__sock, b, n*sizeof(ff_u32_t), &err);
	ff_uint_t i;

	i = 0;
	while(i != n)
		ffly_brick_rid(b[i++]);
	__ffly_mem_free(b);
}

void static
bh_bnew(FF_SOCKET *__sock) {
	ff_u32_t b;

	ff_err_t err;
	ff_u8_t sz;
	ff_net_recv(__sock, &sz, 1, &err);
	b = ffly_brick_new(sz, bread, bwrite, bdel, ffly_reservoir_alloc(&__ffly_reservoir__, bricksz(sz)));

	ff_net_send(__sock, &b, sizeof(ff_u32_t), &err);
	ffly_printf("created new brick - %u, size: %u\n", b, bricksz(sz));
}

void static
bh_brid(FF_SOCKET *__sock) {
	ff_err_t err;
	ff_u32_t b;
	ff_net_recv(__sock, &b, sizeof(ff_u32_t), &err);
	ffly_brick_rid(b);
	ffly_printf("getting rid of brick - %u\n", b);
}

void static
bh_bopen(FF_SOCKET *__sock) {
	ff_err_t err;
	ff_u32_t b;
	ff_net_recv(__sock, &b, sizeof(ff_u32_t), &err);
	ffly_brick_open(b);
	ffly_printf("opening brick - %u\n", b);
}

void static
bh_bwrite(FF_SOCKET *__sock) {
	ff_err_t err;
	ff_u32_t b;
	ff_uint_t len;
	ff_uint_t off;

	ff_net_recv(__sock, &b, sizeof(ff_u32_t), &err);
	ff_net_recv(__sock, &len, sizeof(ff_uint_t), &err);
	ff_net_recv(__sock, &off, sizeof(ff_uint_t), &err);

	void *p;
	if (!(p = ffly_brick_get(b))) {
		// error
	}

	ff_net_recv(__sock, p, len, &err);
	ffly_printf("brick : write -%u, buflen: %u\n", b, len);
}

void static 
bh_bread(FF_SOCKET *__sock) {
	ff_err_t err;
	ff_u32_t b;
	ff_uint_t len;
	ff_uint_t off;

	ff_net_recv(__sock, &b, sizeof(ff_u32_t), &err);
	ff_net_recv(__sock, &len, sizeof(ff_uint_t), &err);
	ff_net_recv(__sock, &off, sizeof(ff_uint_t), &err);

	void *p;
	if (!(p = ffly_brick_get(b))) {
		// error
	}

	ff_net_send(__sock, p, len, &err);	
	ffly_printf("brick : read -%u, buflen: %u\n", b, len);
}

void static
bh_bclose(FF_SOCKET *__sock) {
	ff_err_t err;
	ff_u32_t b;
	ff_net_recv(__sock, &b, sizeof(ff_u32_t), &err);
	ffly_brick_close(b);
	ffly_printf("closing brick -%u\n", b);
}

void _bnewm();
void _bridm();
void _bnew();
void _brid();
void _bopen();
void _bwrite();
void _bread();
void _bclose();

static void *jmp[] = {
	_bnewm,
	_bridm,
	_bnew,
	_brid,
	_bopen,
	_bwrite,
	_bread,
	_bclose
};

# define jmpend __asm__("jmp _bh_end")
void static
doop(struct bhop *__op, FF_SOCKET *__sock) {
	__asm__("jmp *%0" : : "r"(jmp[__op->kind]));

	__asm__("_bnewm:\n\t");
	bh_bnewm(__sock);
	jmpend;

	__asm__("_bridm:\n\t");
	bh_bridm(__sock);
	jmpend;

	__asm__("_bnew:\n\t");
	bh_bnew(__sock);
	jmpend;

	__asm__("_brid:\n\t");
	bh_brid(__sock);
	jmpend;

	__asm__("_bopen:\n\t");
	bh_bopen(__sock);
	jmpend;

	__asm__("_bwrite:\n\t");
	bh_bwrite(__sock);
	jmpend;

	__asm__("_bread:\n\t");
	bh_bread(__sock);
	jmpend;

	__asm__("_bclose:\n\t");
	bh_bclose(__sock);
	jmpend;

	__asm__("_bh_end:\n\t");
}

void ff_bhs_start(void) {
	struct bhop op;
	ff_err_t err;
	FF_SOCKET *peer;
	socklen_t len;

	FF_SOCKET *sock;


	sock = bh.sock;
	struct sockaddr_in cl;
	peer = NULL;
_again:
	ffly_bzero(&cl, sizeof(struct sockaddr_in));
	len = sizeof(struct sockaddr_in);
	if (_err(err = ff_net_listen(sock))) {
		return;
	}

	peer = ff_net_accept(sock, (struct sockaddr*)&cl, &len, &err);
	if (_err(err))
		return;
	ffly_printf("some bigshot has decided to connected.\n");
	while(1) {
		err = ff_bh_oprcv(peer, &op);
		if (_err(err))
			goto _fail;
		if (op.kind == _bhop_disconnect)
			break;
		if (op.kind >_bhop_bclose) {
			ffly_printf("invalid op kind.\n");
			break;
		}
		doop(&op, peer);
	}

	ffly_printf("thay left.\n");
	ff_net_close(peer);
	peer = NULL;
	goto _again;
	return;
_fail:
	if (peer != NULL) {
		ff_net_close(peer);
	}

	while(to_shut<0);
	return;
}

void ff_bhs_closedown(void) {
	to_shut = 0;
	// wait for okay
	ff_bhs_close();
}

void ff_bhs_cleanup(void) {
}

void ff_bh_open(ff_bhp __bh) {
	ff_err_t err;
	__bh->sock = ff_net_creat(&err, AF_INET, SOCK_STREAM, 0);
}

void ff_bh_connect(ff_bhp __bh, char const *__ipadr, ff_u16_t __port) {
	struct sockaddr_in addr;
	ffly_bzero(&addr, sizeof(struct sockaddr_in));
	addr.sin_addr.s_addr = inet_addr(__ipadr);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(__port);
	ff_net_connect(__bh->sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
}

void ff_bh_disconnect(ff_bhp __bh) {
	// tell server we are going to disconnect and to remove are existance
	struct bhop op = {_bhop_disconnect};
	ff_bh_opsnd(__bh->sock, &op);
}

void ff_bh_close(ff_bhp __bh) {
	ff_net_close(__bh->sock);
}
