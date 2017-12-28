# include "ffly_system.h"
pid_t ffly_ppid;
void ffly_set_ppid(pid_t __id) {
    ffly_ppid = __id;
}
