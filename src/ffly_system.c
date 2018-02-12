# include "ffly_system.h"
# ifndef __ffly_no_sysconf
#   include "system/config.h"
#   include "network/http.h"
# endif
# include "linux/unistd.h"
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
# ifndef __ffly_no_thread
    ffly_thread_cleanup();
# endif
    return FFLY_SUCCESS;
}

static __linux_pid_t ppid;
static __thread __linux_pid_t pid;
void ff_set_ppid(__linux_pid_t __id) {
    ppid = __id;
}
void ff_setpid() {
    pid = getpid();
}
__linux_pid_t ff_get_ppid(){return ppid;}
__linux_pid_t ff_getpid(){return pid;}
