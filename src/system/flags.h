# ifndef __ffly__flags__h
# define __ffly__flags__h
# include "../ffint.h"
# include "../types.h"
# define TW_FLG_ACTIVE 0x1
# define ffly_is_flag(__flags, __flag) \
	(((__flags)&(__flag))==(__flag))
# define ffly_add_flag(__flags, __flag) \
	__flags |= __flag
# define ffly_rm_flag(__flags, __flag) \
	__flags ^= (__flags)&(__flag)
# endif /*__ffly__flags__h*/
