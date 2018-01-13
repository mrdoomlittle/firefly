# include "thread.h"
# include "task_pool.h"
# include "io.h"
# include "nanosleep.h"
# include "../data/mem_set.h"
# include "../data/mem_cpy.h"
# include "errno.h"
# include "config.h"
# include "nanosleep.h"
void *call(void *__arg_p) {
    ffly_nanosleep(0, 1000000);
    ffly_printf("%u\n", ffly_gettid());
}

int main() {
/*
    __ffly_sysconf__.max_threads = 20;
    ffly_io_init();
    ffly_printf("starting.\n");
    ffly_tid_t tid;
    mdl_uint_t i = 0;   
    while(i != 50) {
        ffly_nanosleep(0, 1000);
        ffly_thread_create(&tid, &call, NULL);
        i++;
    }
    ffly_printf("stopping.\n");

    while(1);
    ffly_io_closeup();
    return 0;
*/
    ffly_io_init();
    __ffly_sysconf__.max_threads = 20;
    ffly_task_pool_init(&__ffly_task_pool__, 20);
    
    mdl_u8_t dst[8000];
    mdl_u8_t src[8000];
    ffly_mem_set(src, 0xFF, sizeof(src));
    //ffly_printf("done.\n");
    //ffly_nanosleep(1, 0);
    if (ffly_mem_cpy(dst, src, sizeof(src)) != FFLY_SUCCESS) {
        ffly_printf("failed.\n");
    }

    mdl_u8_t *itr = dst;
    while(itr != dst+800) {
        ffly_printf("%u\n", *itr);
        itr++;
    }

    ffly_task_pool_cleanup(&__ffly_task_pool__);
    ffly_thread_cleanup();
    ffly_io_closeup();
}
