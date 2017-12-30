# include "ffly_system.h"
# include "system/config.h"
# include <unistd.h>
# include "network/http.h"
# include "system/err.h"
# include "system/io.h"
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
    return FFLY_SUCCESS;
}

ffly_err_t ffly_system_de_init() {

}

pid_t ffly_ppid;
void ffly_set_ppid(pid_t __id) {
    ffly_ppid = __id;
}
