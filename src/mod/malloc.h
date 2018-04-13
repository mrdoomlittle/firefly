# ifndef __ffly__mod__malloc__h
# define __ffly__mod__malloc__h
# include "../ffint.h"
# define FF_MAL_O_LOC 0x1
# define FF_MAL_O_OSD 0x2
void setmalopt(ff_u8_t);
void* malloc(ff_uint_t);
void free(void*);
void* realloc(void*, ff_uint_t);
# endif /*__ffly__mod__malloc__h*/
