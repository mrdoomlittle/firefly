# include "ffly_system.h"
# include "system/config.h"
# include <unistd.h>
# include "network/http.h"
# include "system/err.h"
# include "system/io.h"
# include "system/task_pool.h"
# include "system/thread.h"
ffly_err_t ffly_system_init() {
    if (access("sys.conf", F_OK) == -1) {
       // download config
        return FFLY_FAILURE;
    }

    ffly_err_t err;
    if (_err(err = ffly_ld_sysconf("sys.conf"))) {
        ffly_fprintf(ffly_err, "failed to load system config.\n");
        return err;
    }

    ffly_task_pool_init(&__ffly_task_pool__, 20);
    return FFLY_SUCCESS;
}

ffly_err_t ffly_system_de_init() {
    ffly_task_pool_cleanup(&__ffly_task_pool__);
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
