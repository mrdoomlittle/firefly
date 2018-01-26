# include "db.h"
# include "system/io.h"
# include "data/str_len.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "memory/mem_realloc.h"

# define blkd_size sizeof(struct ffdb_blkd)
ffly_err_t ffdb_init(ffdbp __db) {
    __db->off = 0;
    __db->top = NULL;
    __db->next = __db->free;
    ffly_map_init(&__db->map, _ffly_map_127);
}

ffly_err_t ffdb_open(ffdbp __db, char const *__file) {
    ffly_err_t err;
    __db->file = ffly_fopen(__file, FF_O_RDWR|FF_O_TRUNC|FF_O_CREAT, FF_S_IRUSR|FF_S_IWUSR, &err);
}

ffly_err_t ffdb_close(ffdbp __db) {
    ffly_fclose(__db->file);
}

void prepare_pile(ffdb_pilep __p) {
    ffly_map_init(&__p->map, _ffly_map_127);
    __p->top = NULL;
}

void destroy_pile(ffdb_pilep __p) {
    ffly_map_de_init(&__p->map);
}

void free_pile(ffdb_pilep __p) {
    destroy_pile(__p);
    __ffly_mem_free(__p);
}

ffly_err_t ffdb_cleanup(ffdbp __db) {
    ffly_map_de_init(&__db->map);
    ffdb_pilep cur = __db->top, prev;
    ffdb_pilep *p;
    if (!cur)
        goto _sk;
    prev = NULL;
    while(cur != NULL) {
        if (prev != NULL)
            free_pile(prev);
        prev = cur;
        cur = cur->next;
    }
    free_pile(prev);
    __db->top = NULL;

    _sk:
    if (__db->next > __db->free) {
        p = __db->free;
        while(p != __db->next) {
            __ffly_mem_free(*p);
            p++;
        }
        __db->next = __db->free; 
    }
}

ffdb_recordp ffdb_creat_record(ffdbp __db, ffdb_pilep __pile, mdl_uint_t __size) { 
    ffdb_recordp p = (ffdb_recordp)__ffly_mem_alloc(sizeof(struct ffdb_record));
    p->prev = NULL;
    p->next = NULL;
    if (!__pile->top)
        __pile->top = p;
    else {
        p->next = __pile->top;
        __pile->top->prev = p;
        __pile->top = p;
    }
    p->p = ffdb_alloc(__db, __size);
    return p;
}

ffdb_recordp ffdb_fetch_record(ffdbp __db, char const *__pile, char const *__name) {
    
}

void ffdb_del_record(ffdbp __db, ffdb_pilep __pile, ffdb_recordp __rec) {
    if (__pile->top == __rec) {
        __pile->top = __rec->next;
        if (__pile->top != NULL)
            __pile->top->prev = NULL;
    } else {
        if (__rec->prev != NULL)
            __rec->prev->next = __rec->next;
        if (__rec->next != NULL)
            __rec->next->prev = __rec->prev;
    }
 
    ffdb_free(__db, __rec->p); 
}

void ffdb_write(ffdbp __db, ffdb_pilep __pile, ffdb_recordp __rec, ffly_off_t __off, void *__buf, mdl_uint_t __size) {
    ffly_fseek(__db->file, __rec->p+__off, FF_SEEK_SET);
    ffly_fwrite(__db->file, __buf, __size);
}

void ffdb_read(ffdbp __db, ffdb_pilep __pile, ffdb_recordp __rec, ffly_off_t __off, void *__buf, mdl_uint_t __size) {
    ffly_fseek(__db->file, __rec->p+__off, FF_SEEK_SET);
    ffly_fread(__db->file, __buf, __size);
}

void ffdb_record_alias(ffdbp __db, ffdb_pilep __pile, char const *__name, ffdb_recordp __p) {
    ffly_map_put(&__pile->map, (mdl_u8_t const*)__name, ffly_str_len(__name), __p);
}

void ffdb_pile_alias(ffdbp __db, char const *__name, ffdb_pilep __p) {
    ffly_map_put(&__db->map, (mdl_u8_t const*)__name, ffly_str_len(__name), __p);
}

ffdb_pilep ffdb_creat_pile(ffdbp __db) {
    ffdb_pilep p;
    if (__db->next > __db->free)
        p = *(--__db->next);
    else
        p = __ffly_mem_alloc(sizeof(struct ffdb_pile));
    p->prev = NULL;
    p->next = NULL;
    if (!__db->top)
        __db->top = p; 
    else {
        p->next = __db->top;
        __db->top->prev = p;
        __db->top = p; 
    }

    prepare_pile(p);
    return p;
}

ffdb_pilep ffdb_fetch_pile(ffdbp __db, char const *__name) {
    ffdb_pilep p;

    ffly_err_t err;
    p = (ffdb_pilep)ffly_map_get(&__db->map, (mdl_u8_t const*)__name, ffly_str_len(__name), &err);
    return p;
}

void ffdb_del_pile(ffdbp __db, ffdb_pilep __pile) {
    if (__pile == __db->top) {
        __db->top = __pile->next;
        if (__db->top != NULL)
            __db->top->prev = NULL;
        goto _sk;
    }

    if (__pile->prev != NULL)
        __pile->prev->next = __pile->next;
    if (__pile->next != NULL)
        __pile->next->prev = __pile->prev;
    __pile->prev = NULL;
    __pile->next = NULL;

    _sk:
    if (__db->next-__db->free < 20) {
        destroy_pile(__pile);
        *(__db->next++) = __pile;
        return;
    } 

    free_pile(__pile);
}

mdl_uint_t ffdb_alloc(ffdbp __db, mdl_uint_t __bc) {
    ffly_off_t off = __db->off+blkd_size;
    __db->off+=__bc+blkd_size;
    return off;
}

void ffdb_free(ffdbp __db, mdl_uint_t __p) {

}

int main() {
    ffly_io_init();
    struct ffdb db;
    ffdb_init(&db);
    ffdb_open(&db, "test.db");

    ffdb_pilep p = ffdb_creat_pile(&db);
    ffdb_pile_alias(&db, "example-pile", p);

    ffdb_recordp rec = ffdb_creat_record(&db, p, sizeof(mdl_uint_t));

    mdl_uint_t val = 21299;
    ffdb_write(&db, p, rec, 0, &val, sizeof(mdl_uint_t));
    val = 0;
    ffdb_read(&db, p, rec, 0, &val, sizeof(mdl_uint_t));
    ffly_printf("%u\n", val);

    p = ffdb_fetch_pile(&db, "example-pile");
    ffdb_del_pile(&db, p);
    ffdb_close(&db);
    ffdb_cleanup(&db);
    ffly_io_closeup();
}
