# include "shm.h"
# include <sys/shm.h>
# include <sys/stat.h>
# include <unistd.h>
# include "io.h"

// needs testing
# define BASE 21299
# define MAX 20
# define get_obj(__id) (objs+__id)
# define get_key(__id) *(keys+__id)
# define is_opt(__opts, __opt) \
    ((__opts&__opt) == __opt)
/*
    BASE+key = real key
*/
mdl_uint_t static keys[MAX] = {
    0, 1, 2, 3, 4, 5, 6,
    7, 8, 9, 10, 11, 12,
    13, 14, 15, 16, 17, 18,
    19
};

struct obj {
    int seg_id;
    mdl_uint_t size;
    void *p;
};

struct obj static objs[MAX];
mdl_uint_t static bin[MAX];
mdl_uint_t static *end = bin; 
mdl_uint_t static *next = keys;
void* ffly_shmget(mdl_uint_t *__id, mdl_uint_t __size, int __flags, mdl_u8_t __opts) {
    mdl_uint_t id, key;
    if (is_opt(__opts, FF_SHM_MCI)) {
        id = *__id;
        key = get_key(id);   
    } else {
    if (end-bin > 0) {
        id = *(--end);
        key = get_key(id);
    } else {
        id = next-keys;
        key = *(next++);
    } 
    *__id = id;
    }
    struct obj *o = get_obj(id);
    ffly_printf("using key: %u, id: %u\n", BASE+key, id);
    o->seg_id = shmget(BASE+key, __size, __flags);
    if (o->seg_id == -1) {
        ffly_fprintf(ffly_out, "shmget() failure.\n");
        ffly_shm_cleanup(id); 
        return NULL;
    }
    o->size = __size;
    void *p;
    if (!(p = shmat(o->seg_id, NULL, 0))) {
        ffly_fprintf(ffly_out, "shm failure.\n");
    }
    o->p = p;
    ffly_printf("seg_id: %u, ptr: %p\n", o->seg_id, p);
    return p;
}

void ffly_shmdt(mdl_uint_t __id) { 
    shmdt(get_obj(__id)->p);
}

void ffly_shm_free(mdl_uint_t __id) { 
    shmctl(get_obj(__id)->seg_id, IPC_RMID, 0);
}

void ffly_shm_cleanup(mdl_uint_t __id) {
    if (keys+__id == next+1)
        next--;
    else
        *(end++) = __id;
}

/*
int main() {
    ffly_io_init();
    mdl_uint_t id0, id1;
    void *p0 = ffly_shmget(&id0, 8, IPC_CREAT|S_IRWXU|S_IRWXO|S_IRWXG);
    void *p1 = ffly_shmget(&id1, 8, IPC_CREAT|S_IRWXU|S_IRWXO|S_IRWXG);
    ffly_shmdt(id0);
    ffly_shm_free(id0);
    ffly_shm_cleanup(id0);

    ffly_shmdt(id1);
    ffly_shm_free(id1);
    ffly_io_closeup();
}*/
