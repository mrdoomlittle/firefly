# include "task.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"

ffly_taskp ffly_alloc_task() {
    return (ffly_taskp)__ffly_mem_alloc(sizeof(struct ffly_task));
}

void ffly_free_task(ffly_taskp __task) {
    __ffly_mem_free(__task);
}
