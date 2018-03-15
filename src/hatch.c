# include "types/err_t.h"
# include "system/err.h"
# include "system/pipe.h"
# include "linux/types.h"
# include "linux/unistd.h"
# include "data/str_cpy.h"
# include "system/io.h"
# include "hatch.h"
# define __ffly_debug_enabled
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/vec.h"
# include "system/map.h"
# include "system/flags.h"
# include "lib.h"
char const tmpl[] = "/tmp/hatch.XXXXXX";
mdl_u8_t rcvop(mdl_uint_t __pipe) {
    mdl_u8_t op = 0;
    ffly_printf("recving op.\n");
    op = ffly_pipe_rd8l(__pipe);
    ffly_printf("recved.\n");
    return op;
} 

# define flg_sep if (p != buf) *p++ = ',';
char* vec_flags(ffly_vecp __vec) {
    char static buf[200];
    char *p = buf;
    if (ffly_is_flag(__vec->flags, VEC_AUTO_RESIZE))
        p+=ffly_str_cpy(p, "auto-resize");
    if (ffly_is_flag(__vec->flags, VEC_BLK_CHAIN)) {
        flg_sep
        p+=ffly_str_cpy(p, "block-chain");
    }
    if (ffly_is_flag(__vec->flags, VEC_UUU_BLKS)) {
        flg_sep
        p+=ffly_str_cpy(p, "uuu-blocks");
    }
    if (ffly_is_flag(__vec->flags, VEC_NONCONTINUOUS)) {
        flg_sep
        p+=ffly_str_cpy(p, "noncontinuous");
    }
    *p = '\0';
    return buf;
}

void static lsvec(mdl_uint_t __pipe) {
    ffly_vecp p;
    if (!(p = ffly_vec_list())) {
        ffly_printf("nothing to be sent.\n");
        ffly_pipe_wr8l(-1, __pipe);
        return;
    }

    ffly_pipe_wr8l(0, __pipe);
    mdl_uint_t l = 1024;
    char *buf = (char*)__ffly_mem_alloc(l);
    ffly_pipe_write(&l, sizeof(mdl_uint_t), __pipe);
    mdl_uint_t i = 0;
    while(p != NULL) {
        ffly_sprintf(buf, "vec %u; size: %u, page_c: %u, blk_size: %u - off: %u, flags: %s\n", i, p->size, p->page_c, p->blk_size, p->off, vec_flags(p));
        ffly_printf("sent: %s\n", buf);
        ffly_pipe_write(buf, l, __pipe);
        p = vec_next(p);
        if (!p)
            ffly_pipe_wr8l(-1, __pipe); 
        else
            ffly_pipe_wr8l(0, __pipe);
    } 

    __ffly_mem_free(buf);
}

void static lsmap(mdl_uint_t __pipe) {

}

ffly_err_t ffly_hatch_run() {
    char file[sizeof(tmpl)];
    ffly_str_cpy(file, tmpl);
    int fd;
    if ((fd = ff_mktemp(file)) == -1) {
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
        if (op == _ffly_ho_login) {
            /*
                recv hashed password then send a session key
            */
        } else if (op == _ffly_ho_logout) {
            
        } else if (op == _ffly_ho_meminfo) {
            struct ffly_meminfo info;
            info.used = ffly_mem_alloc_bc-ffly_mem_free_bc;
            ffly_pipe_write(&info, sizeof(struct ffly_meminfo), pipe);  
        } else if (op == _ffly_ho_disconnect) {
            goto _back;  
        } else if (op == _ffly_ho_shutdown) {
            ffly_printf("recved shutdown command.\n");
            break;
        } else {
            switch(op) {
                case _ffly_ho_lsvec: lsvec(pipe); break;
                case _ffly_ho_lsmap: lsmap(pipe); break;
            }
        }
    }

    ffly_pipe_close(pipe);
    unlink(file);
}

# include "system/vec.h"
ffly_err_t ffmain(int __argc, char **__argv) {
    ffly_err_t err;
    ffly_vecp p = ffly_vec(1, VEC_AUTO_RESIZE|VEC_NONCONTINUOUS, &err);
    ffly_hatch_run();
    ffly_vec_destroy(p);
    retok;
}
