# include "thread.h"
# include "task_pool.h"
# include "io.h"
# include "nanosleep.h"
# include "../data/mem_set.h"
# include "../data/mem_cpy.h"
# include "errno.h"
int main() {
    ffly_io_init();
    ffly_task_pool_init(&__ffly_task_pool__, 20);
    
    mdl_u8_t dst[800];
    mdl_u8_t src[800];
    ffly_mem_set(src, 0xFF, sizeof(src));
    if (ffly_mem_cpy(dst, src, sizeof(src)) != FFLY_SUCCESS) {
        ffly_printf("failed.\n");
    }
 
    mdl_u8_t *itr = dst;
    while(itr != dst+800) {
        ffly_printf("%u\n", *itr);
        itr++;
    }

    ffly_nanosleep(0, 100000000);
    ffly_task_pool_cleanup(&__ffly_task_pool__);
    ffly_thread_cleanup();
    ffly_io_closeup();
}
