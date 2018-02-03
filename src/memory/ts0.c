# include "../types/err_t.h"
# include <time.h>
# include "allocr.h"
# include <mdlint.h>
# include "../system/io.h"
# include "../data/bzero.h"
# include "../data/mem_set.h"
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
int main(int __argc, char **__argv) {
	ffly_ar_init();
	ffly_io_init();
    struct timespec begin, end;
    mdl_u64_t nsec;
    mdl_uint_t size;

    mdl_u16_t const n = 2000;
    void *list[n];
    mdl_u16_t i = 0;
	void *p = ffly_alloc(1000), *l;

	void *old = NULL;
	l = ffly_alloc(1);
	ffly_free(p);
    while(i != n) {
        size = rand()%600;
        if (size>20) {
        clock_gettime(CLOCK_MONOTONIC, &begin);
        p = ffly_alloc(size);
		if (old != NULL)
			ffly_free(old);
		old = ffly_alloc(200);
        clock_gettime(CLOCK_MONOTONIC, &end);
		ffly_mem_set(p, '@', size);
		ffly_mem_set(old, '@', 200);
        nsec = (end.tv_nsec-begin.tv_nsec)+((end.tv_sec-begin.tv_sec)*1000000000.0);
        ffly_printf("%u,\t\t%luns\n", size, nsec);

        list[i] = ffly_alloc(2);
		*(mdl_u16_t*)list[i] = i;
        ffly_free(p);
        i++;
        }
    }

	i = 0;
	while(i != n) {
		ffly_printf("%s\n", (i == *(mdl_u16_t*)list[i])?"ok":"error");
		ffly_free(list[i++]);
	}

	ffly_free(old);
	ffly_free(l);
    pr();
    pf();
	ffly_io_closeup();
	ffly_ar_cleanup();
/*
    i = 0;
    while(i != n) {
        ffly_free(list[i++]);
    }
*/
}
