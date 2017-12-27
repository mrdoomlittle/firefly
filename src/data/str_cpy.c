# include "str_cpy.h"
# include "str_len.h"
# include "mem_cpy.h"
# include "../system/errno.h"
ffly_err_t ffly_str_cpy(char *__dst, char *__src) {
    ffly_mem_cpy(__dst, __src, ffly_str_len(__src)+1);
    return FFLY_SUCCESS;
}
