# ifndef __ffly__task__h
# define __ffly__task__h
# include <mdlint.h>
# include "mutex.h"
struct ffly_task {
    ffly_mutex_t mutex;
    struct ffly_task *prev, *next;
    void *arg_p;
    mdl_i8_t(*call)(void*);
};

typedef struct ffly_task* ffly_taskp;
# ifdef __cplusplus
extern "C" {
# endif
ffly_taskp ffly_alloc_task();
void ffly_free_task(ffly_taskp);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__task__h*/
