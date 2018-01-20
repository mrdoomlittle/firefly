# include "../types/err_t.h"
# include <time.h>
# include "allocr.h"
# include <mdlint.h>
# include "../system/io.h"
mdl_uint_t no = 0;
mdl_uint_t rand() {
    mdl_uint_t ret;
    ret = no;
    no = ((no<<1)|0x1)+7;
    no = no^((no>>8)&0xff);
    no = no&0xffff;
    return ret; 
}

void pr();
void pf();
ffly_err_t ffmain(int __argc, char **__argv) {
    struct timespec begin, end;
    mdl_u64_t nsec;
    mdl_uint_t size;

    mdl_u8_t const n = 0xf;
    void *list[n];
    mdl_u8_t i = 0;
    void *p;
    while(i != n) {
        size = rand()%80;
        if (size>0) {
        clock_gettime(CLOCK_MONOTONIC, &begin);
        p = ffly_alloc(size);
        clock_gettime(CLOCK_MONOTONIC, &end);
        nsec = (end.tv_nsec-begin.tv_nsec)+((end.tv_sec-begin.tv_sec)*1000000000.0);
        ffly_printf("%u,\t\t%luns\n", size, nsec);
        list[i] = ffly_alloc(20);
        ffly_free(p);
        i++;
        }
    }

    pr();
    pf();
/*
    i = 0;
    while(i != n) {
        ffly_free(list[i++]);
    }
*/
}
