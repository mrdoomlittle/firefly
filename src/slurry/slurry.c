# include "slurry.h"
# ifndef __fflib
# include <malloc.h>
# include <sys/socket.h>
# include <string.h>
# include <errno.h>
# else
# include "../string.h"
# include "../malloc.h"
# include "../linux/socket.h"
# include "../stdio.h"
# include "../linux/errno.h"
# endif
void *s_ov[] = {
	NULL
};

static void chd(ff_u8_t *__p, ff_uint_t __n) {
	ff_uint_t i;

	i = 0;
	for(;i != __n;i++) {
		ff_u8_t c;
		c = *(__p+i);
		if ((c>='a' && c<='z') || (c>='A' && c<='Z'))
			printf("%c", c);
	}
	printf("\n");
}

//#define DEBUG
#define SHH 0x01
struct seghdr {
	ff_u16_t n;
	ff_u16_t size;
} __attribute__((aligned(8)));

struct plhdr {
	ff_u8_t size;
} __attribute__((aligned(8)));

#define WTBD 0x01
#define SEGSHFT 16
#define SEGSIZE (1<<SEGSHFT)

#define NSTB 12
#define BUFSIZE (NSTB<<SEGSHFT)

static ff_u8_t buf[BUFSIZE];
static struct seghdr *sh;
static ff_u32_t bn = sizeof(struct plhdr);
static ff_u16_t sn = 0;
static ff_u64_t sent = 0, recved = 0;

static ff_u8_t flags = 0x00;
static ff_u8_t *test_icb[600*600*8];
static ff_uint_t ticbs_sn = 0;
static ff_uint_t ticbs_rv = 0;
ff_s32_t s__send(long long __sock, void *__buf, ff_uint_t __size, ff_u32_t __flags) {
	ff_s32_t res;

//	sent+=__size;

//	memcpy(test_icb+ticbs_sn, __buf, __size);
//	ticbs_sn+=__size;
//	return 1;
//	printf("%lu v %lu, %lu\n", sent, recved, 600*600*8);
	ff_u8_t *p;
	p = (ff_u8_t*)__buf;
	ff_uint_t size;
	size = __size;
_again:
#ifdef DEBUG
	printf("send %u, %u\n", size, __sock);
#endif
	res = send(__sock, p, size, __flags);
	if (res<=0) {
		if (errno == EAGAIN) {
			printf("trying again, %u\n", size);
			goto _again;
		}

		printf("got error, %s\n", strerror(errno));
		return -1;
	}
	p+=res;
	size-=res;
	if (size>0) {
#ifdef DEBUG
		printf("dident send right amount.\n");
#endif
		goto _again;
	}
	return 1;
}

ff_s32_t s__recv(long long __sock, void *__buf, ff_uint_t __size, ff_u32_t __flags) {
	ff_s32_t res;
//	recved+=__size;

//	memcpy(__buf, test_icb+ticbs_rv, __size);
//	ticbs_rv+=__size;
//	return 1;
//	printf("%lu v %lu\n", sent, recved);
	ff_u8_t *p;
	p = (ff_u8_t*)__buf;
	ff_uint_t size;
	size = __size;
_again:
#ifdef DEBUG
	printf("recv %u, %u\n", size, __sock);
#endif
	res = recv(__sock, p, size, __flags);
	if (res<=0) {
		if (errno == EAGAIN) {
			printf("trying again, %u\n", size);
			goto _again;
		}

		printf("got error, %s\n", strerror(errno));
		return -1;
	}
	p+=res;
	size-=res;

	if (size>0) {
#ifdef DEBUG
		printf("dident recv right amount.\n");
#endif
		goto _again;
	}
	return 1;
}

#define segat(__n) ((struct seghdr*)(buf+((__n)<<SEGSHFT)))
#define PL_HDR ((struct plhdr*)(buf+sizeof(struct seghdr)))

#define SHSZ sizeof(struct seghdr)
#define PLHSZ sizeof(struct plhdr)
void static _flush(long long __sock) {
	/*
		if last segment has not been finished add it
	*/
	if (bn>0) {
		sh = segat(sn);
		sh->size = bn;
		sn++;
	}

	struct plhdr *plh;
	plh = PL_HDR;
	plh->size = sn;

	ff_uint_t i;
	i = 0;
	ff_u8_t *out;
	while(i != sn) {
		out = buf+(i<<SEGSHFT);
		sh = (struct seghdr*)out;
		sh->n = i;
#ifdef DEBUG
		printf("out segment: %u, %u : %u\n", i, sh->size, SEGSIZE);
#endif
		s__send(__sock, out, SEGSIZE, 0);
		i++;
	}
	
	bn = sizeof(struct plhdr);
	sn = 0;
	flags &= ~WTBD;
}

# include "../system/nanosleep.h"
ff_s32_t static
_send(void *__buf, ff_uint_t __size, long long __sock) {
	ff_int_t of;
	ff_u8_t *src;
	ff_u8_t *dst;
	src = (ff_u8_t*)__buf;
	ff_uint_t size;
	size = __size;
_btt:
	if (sn>=NSTB) {
#ifdef DEUBG
		printf("flush, %u\n", sn);
#endif
		_flush(__sock);
	}
	if (!size)
		return 0;

	flags |= WTBD;
	of = ((ff_int_t)size)-(ff_int_t)((SEGSIZE-SHSZ)-bn);
	dst = (buf+((sn<<SEGSHFT)+SHSZ))+bn;

	if (of<=0) {
		memcpy(dst, src, size);
		bn+=size;
	} else {			
		ff_uint_t left;
		left = (SEGSIZE-SHSZ)-bn;
		memcpy(dst, src, left);
		src+=left;
		size-=left;

		sh = segat(sn);
		sh->size = bn+left;
		sn++;

		bn = 0;
		goto _btt;
	}
	return 0;
}


static ff_u8_t ibuf[BUFSIZE];
static ff_u16_t cds = 0;
static ff_u16_t is = 0;
static ff_u16_t soff = 0;

ff_s32_t static _in(long long __sock, ff_u8_t *__buf, ff_uint_t __size) {
	ff_u8_t *dst, *src;
	dst = (ff_u8_t*)__buf;
	ff_uint_t n;
	n = __size;
	ff_uint_t size;
#ifdef DEBUG
	printf("--> %u, %u, %u\n", cds, is, soff);
#endif
	_back:
	if (!n)
		return 0;
	if (cds<is) {
	_b:
		sh = (struct seghdr*)(ibuf+(cds<<SEGSHFT));
		src = ((ff_u8_t*)sh)+soff+sizeof(struct seghdr);
		size = sh->size-soff;

		if (n<size) {
			memcpy(dst, src, n);
			soff+=n;
			return 0;
		}

		n-=size;

		if (soff>0) {
			memcpy(dst, src, size);
			dst+=size;
			cds++;
			soff = 0;
			goto _back;
		}

		memcpy(dst, src, size);
		dst+=size;
		cds++;
		goto _back;
	}
	{
	printf("getting more segments.\n");
	ff_u8_t bf[SEGSIZE];
	ff_uint_t i;
	i = 0;
	sh = (struct seghdr*)bf;

	ff_uint_t totsz;
	
	totsz = ~(ff_uint_t)0;
	while(i<totsz) {
		if (s__recv(__sock, bf, SEGSIZE, 0)<=0)
			return -1;
		memcpy(ibuf+(sh->n<<SEGSHFT), bf, SEGSIZE);
#ifdef DEBUG
		chd(bf, SEGSIZE);
		printf("in segment: %u, %u\n", sh->n, sh->size);
#endif
		i++;
		if (!sh->n) {
#ifdef DEBUG
			printf("got segment that contains payload meta info.\n");
#endif
			struct plhdr *plh;
			plh = (struct plhdr*)(bf+sizeof(struct seghdr));
			totsz = plh->size;
#ifdef DEBUG
			printf("total payload size: %u : %u\n", totsz, i);
#endif
		}
	}
	is = i;
	cds = 0;
	soff = sizeof(struct plhdr);
	goto _b;
	}
}

ff_s32_t static
_recv(void *__buf, ff_uint_t __size, long long __sock) {
	s_flush(__sock);
	return _in(__sock, (ff_u8_t*)__buf, __size);
}

void s_flush(long long __sock) {
	if (!(flags&WTBD) && !sn) return;
	_flush(__sock);
}
ff_s32_t(*s_io_send)(void*, ff_uint_t, long long) = _send;
ff_s32_t(*s_io_recv)(void*, ff_uint_t, long long) = _recv;

s_tapep s_tape_new(ff_uint_t __sz) {
	s_tapep t;
	t = (s_tapep)malloc(sizeof(struct s_tape));
	t->text = malloc(__sz);
	t->len = __sz;
	return t;
}

void s_tape_destroy(s_tapep __t) {
	free(__t->text);
	free(__t);
}
