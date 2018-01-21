# include "types/err_t.h"
# include "system/err.h"
# include "system/pipe.h"
# include <sys/types.h>
# include <unistd.h>
# include <stdlib.h>
# include "data/str_cpy.h"
# include "system/io.h"
# include "hatch.h"
# define __ffly_debug_enabled
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
char const tmpl[] = "/tmp/hatch.XXXXXX";
mdl_u8_t rcvop(mdl_uint_t __pipe) {
    mdl_u8_t op = 0;
    ffly_printf("recving op.\n");
    ffly_pipe_read(&op, 1, __pipe);
    ffly_printf("recved.\n");
    return op;
} 

ffly_err_t ffly_hatch_run() {
    char file[sizeof(tmpl)];
    ffly_str_cpy(file, tmpl);
    int fd;
    if ((fd = mkstemp(file)) == -1) {
        reterr;
    }

    ffly_printf("key: %s\n", file); 
    mdl_uint_t pipe;
    pipe = ffly_pipe(8, FF_PIPE_CREAT, 0);

    mdl_uint_t shm_id = ffly_pipe_get_shmid(pipe);
    write(fd, &shm_id, sizeof(mdl_uint_t));
    _back:
    ffly_pipe_listen(pipe);
    while(1) {
        mdl_u8_t op = rcvop(pipe);
        if (op == _ffly_ho_meminfo) {
            struct ffly_meminfo info;
            info.used = ffly_mem_alloc_bc-ffly_mem_free_bc;
            ffly_pipe_write(&info, sizeof(struct ffly_meminfo), pipe);  
        } else if (op == _ffly_ho_lsvec) {
        } else if (op == _ffly_ho_disconnect) {
            goto _back;  
        } else if (op == _ffly_ho_shutdown) {
            ffly_printf("recved shutdown command.\n");
            break;
        }
    }

    ffly_pipe_close(pipe);
    unlink(file);
}

ffly_err_t ffmain(int __argc, char **__argv) {
    ffly_hatch_run();
    retok;
}
