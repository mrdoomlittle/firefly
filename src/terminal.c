# include "types/err_t.h"
# include "system/err.h"
# include "system/pipe.h"
# include "system/io.h"
# include "data/str_cmp.h"
# include "data/str_dupe.h"
# include "memory/mem_free.h"
# include "system/file.h"
# include "hatch.h"
char* read_part(char **__p, char *__buf) {
    char *p = __buf;
    while(**__p != ' ' && **__p != '\n' && **__p != '\0') *(p++) = *((*__p)++);
    *p = '\0';
    (*__p)++;
    return __buf;
}

char* read_part_and_dupe(char **__p, char *__buf) {
    char *part = read_part(__p, __buf);
    return ffly_str_dupe(part);
}
    
void help() {
    ffly_printf("commands:\n");
    ffly_printf("connect [file], exit\n");
}

void static sndop(mdl_u8_t __op, mdl_uint_t __pipe) {
    ffly_printf("sending op.\n");
    ffly_pipe_write(&__op, 1, __pipe);
    ffly_printf("sent.\n");
}

ffly_err_t ffmain(int __argc, char **__argv) {
    char buf[200];
    char line[200];
    char *p;
    ffly_err_t err;
    mdl_uint_t pipe;
    mdl_i8_t conn = -1;
    while(1) {
        p = line;
        ffly_rdline(line, 200, ffly_in);
        char *part = read_part(&p, buf);
        if (!ffly_str_cmp(part, "meminfo")) {
            sndop(_ffly_ho_meminfo, pipe);
            struct ffly_meminfo info;
            ffly_pipe_read(&info, sizeof(struct ffly_meminfo), pipe);
            ffly_printf("used: %u\n", info.used);
        } else if (!ffly_str_cmp(part, "connect")) {
            char *key = read_part_and_dupe(&p, buf);
            FF_FILE *file = ffly_fopen(key, FF_O_RDONLY, 0, &err);
        
            mdl_uint_t shm_id;
            ffly_fread(file, &shm_id, sizeof(mdl_uint_t));   
            ffly_printf("connecting to pipe with shmid: %u\n", shm_id);
            pipe = ffly_pipe(8, FF_PIPE_SHMM, shm_id);
            ffly_pipe_connect(pipe);
            conn = 0; 
            ffly_fclose(file); 
            __ffly_mem_free(key);
        } else if (!ffly_str_cmp(part, "help"))
            help();
        else if (!ffly_str_cmp(part, "shutdown")) {
            sndop(_ffly_ho_shutdown, pipe);
            conn = -1;
            ffly_pipe_close(pipe);
        } else if (!ffly_str_cmp(part, "exit")) {
            if (!conn)
                sndop(_ffly_ho_disconnect, pipe);

            ffly_printf("goodbye.\n");
            break;
        }
    }

    if (!conn) {
        ffly_pipe_close(pipe);
    }
    retok;
}
