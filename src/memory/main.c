# define __ffly_debug_enabled
# include "allocr.h"
# include "../system/io.h"
# include "../memory/mem_alloc.h"
# include "../system/printf.h"
# include "../dep/mem_set.h"
void pr();
void pf();
/*
void t0() {
	void *p0, *p1, *p2, *p3;
	p0 = ffly_balloc(10);
	p1 = ffly_balloc(20);
	p2 = ffly_balloc(30);
	p3 = ffly_balloc(40);
	ffly_bfree(p0);
	ffly_bfree(p1);
	ffly_bfree(p2);
	ffly_bfree(p3);



	p0 = ffly_balloc(200);
	p1 = ffly_balloc(200);
}
*/
void ts1() {
	void *list[20];
	ff_u8_t i = 0;
	void *p;
	while(i != 20) {
		p = ffly_balloc((i+1)*10);
		list[i] = ffly_balloc(8);
		ffly_bfree(p);
		i++;
	}

	i = 0;
	while(i != 20) {
		ffly_bfree(list[i]);
		i++;
	}
}

# include "../system/vec.h"

struct tsstruct {
	ff_u64_t i;
	ff_u64_t a, b, c;
	ff_u8_t junk[76];
	void *p;
};

void ts2() {
	void *pp = ffly_balloc(1<<5);
	ffly_balloc(1);
	ffly_bfree(pp);
	struct ffly_vec vec;
	ffly_vec_set_flags(&vec, VEC_AUTO_RESIZE);
	ffly_vec_init(&vec, sizeof(struct tsstruct));
	ff_u16_t i = 0, n = 2000;
	void *t;
	struct tsstruct *p;
	while(i != n) {
		ffly_vec_push_back(&vec, (void**)&p);
		p->i = i;
		p->a = i>>1;
		p->b = i>>2;
		p->c = i>>3;
		t = ffly_balloc(400);
		p->p = ffly_balloc(100);
		ffly_bfree(t);
		ffly_mem_set(p->p, '@', 100);
		i++;
	}

	struct tsstruct ts;
	i = 0;
	while(i != n) {
		ffly_vec_pop_back(&vec, &ts);
		ffly_printf("%u{%s}, %u-%u-%u\n", ts.i, ts.i==((n-1)-i)?"ok":"error", ts.a==(((n-1)-i)>>1), ts.b==(((n-1)-i)>>2), ts.c==(((n-1)-i)>>3));
		ffly_bfree(ts.p);
		i++;
	}
	
	ffly_vec_de_init(&vec);
}

void ts3() {
	void *p0, *p1, *p2, *p3, *p4, *p5, *p6;
	p0 = ffly_balloc(124);
	p1 = ffly_balloc(583);
	p2 = ffly_balloc(212);
	p3 = ffly_balloc(332);
	p4 = ffly_balloc(113);
	p5 = ffly_balloc(27);
	p6 = ffly_balloc(449);


	ffly_bfree(p0);
	ffly_bfree(p1);
	ffly_bfree(p2);
	ffly_bfree(p3);
	ffly_bfree(p4);
	ffly_bfree(p5);
	ffly_bfree(p6);

	void *p = ffly_balloc(1000);
	ffly_balloc(1);
	ffly_bfree(p);
}

void ts4() {
	ff_u8_t i = 20;
	void *p = ffly_balloc((--i)*10);
	while(i != 1) {
		p = ffly_brealloc(p, (--i)*10);
	}
}

# include "../dep/bzero.h"
void ts5() {
	void *p0, *p1;
	p0 = ffly_balloc(2000);
	ffly_bzero(p0, 2000);
	p1 = ffly_balloc(1);

	ffly_bfree(p0);

	ff_u8_t i = 0;

	while(i != 200) {
	void *p2 = ffly_balloc(1940);
	ffly_bzero(p2, 1940);
	void *p3 = ffly_balloc(27);
	ffly_bzero(p3, 27);

	ffly_bfree(p2);
	ffly_bfree(p3);
	i++;
	}
}

void ts6() {
	void *p0, *p1, *p2, *p3, *p4, *p5, *p6;
	p0 = ffly_balloc(3000);
	ffly_balloc(1);
	ffly_bfree(p0);
	ffly_balloc(2950);
	ff_u8_t i = 0;

	while(i != 200) {
		p0 = ffly_balloc(17);
		ffly_bfree(p0);
		i++;
	}
}

void ts7() {
	ff_u8_t const n = 200;
	void *list[n];
	ff_u8_t i = 0;
	while(i != n) {
		list[i] = ffly_balloc(100);
		ffly_bzero(list[i], 100);
		i++;
	}

	i = 0;
	while(i != n) {
		ffly_bfree(list[i]);
		i++;
	}
}
# include "../data/mem_set.h"
# include "../system/nanosleep.h"
void ts8() {
	ff_u16_t const n = 300;
	ff_u16_t i = 0;
	void *p[n];
	while(i != n) {
		p[i] = ffly_balloc(200);
		//ffly_bzero(p[i], 200);
		ffly_mem_set(p[i], 'A', 200);
		i++;
	}
	void *e;
	e = ffly_balloc(1);
	i = 0;
	while(i != n) {
		//ffly_nanosleep(0, 99999999);
		ffly_bfree(p[i++]);
		//ffly_printf("\n\n");
	}


	ffly_printf("\n\n\n\n");
	ffly_bfree(e);
}

void ts9() {
	char *p = (char*)ffly_balloc(100);
	ffly_mem_set(p, 'X', 100);

	ff_u8_t i = 0;
	p = ffly_brealloc(p, 200);

	void *pp = ffly_balloc(30);
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
	ffly_buff_init(&buf, 100, sizeof(ff_u64_t));

	ffly_buff_de_init(&buf);
}

void ts11() {
	ff_u8_t const n = 200;
	void *p[n];
	ff_u8_t i = 0;
	while(i != n) {
		p[i] = ffly_balloc(i+1);
		i++;
	}

	i = 0;
	while(i != n-1) {
		ffly_bfree(p[i++]);
	}

	void *p0, *p1;
	p0 = ffly_balloc(26000);
	p1 = ffly_balloc(401);
	void *l = ffly_balloc(1);
	ffly_bfree(p1);
	ffly_bfree(p0);
	ffly_bfree(l);
	ffly_bfree(p[n-1]);
}

void ts12() {
	ff_uint_t const m = 10, n = 100;
	ff_uint_t i = 10, c = 0;
	void *p = ffly_balloc(10);
	_again:
	while(i != 10*m) {
		p = ffly_brealloc(p, i);
		i += 10;
	}

	while(i != 10) {
		p = ffly_brealloc(p, i);
		i -= 10;
	}

	if (c != n-1) {
		c++;
		goto _again;
	}

	ffly_bfree(p);
}

# include "../system/nanosleep.h"
ff_uint_t *p;

void* thr0(void *__arg_p) {
	*p = 0xFFF;
	ff_u8_t i = 0;
	void *p0;
	while(i != 0xFF) {
		p0 = ffly_balloc(12);
		ffly_bfree(p0);
		ffly_nanosleep(0, 1000);
		i++;
	}
	ffly_printf("Hello\n");
}

# include "../dep/str_cpy.h"
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
	ff_u64_t a,b,c,d,e,f;
};

void prr(struct r *__r) {
	ffly_printf("%lu, %lu, %lu, %lu, %lu, %lu\n", __r->a, __r->b, __r->c, __r->d, __r->e, __r->f);
}

# include "../ffly_system.h"
# include "../system/nanosleep.h"
# include "../linux/unistd.h"
# include "../dep/str_cpy.h"
void copy(void *__dst, void *__src, ff_uint_t __bc);
# include "../linux/stat.h"
# include "../rand.h"
# include "../linux/unistd.h"
# include "../linux/mman.h"
# include "../mode.h"
# include "../stdio.h"
# include "../linux/time.h"
# include "mem_alloc.h"
# include "mem_free.h"
# include "mem_realloc.h"
# include "../rat.h"
# include "plate.h"
# include "../system/tls.h"
# include "../malloc.h"
# include "../location.h"
# include "../mal.h"
# include "../ctl.h"
void *pp;
ff_i8_t okay = -1;
void *pot;

ff_uint_t const n = 1204;
void t0(void *__p) {
	ffly_ctl(ffly_malc, _ar_setpot, (ff_u64_t)pot);
	ff_uint_t i = 0;
	while(i != n) {
		pp = ffly_balloc((ffly_rand()%600)+1);
		*(ff_u8_t*)pp = 212;
		okay = 0;
		while(!okay);
		i++;
	}
	ffly_ctl(ffly_malc, _ar_unset, 0);
}

void t1(void *__p) {
	ffly_ctl(ffly_malc, _ar_setpot, (ff_u64_t)pot);
	ff_uint_t i = 0;
	while(i != n) {
		while(okay == -1);
		if (*(ff_u8_t*)pp != 212)
			ffly_printf("error.\n");
		ffly_bfree(pp);
		okay = -1;
		i++;
	}
	ffly_ctl(ffly_malc, _ar_unset, 0);
}
void ffly_dispose(void*, ff_uint_t);

void _start() {
	ffly_tls_new();
	ffly_ar_init();
//	ff_location_init();
	ffly_io_init();
	ffly_thread_init();
	ffset_mode(_ff_mod_debug);
	ff_rat_put(_ff_rat_2);
	ffly_ctl(ffly_malc, _ar_getpot, (ff_u64_t)&pot);
	ff_tid_t ta, tb;
	ffly_thread_create(&ta, t0, NULL);
	ffly_thread_create(&tb, t1, NULL);
	ff_uint_t i = 0;
	while(i != n) {
		ffly_bfree(ffly_balloc((ffly_rand()%600)+1));
		struct arinfo info;
		arinfo(&info);
		ffly_printf("\e[2Juseage: ar{(%u-bytes), (%u-blocks)} : (%u-bytes)\n", info.used, info.block_c, ffly_mem_alloc_bc-ffly_mem_free_bc);
		ffly_fdrain(ffly_out);
		ffly_arstat();
		ffly_nanosleep(0, 1000);
		i++;
	}
	ffly_twait(ta);
	ffly_twait(tb);
/*
	struct ffly_vec vec;
	ffly_vec_set_flags(&vec, VEC_AUTO_RESIZE);
	ffly_vec_init(&vec, 8);

	ff_uint_t i;
	ff_uint_t const n = 210;
	i = 0;
	while(i != n) {
		ff_uint_t *p;
		*((ff_u8_t*)ffly_balloc(1)) = 212;
		ffly_vec_push_back(&vec, (void**)&p);
		*p = i++;
	}


	i  = 0;
	while(i != n) {
		ff_uint_t p;
		ffly_vec_pop_back(&vec, (void*)&p);
		if (p != (n-1)-i)
			ffly_printf(">>> %u\n", p);
		i++;
	}

	ffly_vec_de_init(&vec);
*/
//	ffly_bfree(p2);
/*
	ff_uint_t const n = 1000;
	void *list[n];

	void **cur, **end = list+n;
	ff_uint_t i, size;
	cur = list;
	while(cur != end) *(cur++) = NULL;

	ff_uint_t ac = 0;
	struct timespec begt, endt;

	ff_u64_t nsec = 0, c = 0;
	i = 0;
	while(i != n) {
		cur = list;
		while(cur != end) {
			if (!*cur) {
				size = ((ffly_rand()%287)+20)+(ffly_rand()%287);
				clock_gettime(CLOCK_MONOTONIC, &begt);
				*cur = __ffly_mem_alloc(size);
				clock_gettime(CLOCK_MONOTONIC, &endt);
				*(ff_u8_t*)*cur = 212;
				*cur = __ffly_mem_realloc(*cur, size-10);	
				if (*(ff_u8_t*)*cur != 212)
					ffly_printf("error, got: %u\n", *(ff_u8_t*)*cur);
				nsec+=(endt.tv_nsec-begt.tv_nsec)+((endt.tv_sec-begt.tv_sec)*1000000000);
				i++;
				c++;
			}

		
			ff_uint_t l = 0;

			while(l != 6) {
				void **p = list+(ffly_rand()%(n-1));
				if (p >= end) {
					return;
				}
				if (*p != NULL) {
					__ffly_mem_free(*p);
					*p = NULL;
					i--;
				}
				l++;
			}

			cur++;
		}

		struct arinfo info;
		arinfo(&info);
		ffly_printf("\nuseage: ar{(%u-bytes), (%u-blocks)} : (%u-bytes)\n\n", info.used, info.block_c, ffly_mem_alloc_bc-ffly_mem_free_bc);
		ffly_fdrain(ffly_out);
		ffly_arstat();

		ffly_nanosleep(0, 100000000);
	}
	
	i = 0;
	while(i != n) {
		if (list[i] != NULL)
			__ffly_mem_free(list[i]);
		i++;
	}
*/
/*
	ff_uint_t const n = 100;
	void *list[n];

	ff_uint_t i;
	ff_uint_t ii;
	ii = 0;
	while(ii != 1) {
		i = 0;
		while(i != n) {
			list[i] = ffly_balloc((ffly_rand()%40)+12);
			i++;
		}

		i = 0;
		while(i != n) {
			ffly_bfree(list[i]);
			i++;
		}
		ii++;
	}

	pr();
	pf();
*/
/*
	ff_uint_t const c = 200;
	ff_uint_t i  0;
	void *p0 = ffly_balloc(12);
	void *p1 = ffly_balloc(16);
	void *p2 = ffly_balloc(16);

	ffly_bfree(p0);
	while(i != c) {
	*(ff_u8_t*)p1 = 212;
	ffly_arbl(p1);
	p1 = ffly_arsh(p1, 4);
	ffly_arbl(p1);
	p1 = ffly_argr(p1, 6);
	ffly_arbl(p1);
	if (*(ff_u8_t*)p1 != 212) {
		ffly_printf("error.\n");
	}
	i++;}
	

	pr();
*/
/*
	void *p0, *p1, *p2, *p3;

	p0 = __ffly_mem_alloc(3785);
	p1 = __ffly_mem_alloc(2874);
	p2 = __ffly_mem_alloc(2874);
	p3 = __ffly_mem_alloc(1074);

	__ffly_mem_free(p0);
	__ffly_mem_free(p1);
	__ffly_mem_free(p2);
	pr();
	pf();
*/
/*
	ffly_bfree(p0);
	//ffly_bfree(p1);

	ffly_arsh(p1, 8);

	if (!ffly_argr(p2, 14))
		ffly_printf("failed.\n");
*/
/*
	ff_u8_t i = 0;
	void *list[200];
	while(i != 2) {
		void **cur = list;
		while(cur != list+200) {
			*(cur++) = ffly_plate_alloc();
		}

		while(cur != list) {
			ffly_plate_free(*((cur--)-1));
		}

		i++;
	}
*/
/*
	ff_tid_t id0, id1;
	ffly_thread_create(&id0, t0, NULL);
	ffly_thread_create(&id1, t1, NULL);
	go = 0;
	ffly_thread_wait(id0);
	ffly_thread_wait(id1);
	ffly_thread_cleanup();	
*/
//	void *p;
//	p = ffly_balloc(200);
//
//	ffly_bfree(p);
//	ffly_dispose(p, 200);

//	pr();
//	 pf();
/*
	void *p0, *p1, *p2, *p3;
	p0 = ffly_balloc(8);
	p1 = ffly_balloc(8);
	p2 = ffly_balloc(8);
	p3 = ffly_balloc(8);
	ffly_bfree(p2);
	ffly_bfree(p1);

	p1 = ffly_balloc(13);
//	ffly_arhang();
	pr();
	pf();
//	ffly_arstat();
//	ffly_bfree(p);
*/
	ffly_io_closeup();
	ffly_ar_cleanup();
	ffly_tls_destroy();
	exit(0);
}
/*
int main(int __argc, char **__argv) {
	ffly_ar_init();
	ffly_io_init();
*/
/*
	ffly_set_ppid(getpid());
	void *p0;
	p0 = ffly_balloc(1020);
	ffly_balloc(1);

	ffly_bfree(p0);

	struct timespec begin, end;
	clock_gettime(CLOCK_MONOTONIC, &begin);

	p0 = ffly_balloc(1000); 

	clock_gettime(CLOCK_MONOTONIC, &end);
	ff_u64_t nsec = (end.tv_nsec-begin.tv_nsec)+((end.tv_sec-begin.tv_sec)*1000000000.0);
	ffly_printf("%luns\n", nsec);
*/
/*
	p = (ff_uint_t*)ffly_balloc(sizeof(ff_uint_t));
	ffly_tid_t tid;
	ffly_thread_create(&tid, thr0, NULL);
	void *p0;
	ff_u8_t i = 0;
	while(i != 0xFF) {
		p0 = ffly_balloc(12);
		ffly_bfree(p0);
		ffly_nanosleep(0, 10000);
		i++;
	}
		
	ffly_thread_wait(tid);


	ffly_thread_cleanup();
	ffly_bfree(p);
*/
/*
	struct r *rr = (struct r*)ffly_balloc(sizeof(struct r));
	rr->a = 0xf;
	rr->b = 0xff;
	rr->c = 0xfff;
	rr->d = 0xffff;
	rr->e = 0xfffff;
	rr->f = 0xffffff;

	prr(rr);

	ff_u8_t *pp = (ff_u8_t*)ffly_balloc(200);
	ffly_mem_set(pp, 0xFF, 200);
	ffly_bfree(pp);

	rr = (struct r*)ffly_brealloc(rr, 2*sizeof(struct r));
	prr(rr);

*/
//	ts12();
	//pr();
	//pf();
//	ffly_arstat();
//	ffly_io_closeup();
//	ffly_ar_cleanup();
