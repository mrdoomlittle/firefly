# include "str_cpy.h"
# include "str_len.h"
# include "mem_cpy.h"
# include "../system/errno.h"
mdl_uint_t ffly_str_cpy(char *__dst, char const *__src) {
    mdl_uint_t l;
    ffly_mem_cpy(__dst, __src, (l = ffly_str_len(__src))+1);
    return l;
}
