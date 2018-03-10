# ifndef __ffly__mod__malloc__h
# define __ffly__mod__malloc__h
# include <mdlint.h>
# define FF_MAL_O_LOC 0x1
# define FF_MAL_O_OSD 0x2
void setmalopt(mdl_u8_t);
void* malloc(mdl_uint_t);
void free(void*);
void* realloc(void*, mdl_uint_t);
# endif /*__ffly__mod__malloc__h*/
