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

#define SEGEND 0x01
struct seghdr {
	ff_u16_t n;
	ff_u16_t size;
	ff_u8_t flags;
};

#define SEGSHFT 15
#define SEGSIZE (1<<SEGSHFT)

#define NSTB 12
#define BUFSIZE (NSTB<<SEGSHFT)

static ff_u8_t buf[BUFSIZE];
static struct seghdr *sh;
static ff_uint_t bn = sizeof(struct seghdr);
static ff_uint_t sn = 0;
static ff_u64_t sent = 0, recved = 0;


static ff_u8_t *test_icb[20000];
static ff_uint_t ticbs_sn = 0;
static ff_uint_t ticbs_rv = 0;
ff_s32_t s__send(long long __sock, void *__buf, ff_uint_t __size, ff_u32_t __flags) {
	ff_s32_t res;

	sent+=__size;

//	memcpy(test_icb+ticbs_sn, __buf, __size);
//	ticbs_sn+=__size;
//return 1;
	printf("%lu v %lu\n", sent, recved);
_again:
	res = send(__sock, __buf, __size, __flags);
	if (res<=0) {
		if (errno == EAGAIN) {
			printf("trying again, %u\n", __size);
			goto _again;
		}
	}
	return res;
}

ff_s32_t s__recv(long long __sock, void *__buf, ff_uint_t __size, ff_u32_t __flags) {
	ff_s32_t res;
	recved+=__size;

//	memcpy(__buf, test_icb+ticbs_rv, __size);
//	ticbs_rv+=__size;
//return 1;
	printf("%lu v %lu\n", sent, recved);
_again:
	res = recv(__sock, __buf, __size, __flags);
	if (res<=0) {
		if (errno == EAGAIN) {
			printf("trying again, %u\n", __size);
			goto _again;
		}
	}
	return res;
}

void static _flush(long long __sock) {
	sh = (struct seghdr*)(buf+((sn-1)<<SEGSHFT));
	sh->flags = SEGEND;
	ff_uint_t i;
	i = 0;
	ff_u8_t *out;
	while(i != sn) {
		out = buf+(i<<SEGSHFT);
		sh = (struct seghdr*)out;
		sh->n = i;
		printf("out segment: %u, %u\n", i, sh->size);
		s__send(__sock, out, SEGSIZE, 0);
		i++;
	}

	bn = sizeof(struct seghdr);
	sn = 0;
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
	if (sn >= NSTB) {
		printf("flush, %u\n", sn);
		_flush(__sock);
	}
	if (!size)
		return 0;
//	printf("%u, %u\n", size, 0);
	of = ((ff_int_t)size)-(ff_int_t)(SEGSIZE-bn);
	dst = (buf+(sn<<SEGSHFT))+bn;
//	printf("%u, %u, %d\n", bn, size, of);
//	printf("---> %u, %d\n", sn, of);
	if (of<=0) {
		memcpy(dst, src, size);
		bn+=size;
	} else {			
		ff_uint_t left;
		left = SEGSIZE-bn;
		memcpy(dst, src, left);
		bn+=left;
		src+=left;
		size-=left;

		sh = (struct seghdr*)(buf+(sn<<SEGSHFT));
		sh->size = bn-sizeof(struct seghdr);
		sh->flags = 0x00;
		sn++;
		bn = sizeof(struct seghdr);

		goto _btt;
	}
//	if (bn>0)
//		flush(__sock);
	return 0;
}


static ff_u8_t ibuf[BUFSIZE];
static ff_uint_t cds = 0;
static ff_uint_t is = 0;
static ff_uint_t soff = 0;

ff_s32_t static _in(long long __sock, ff_u8_t *__buf, ff_uint_t __size) {
	ff_u8_t *dst, *src;
	dst = (ff_u8_t*)__buf;
	ff_uint_t n;
	n = __size;
	ff_uint_t size;
	printf("--> %u, %u, %u\n", cds, is, soff);
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
			soff = n;
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

	while(1) {
		if (s__recv(__sock, bf, SEGSIZE, 0)<=0)
			return -1;
		chd(bf, SEGSIZE);
		memcpy(ibuf+(sh->n<<SEGSHFT), bf, SEGSIZE);
		printf("in segment: %u, %u\n", sh->n, sh->size);
		i++;
		if ((sh->flags&SEGEND)>0) {
			printf("got segment end.\n");
			break;
		}
	}
	is = i;
	cds = 0;
	soff = 0;
	goto _b;
	}
}

ff_s32_t static
_recv(void *__buf, ff_uint_t __size, long long __sock) {

	return _in(__sock, (ff_u8_t*)__buf, __size);
}

void s_flush(long long __sock) {
	if (bn == sizeof(struct seghdr) && !sn) return;
	if (bn>sizeof(struct seghdr)) {
		((struct seghdr*)(buf+(sn<<SEGSHFT)))->size = bn-sizeof(struct seghdr);
		sn++;
	}
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
