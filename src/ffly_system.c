# include "ffly_system.h"
# ifndef __ffly_no_sysconf
#   include "system/config.h"
#   include "network/http.h"
# endif
# include <unistd.h>
# include "system/err.h"
# include "system/io.h"
# ifndef __ffly_no_task_pool
#   include "system/task_pool.h"
# endif
# include "system/thread.h"
ffly_err_t ffly_system_init() {
# ifndef __ffly_no_sysconf
    if (access("sys.conf", F_OK) == -1) {
       // download config
        return FFLY_FAILURE;
    }

    ffly_err_t err;
    if (_err(err = ffly_ld_sysconf("sys.conf"))) {
        ffly_fprintf(ffly_err, "failed to load system config.\n");
        return err;
    }
# endif
# ifndef __ffly_no_task_pool
    ffly_task_pool_init(&__ffly_task_pool__, 20);
# endif
    return FFLY_SUCCESS;
}

ffly_err_t ffly_system_de_init() {
# ifndef __ffly_no_task_pool
    ffly_task_pool_cleanup(&__ffly_task_pool__);
# endif
    ffly_thread_cleanup();
    return FFLY_SUCCESS;
}

static pid_t ppid;
static __thread pid_t pid;
void ffly_set_ppid(pid_t __id) {
    ppid = __id;
}
void ffly_setpid() {
    pid = getpid();
}
pid_t ffly_get_ppid(){return ppid;}
pid_t ffly_getpid(){return pid;}
