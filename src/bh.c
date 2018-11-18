# include "bh.h"
# include "system/io.h"
# include "signal.h"
# include "system/error.h"
# include "brick.h"
# include "storage/cistern.h"
# include "dep/bzero.h"
# include "inet.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "memory/mem_realloc.h"
/*
	move server and client code to there own file coth coth bh/server.c/client.c
*/

/*
	store bricks in there own cistern
*/
struct ff_bh bh;
/*
	TODO:
		way to create x amount of bricks
*/

# define tape(__bh, __code, __len, __err)	\
{	\
	ff_u32_t len;	\
	len = __len;	\
	ff_net_send(__bh->sock, &len, sizeof(ff_u32_t), 0, __err);	\
	ff_net_send(__bh->sock, __code, __len, 0, __err);	\
}

struct ffly_cistern static ctn;
ff_i8_t static to_shut = -1;
int static sockfd;
void sig(int __sig) {
	to_shut = 0;
	shutdown(sockfd, SHUT_RDWR);
}

void ff_bhs_prep(void) {
	/*
		capture signal of ctl-c
	*/
	struct sigaction sa;
	ffly_bzero(&sa, sizeof(struct sigaction));
	sa.sa_handler = sig;
	sigaction(SIGINT, &sa, NULL);
	ffly_cistern_init(&ctn, "bh.cis");
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
	addr.sin_port = htons(40960);
	FF_SOCKET *sock;

	sock = ff_net_creat(&err, _NET_PROT_TCP);
	if (_err(err))
		return;

	if (_err(err = ff_net_bind(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)))) {
		return;
	}

	sock->prot.get(0x00, (long long)&sockfd, sock->prot_ctx);
	bh.sock = sock;
}

void static
bread(long long __arg, void *__p, ff_u8_t __bhz) {
	ffly_cistern_read(&ctn, (void*)__arg, __p, 0, bricksz(__bhz));
}

void static
bwrite(long long __arg, void *__p, ff_u8_t __bhz) {
	ffly_cistern_write(&ctn, (void*)__arg, __p, 0, bricksz(__bhz));
}

void static
bdel(long long __arg) {
	ffly_cistern_free(&ctn, (void*)__arg);
}

ff_err_t ff_bh_bnewm(ff_bhp __bh, ff_u8_t __sz, ff_u32_t *__b, ff_uint_t __n) {
	ff_err_t err;
	ff_u8_t code[6];
	*code = _bhop_bnewm;
	*(code+1) = __sz;
	*(ff_u32_t*)(code+2) = __n;
	tape(__bh, code, 6, &err);

	ff_net_recv(__bh->sock, __b, __n*sizeof(ff_u32_t), 0, &err);
	ff_uint_t i;
	i = 0;
	for(;i != __n;i++)
		ffly_printf("new brick: %u\n", __b[i]);
	return err;
}

ff_err_t ff_bh_bridm(ff_bhp __bh, ff_u32_t *__b, ff_uint_t __n) {
	ff_err_t err;
	ff_u8_t code[5];
	*code = _bhop_bridm;
	*(ff_u32_t*)(code+1) = __n;
	tape(__bh, code, 5, &err);

	ff_uint_t i;
	i = 0;
	for(;i != __n;i++)
		ffly_printf("rid brick: %u\n", __b[i]);

	ff_net_send(__bh->sock, __b, __n*sizeof(ff_u32_t), 0, &err);
	return err;
}

ff_u32_t ff_bh_bnew(ff_bhp __bh, ff_u8_t __sz, ff_err_t *__err) {
	ff_err_t err;
	ff_u8_t code[2];
	*code = _bhop_bnew;
	*(code+1) = __sz;
	tape(__bh, code, 2, &err);

	ff_u32_t b;
	ff_net_recv(__bh->sock, &b, sizeof(ff_u32_t), 0, &err);
	*__err = err;
	return b;
}

ff_err_t ff_bh_brid(ff_bhp __bh, ff_u32_t __b) {
	ff_err_t err;
	ff_u8_t code[5];
	*code = _bhop_brid;
	*(ff_u32_t*)(code+1) = __b;
	tape(__bh, code, 5, &err);
	return err;
}

ff_err_t ff_bh_bopen(ff_bhp __bh, ff_u32_t __b) {
	ff_err_t err;
	ff_u8_t code[5];
	*code = _bhop_bopen;
	*(ff_u32_t*)(code+1) = __b;
	tape(__bh, code, 5, &err);
	return err;
}

ff_err_t ff_bh_bwrite(ff_bhp __bh, ff_u32_t __b, void *__buf, ff_uint_t __len, ff_uint_t __offset) {
	ff_err_t err;
	ff_u8_t code[13];
	*code = _bhop_bwrite;
	*(ff_u32_t*)(code+1) = __b;
	*(ff_u32_t*)(code+5) = __len;
	*(ff_u32_t*)(code+9) = __offset;
	tape(__bh, code, 13, &err);
	ff_net_send(__bh->sock, __buf, __len, 0, &err);
	return err;
}

ff_err_t ff_bh_bread(ff_bhp __bh, ff_u32_t __b, void *__buf, ff_uint_t __len, ff_uint_t __offset) {
	ff_err_t err;
	ff_u8_t code[13];
	*code = _bhop_bread;
	*(ff_u32_t*)(code+1) = __b;
	*(ff_u32_t*)(code+5) = __len;
	*(ff_u32_t*)(code+9) = __offset;
	tape(__bh, code, 13, &err);
	ff_net_recv(__bh->sock, __buf, __len, 0, &err);
	return err;
}

ff_err_t ff_bh_bclose(ff_bhp __bh, ff_u32_t __b) {
	ff_err_t err;
	ff_u8_t code[5];
	*code = _bhop_bclose;
	*(ff_u32_t*)(code+1) = __b;
	tape(__bh, code, 5, &err);
	return err;
}

ff_i8_t ff_bh_bexist(ff_bhp __bh, ff_u32_t __b, ff_err_t *__err) {
	ff_u8_t code[5];
	*code = _bhop_bexist;
	*(ff_u32_t*)(code+1) = __b;
	tape(__bh, code, 5, __err);
	ff_i8_t r;
	ff_net_recv(__bh->sock, &r, 1, 0, __err);
	return r;
}

struct {FF_SOCKET *sock;} client;
ff_u8_t static *bh_cc;

void static
bh_bnewm(void) {
	ff_err_t err;
	ff_u8_t sz;
	ff_u32_t n;
	sz = *bh_cc;
	n = *(ff_u32_t*)(bh_cc+1);

	ff_u32_t *b;
	b = (ff_u32_t*)__ffly_mem_alloc(n*sizeof(ff_u32_t));

	ff_uint_t i;
	i = 0;

	while(i != n) {
		void *p;
		b[i++] = ffly_brick_new(sz, bread, bwrite, bdel, (long long)(p = ffly_cistern_alloc(&ctn, bricksz(sz))));
		ffly_printf("new brick: %u\n", b[i-1]);
	}
	ff_net_send(client.sock, b, n*sizeof(ff_u32_t), 0, &err);
	__ffly_mem_free(b);
	ffly_printf("created new brick/s - %u, size: %u\n", n, bricksz(sz));
}

void static
bh_bridm(void) {
	ff_err_t err;
	ff_u32_t n;
	n = *(ff_u32_t*)bh_cc;

	ff_u32_t *b;
	b = __ffly_mem_alloc(n*sizeof(ff_u32_t));
	ff_net_recv(client.sock, b, n*sizeof(ff_u32_t), 0, &err);
	ff_uint_t i;

	i = 0;
	while(i != n) {
		ffly_printf("rid brick: %u\n", b[i]);
		ffly_brick_rid(b[i++]);
	}
	__ffly_mem_free(b);
}

void static
bh_bnew(void) {
	ff_u32_t b;

	ff_err_t err;
	ff_u8_t sz;
	sz = *bh_cc;
	b = ffly_brick_new(sz, bread, bwrite, bdel, (long long)ffly_cistern_alloc(&ctn, bricksz(sz)));

	ff_net_send(client.sock, &b, sizeof(ff_u32_t), 0, &err);
	ffly_printf("created new brick - %u, size: %u\n", b, bricksz(sz));
}

void static
bh_brid(void) {
	ff_err_t err;
	ff_u32_t b;
	b = *(ff_u32_t*)bh_cc;
	ffly_brick_rid(b);
	ffly_printf("getting rid of brick - %u\n", b);
}

void static
bh_bopen(void) {
	ff_err_t err;
	ff_u32_t b;
	b = *(ff_u32_t*)bh_cc;
	ffly_brick_open(b);
	ffly_printf("opening brick - %u\n", b);
}

void static
bh_bwrite(void) {
	ff_err_t err;
	ff_u32_t b;
	ff_u32_t len;
	ff_u32_t off;

	b = *(ff_u32_t*)bh_cc;
	len = *(ff_u32_t*)(bh_cc+4);
	off = *(ff_u32_t*)(bh_cc+8);

	void *p;
	if (!(p = ffly_brick_get(b))) {
		// error
	}

	ff_net_recv(client.sock, p, len, 0, &err);
	ffly_printf("brick : write -%u, buflen: %u\n", b, len);
}

void static 
bh_bread(void) {
	ff_err_t err;
	ff_u32_t b;
	ff_uint_t len;
	ff_uint_t off;

	b = *(ff_u32_t*)bh_cc;
	len = *(ff_u32_t*)(bh_cc+4);
	off = *(ff_u32_t*)(bh_cc+8);

	void *p;
	if (!(p = ffly_brick_get(b))) {
		// error
	}

	ff_net_send(client.sock, p, len, 0, &err);	
	ffly_printf("brick : read -%u, buflen: %u\n", b, len);
}

void static
bh_bclose(void) {
	ff_err_t err;
	ff_u32_t b;
	b = *(ff_u32_t*)bh_cc;
	ffly_brick_close(b);
	ffly_printf("closing brick -%u\n", b);
}

void static
bh_bexist(void) {
	ff_err_t err;
	ff_u32_t b;
	b = *(ff_u32_t*)bh_cc;
	ff_i8_t res;
	ffly_printf("checking for existance of brick %u\n", b);
	res = ffly_brick_exist(b);
	ff_net_send(client.sock, &res, 1, 0, &err);
}

ff_i8_t static dc;
void bh_disconnect();
__asm__("bh_disconnect:\n\t"
		"movb $0, dc(%rip)\n\t"
		"ret");

static void(*op[])(void) = {
	bh_bnewm,
	bh_bridm,
	bh_bnew,
	bh_brid,
	bh_bopen,
	bh_bwrite,
	bh_bread,
	bh_bclose,
	bh_disconnect,
	bh_bexist
};

ff_uint_t static osz[] = {
	5,
	4,
	1,
	4,
	4,
	12,
	12,
	4,
	0,
	4
};

/*
	operations will be split so

asm labels:

	_r0:
	{funtion routines}

	_r1:

	{brick operations}

	e.g.
	move brick, copy brick, etc the small shit
*/
void static
texec(bh_tapep __t) {
	ff_u8_t *end;
	bh_cc = (ff_u8_t*)__t->text;
	end = bh_cc+__t->len;
	ff_u8_t on;
_again:
	if (bh_cc>=end)
		return;
	on = *(bh_cc++);
	op[on]();
	bh_cc+=osz[on];
	goto _again;
}

void ff_bhs_start(void) {
	ff_err_t err;
	FF_SOCKET *peer;
	socklen_t len;

	FF_SOCKET *sock;

	sock = bh.sock;
	ff_u32_t tsz;
	bh_tapep t;
	struct sockaddr_in cl;
	peer = NULL;
_again:
	ffly_bzero(&cl, sizeof(struct sockaddr_in));
	len = sizeof(struct sockaddr_in);
	if (_err(err = ff_net_listen(sock))) {
		ffly_printf("failed to listen.\n");
		return;
	}
	peer = ff_net_accept(sock, (struct sockaddr*)&cl, &len, &err);
	if (_err(err))
		return;

	ffly_printf("some bigshot has decided to connected.\n");

	client.sock = peer;
	dc = -1;
	while(dc == -1) {
		ff_u32_t tsz;
		ff_net_recv(peer, &tsz, sizeof(ff_u32_t), 0, &err);
		if (_err(err))
			break;
		t = bh_tape_new(tsz);
		ffly_printf("tape size: %u\n", tsz);
		ff_net_recv(peer, t->text, tsz, 0, &err);
		if (_err(err))
			break;
		texec(t);
		bh_tape_destroy(t);
	}

	ffly_printf("thay left.\n");
	ff_net_close(peer);
	peer = NULL;
	goto _again;
_fail:
	if (peer != NULL) {
		ff_net_close(peer);
	}

//	while(to_shut<0);
	return;
}

void ff_bhs_closedown(void) {
	to_shut = 0;
	// wait for okay
	ff_bhs_close();
}

void ff_bhs_cleanup(void) {
	ffly_cistern_de_init(&ctn);
}

void ff_bh_open(ff_bhp __bh) {
	ff_err_t err;
	__bh->sock = ff_net_creat(&err, _NET_PROT_TCP);
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
	ff_err_t err;
	ff_u8_t code;
	code = _bhop_disconnect;
	tape(__bh, &code, 1, &err);
}

void ff_bh_close(ff_bhp __bh) {
	ff_net_shutdown(__bh->sock, SHUT_RDWR);
	ff_net_close(__bh->sock);
}
