# include "allocr.h"
# include "../system/io.h"
# include "../memory/mem_alloc.h"
# include "../system/printf.h"
# include "../data/mem_set.h"
void pr();
void pf();

void t0() {
	void *p0, *p1, *p2, *p3;
	p0 = ffly_alloc(10);
	p1 = ffly_alloc(20);
	p2 = ffly_alloc(30);
	p3 = ffly_alloc(40);
	ffly_free(p0);
	ffly_free(p1);
	ffly_free(p2);
	ffly_free(p3);



	p0 = ffly_alloc(200);
	p1 = ffly_alloc(200);
}

void ts1() {
	void *list[20];
	mdl_u8_t i = 0;
	void *p;
	while(i != 20) {
		p = ffly_alloc((i+1)*10);
		list[i] = ffly_alloc(8);
		ffly_free(p);
		i++;
	}

	i = 0;
	while(i != 20) {
		ffly_free(list[i]);
		i++;
	}
}

# include "../system/vec.h"

struct tsstruct {
	mdl_u64_t i;
	mdl_u64_t a, b, c;
	mdl_u8_t junk[76];
	void *p;
};

void ts2() {
	void *pp = ffly_alloc(1<<5);
	ffly_alloc(1);
	ffly_free(pp);
	struct ffly_vec vec;
	ffly_vec_set_flags(&vec, VEC_AUTO_RESIZE);
	ffly_vec_init(&vec, sizeof(struct tsstruct));
	mdl_u16_t i = 0, n = 2000;
	void *t;
	struct tsstruct *p;
	while(i != n) {
		ffly_vec_push_back(&vec, (void**)&p);
		p->i = i;
		p->a = i>>1;
		p->b = i>>2;
		p->c = i>>3;
		t = ffly_alloc(400);
		p->p = ffly_alloc(100);
		ffly_free(t);
		ffly_mem_set(p->p, '@', 100);
		i++;
	}

	struct tsstruct ts;
	i = 0;
	while(i != n) {
		ffly_vec_pop_back(&vec, &ts);
		ffly_printf("%u{%s}, %u-%u-%u\n", ts.i, ts.i==((n-1)-i)?"ok":"error", ts.a==(((n-1)-i)>>1), ts.b==(((n-1)-i)>>2), ts.c==(((n-1)-i)>>3));
		ffly_free(ts.p);
		i++;
	}
	
	ffly_vec_de_init(&vec);
}

void ts3() {
	void *p0, *p1, *p2, *p3, *p4, *p5, *p6;
	p0 = ffly_alloc(124);
	p1 = ffly_alloc(583);
	p2 = ffly_alloc(212);
	p3 = ffly_alloc(332);
	p4 = ffly_alloc(113);
	p5 = ffly_alloc(27);
	p6 = ffly_alloc(449);


	ffly_free(p0);
	ffly_free(p1);
	ffly_free(p2);
	ffly_free(p3);
	ffly_free(p4);
	ffly_free(p5);
	ffly_free(p6);

	void *p = ffly_alloc(1000);
	ffly_alloc(1);
	ffly_free(p);
}

void ts4() {
	mdl_u8_t i = 20;
	void *p = ffly_alloc((--i)*10);
	while(i != 1) {
		p = ffly_realloc(p, (--i)*10);
	}
}

# include "../data/bzero.h"
void ts5() {
	void *p0, *p1;
	p0 = ffly_alloc(2000);
	ffly_bzero(p0, 2000);
	p1 = ffly_alloc(1);

	ffly_free(p0);

	mdl_u8_t i = 0;

	while(i != 200) {
	void *p2 = ffly_alloc(1940);
	ffly_bzero(p2, 1940);
	void *p3 = ffly_alloc(27);
	ffly_bzero(p3, 27);

	ffly_free(p2);
	ffly_free(p3);
	i++;
	}
}

void ts6() {
	void *p0, *p1, *p2, *p3, *p4, *p5, *p6;
	p0 = ffly_alloc(3000);
	ffly_alloc(1);
	ffly_free(p0);
	ffly_alloc(2950);
	mdl_u8_t i = 0;

	while(i != 200) {
		p0 = ffly_alloc(17);
		ffly_free(p0);
		i++;
	}
}

void ts7() {
	mdl_u8_t const n = 200;
	void *list[n];
	mdl_u8_t i = 0;
	while(i != n) {
		list[i] = ffly_alloc(100);
		ffly_bzero(list[i], 100);
		i++;
	}

	i = 0;
	while(i != n) {
		ffly_free(list[i]);
		i++;
	}
}
# include "../data/mem_set.h"
# include "../system/nanosleep.h"
void ts8() {
	mdl_u16_t const n = 300;
	mdl_u16_t i = 0;
	void *p[n];
	while(i != n) {
		p[i] = ffly_alloc(200);
		//ffly_bzero(p[i], 200);
		ffly_mem_set(p[i], 'A', 200);
		i++;
	}
	void *e;
	e = ffly_alloc(1);
	i = 0;
	while(i != n) {
		//ffly_nanosleep(0, 99999999);
		ffly_free(p[i++]);
		//ffly_printf("\n\n");
	}


	ffly_printf("\n\n\n\n");
	ffly_free(e);
}

void ts9() {
	char *p = (char*)ffly_alloc(100);
	ffly_mem_set(p, 'X', 100);

	mdl_u8_t i = 0;
	p = ffly_realloc(p, 200);

	void *pp = ffly_alloc(30);
	ffly_mem_set(pp, '@', 30);

	char *end = p+200;
	while(p != end) {
		ffly_printf("%u: %c\n", i++, *p);
		p++;
	}
}

# include "../system/buff.h"
void ts10() {
	struct ffly_buff buf;
	ffly_buff_init(&buf, 100, sizeof(mdl_u64_t));

	ffly_buff_de_init(&buf);
}

void ts11() {
	mdl_u8_t const n = 200;
	void *p[n];
	mdl_u8_t i = 0;
	while(i != n) {
		p[i] = ffly_alloc(i+1);
		i++;
	}

	i = 0;
	while(i != n-1) {
		ffly_free(p[i++]);
	}

	void *p0, *p1;
	p0 = ffly_alloc(26000);
	p1 = ffly_alloc(401);
	void *l = ffly_alloc(1);
	ffly_free(p1);
	ffly_free(p0);
	ffly_free(l);
	ffly_free(p[n-1]);
}

void ts12() {
	mdl_uint_t const m = 10, n = 100;
	mdl_uint_t i = 10, c = 0;
	void *p = ffly_alloc(10);
	_again:
	while(i != 10*m) {
		p = ffly_realloc(p, i);
		i += 10;
	}

	while(i != 10) {
		p = ffly_realloc(p, i);
		i -= 10;
	}

	if (c != n-1) {
		c++;
		goto _again;
	}

	ffly_free(p);
}

# include "../system/nanosleep.h"
mdl_uint_t *p;
void* thr0(void *__arg_p) {
	*p = 0xFFF;
	mdl_u8_t i = 0;
	void *p0;
	while(i != 0xFF) {
		p0 = ffly_alloc(12);
		ffly_free(p0);
		ffly_nanosleep(0, 1000);
		i++;
	}
	ffly_printf("Hello\n");
}

# include "../data/str_cpy.h"
# include "../system/thread.h"
/*
# define __USE_GNU
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/mman.h>
# include <sched.h>
# include <unistd.h>
*/
struct r {
	mdl_u64_t a,b,c,d,e,f;
};

void prr(struct r *__r) {
	ffly_printf("%lu, %lu, %lu, %lu, %lu, %lu\n", __r->a, __r->b, __r->c, __r->d, __r->e, __r->f);
}

# include "../ffly_system.h"
# include "../system/nanosleep.h"
# include "../linux/unistd.h"
# include "../dep/str_cpy.h"
void copy(void *__dst, void *__src, mdl_uint_t __bc);
# include "../linux/stat.h"
# include "../rand.h"
# include "../linux/unistd.h"
# include "../linux/mman.h"
# include "../mode.h"
void _start() {
	ffly_ar_init();
	ffly_io_init();

	ffset_mode(_ff_mod_debug);

	mdl_uint_t const n = 60;
	void *list[n];

	void **cur, **end = (void*)((mdl_u8_t*)list+(n*sizeof(void*)));
	mdl_uint_t i, size;
	cur = list;
	while(cur != end) *(cur++) = NULL;

	i = 0;
	while(i != n) {
//		ffly_printf("%u, %u\n", i, n);
		cur = list;
		while(cur != end) {
			if (!*cur) {
				*cur = ffly_alloc(size = ((ffly_rand()%3000)+1));
//				ffly_printf("%u\n", size);
				ffly_bzero(*cur, size);	
				i++;
			}
/*
			if (ffly_rand()%0x1) {
				void **p = (void*)((mdl_u8_t*)list+((ffly_rand()%(n-1))*sizeof(void*)));
				if (*p != NULL) {
					ffly_free(*p);
					*p = NULL;
					i--;
				}
			}
*/
		cur++;
		}
	}

	i = 0;
	while(i != n) {
		if (list[i] != NULL)
			ffly_free(list[i]);
		i++;
	}

/*
	void *p0, *p1, *p2, *p;
	p = ffly_alloc(1);
	p0 = ffly_alloc(2000);
	p1 = ffly_alloc(1500);
	p2 = ffly_alloc(1);
	
	ffly_free(p1);
	ffly_free(p0);
	p0 = ffly_alloc(2500);
*/
	pr();
	pf();
	ffly_arstat();

	ffly_io_closeup();
	ffly_ar_cleanup();
	exit(0);
}

int main(int __argc, char **__argv) {
	ffly_ar_init();
	ffly_io_init();
/*
	ffly_set_ppid(getpid());
	void *p0;
	p0 = ffly_alloc(1020);
	ffly_alloc(1);

	ffly_free(p0);

	struct timespec begin, end;
	clock_gettime(CLOCK_MONOTONIC, &begin);

	p0 = ffly_alloc(1000); 

	clock_gettime(CLOCK_MONOTONIC, &end);
	mdl_u64_t nsec = (end.tv_nsec-begin.tv_nsec)+((end.tv_sec-begin.tv_sec)*1000000000.0);
	ffly_printf("%luns\n", nsec);
*/
/*
	p = (mdl_uint_t*)ffly_alloc(sizeof(mdl_uint_t));
	ffly_tid_t tid;
	ffly_thread_create(&tid, thr0, NULL);
	void *p0;
	mdl_u8_t i = 0;
	while(i != 0xFF) {
		p0 = ffly_alloc(12);
		ffly_free(p0);
		ffly_nanosleep(0, 10000);
		i++;
	}
		
	ffly_thread_wait(tid);


	ffly_thread_cleanup();
	ffly_free(p);
*/
/*
	struct r *rr = (struct r*)ffly_alloc(sizeof(struct r));
	rr->a = 0xf;
	rr->b = 0xff;
	rr->c = 0xfff;
	rr->d = 0xffff;
	rr->e = 0xfffff;
	rr->f = 0xffffff;

	prr(rr);

	mdl_u8_t *pp = (mdl_u8_t*)ffly_alloc(200);
	ffly_mem_set(pp, 0xFF, 200);
	ffly_free(pp);

	rr = (struct r*)ffly_realloc(rr, 2*sizeof(struct r));
	prr(rr);

*/
//	ts12();
	pr();
	pf();
	ffly_io_closeup();
	ffly_ar_cleanup();
}
