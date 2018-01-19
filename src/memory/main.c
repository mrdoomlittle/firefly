# include "allocr.h"
# include "../system/io.h"
# include "../memory/mem_alloc.h"
# include "../system/printf.h"
void pr();
void pf();

void test0() {
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

void test1() {
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
void test2() {
    struct ffly_vec vec;
    ffly_vec_set_flags(&vec, VEC_AUTO_RESIZE);
    ffly_vec_init(&vec, 2);
    mdl_u16_t i = 0;
    mdl_u16_t *p;
    while(i != 800) {
        ffly_vec_push_back(&vec, (void**)&p);
        *p = i;
        i++;
    }

    mdl_u16_t v;
    i = 0;
    while(i != 800) {
        ffly_vec_pop_back(&vec, &v);
        ffly_printf("%u\n", i);
        i++;
    }
    
    ffly_vec_de_init(&vec);
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
# include "../data/mem_set.h"
struct r {
    mdl_u64_t a,b,c,d,e,f;
};

void prr(struct r *__r) {
    ffly_printf("%lu, %lu, %lu, %lu, %lu, %lu\n", __r->a, __r->b, __r->c, __r->d, __r->e, __r->f);
}
# include <unistd.h>
# include "../ffly_system.h"
# include "../system/nanosleep.h"
# include <time.h>
ffly_err_t ffmain(int __argc, char **__argv) {
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
    pr();
    pf();
}
