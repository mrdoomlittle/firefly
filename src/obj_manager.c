# include "obj_manager.h"
# include "memory/mem_alloc.h"
# include "memory/mem_realloc.h"
# include "memory/mem_free.h"
# include "system/io.h"
# define PAGE_SIZE 0xf
ff_err_t ffly_obj_man_prepare(ffly_obj_manp __obj_man) {
    __obj_man->top = NULL;
    __obj_man->end = NULL;
    __obj_man->off = 0;
    __obj_man->page_c = 0;
}

ff_id_t ffly_obj_man_add(ffly_obj_manp __obj_man, ffly_objp __obj) {
    if (!__obj_man->top) {
        __obj_man->top = (ffly_objpp)__ffly_mem_alloc(PAGE_SIZE*sizeof(ffly_objp));
        __obj_man->page_c++;
        __obj_man->end = __obj_man->top+PAGE_SIZE;
    } else {
        if (__obj_man->top+__obj_man->off >= __obj_man->end) {
            __obj_man->top = (ffly_objpp)__ffly_mem_realloc(__obj_man->top, ((++__obj_man->page_c)*PAGE_SIZE)*sizeof(ffly_objp));
            __obj_man->end = __obj_man->top+(__obj_man->page_c*PAGE_SIZE);
        }
    }

    ff_id_t id = (ff_id_t)__ffly_mem_alloc(sizeof(__ff_id_t));
    *id = __obj_man->off;
    ffly_fprintf(ffly_log, "added object, id: %u\n", *id);
    *(__obj_man->top+__obj_man->off) = __obj;
    __obj_man->off++;
    return id;
}

ffly_objp ffly_obj_man_get(ffly_obj_manp __obj_man, ff_id_t __id) {
    return *(__obj_man->top+*__id);
}

ff_err_t ffly_obj_man_rm(ffly_obj_manp __obj_man, ff_id_t __id) {
}

ff_err_t ffly_obj_man_free(ffly_obj_manp __obj_man) {
    if (__obj_man->top != NULL)
        __ffly_mem_free(__obj_man->top);
}
