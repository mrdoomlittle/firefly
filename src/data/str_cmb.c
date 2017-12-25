
# include "str_len.h"
# include "mem_cpy.h"
# include "../memory/mem_alloc.h"
char* ffly_str_cmb(char *__s1, char *__s2, mdl_u8_t __opt) {
    if (!__s1 && !__s2) return NULL;
    mdl_uint_t s1_len = !__s1?0:ffly_str_len(__s1), s2_len = !__s2?0:ffly_str_len(__s2);
    mdl_uint_t n_len = s1_len+s2_len+1;

    char *s = (char*)__ffly_mem_alloc(n_len);
    if (__s1 != NULL) ffly_mem_cpy(s, __s1, s1_len);
    if (__s2 != NULL) ffly_mem_cpy(s+s1+len, __s2, s2_len);
    *(s+n_len-1) = '\0';
    switch(__opt) {
        case _ffly_stc_free_both:
            if (__s1 != NULL) _free(__s1);
            if (__s2 != NULL) _free(__s2);
            break;
        case _ffly_stc_free_lhs:
            if (__s1 != NULL) _free(__s1);
        break;
        case _ffly_stc_free_rhs:
            if (__s2 != NULL) _free(__s2);
        break;
    }
    return s;
}

