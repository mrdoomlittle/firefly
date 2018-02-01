# include "../types/err_t.h"
# include "file.h"
# include "../data/str_len.h"
# include "../types/size_t.h"
# include "printf.h"
# include "io.h"
# include "../data/str_cpy.h"
# include "string.h"
# include <mdlint.h>
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
void ffly_write(int, void*, mdl_uint_t);
ffly_err_t ffly_printf(char const *__format, ...) {
    va_list args;
    va_start(args, __format);
    ffly_vfprintf(ffly_out, __format, args); 
    va_end(args);
}

ffly_err_t ffly_fprintf(FF_FILE *__file, char const *__format, ...) {
    va_list args;
    va_start(args, __format);
    ffly_vfprintf(__file, __format, args);
    va_end(args);
}

ffly_err_t ffly_vfprintf(FF_FILE *__file, char const *__format, va_list __args) {
    mdl_uint_t l = ffly_str_len(__format);
    ffly_vsfprintf(__file, l, __format, __args);
}

mdl_uint_t static gen(char *__buf, ffly_size_t __n, char const *__format, va_list __args) {
    char const *p = __format;
    char *bufp = __buf;
    while(p != __format+__n) {
        if (*(p++) == '%') {
            mdl_u8_t is_long;
            if (is_long = (*p == 'l')) p++; 
            if (*p == 'd') {
                mdl_i64_t v = is_long? va_arg(__args, mdl_i64_t):va_arg(__args, mdl_i32_t);
                mdl_u8_t neg;
                if ((neg = (v < 0))) {
                    *(bufp++) = '-';
                    v = -v;
                } 
                bufp+= ffly_nots(v, bufp);
            } else if (*p == 'u') {
                mdl_u64_t v = is_long? va_arg(__args, mdl_u64_t):va_arg(__args, mdl_u32_t);
                bufp+= ffly_nots(v, bufp);
            } else if (*p == 's') {
                char *s = va_arg(__args, char*);
                bufp+= ffly_str_cpy(bufp, s);  
            } else if (*p == 'c') {
                char c = va_arg(__args, int);
                *(bufp++) = c;      
            } else if (*p == 'f') {
//                double v = va_arg(__args, double);
//                bufp+= ffly_floatts(v, bufp);
            } else if (*p == 'p') {
                void *v = va_arg(__args, void*);
                *(bufp++) = '0';
                *(bufp++) = 'x';
                bufp+= ffly_noths((mdl_u64_t)v, bufp);
            } else if (*p == 'x') {
                mdl_u64_t v = is_long? va_arg(__args, mdl_u64_t):va_arg(__args, mdl_u32_t);
                bufp+= ffly_noths((mdl_u64_t)v, bufp);
            }
            p++;
        } else
            *(bufp++) = *(p-1);
    }
    *bufp = '\0';
    return bufp-__buf;
}

mdl_uint_t ffly_vsprintf(char *__buf, char const *__format, va_list __args) {
    return gen(__buf, ffly_str_len(__format), __format, __args);
}

mdl_uint_t ffly_sprintf(char *__buf, char const *__format, ...) {
    va_list args;
    mdl_uint_t l;
    va_start(args, __format);
    l = gen(__buf, ffly_str_len(__format), __format, args); 
    va_end(args);
    return l;
}

ffly_err_t ffly_vsfprintf(FF_FILE *__file, ffly_size_t __n, char const *__format, va_list __args) {
# ifdef __ffly_debug
    char buf[1024];
# else
    char *buf = (char*)__ffly_mem_alloc(1024);
# endif
    mdl_uint_t l = gen(buf, __n, __format, __args);
    ffly_write(ffly_fileno(__file), buf, l);
# ifndef __ffly_debug
    __ffly_mem_free(buf);
# endif
}

/*
int main() {
    ffly_io_init();
    mdl_u8_t x;
    ffly_fprintf(ffly_out, "test: %p\n", &x);
    ffly_io_closeup();
}*/

