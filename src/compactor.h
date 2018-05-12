# ifndef __ffly__compactor__h
# define __ffly__compactor__h
# include "ffint.h"
# include "types.h"
typedef struct ff_compactor {
	void(*put)(ff_u8_t);
	ff_u8_t(*get)();
	void(*reset)();
	ff_i8_t(*at_eof)(void);

	ff_u8_t buf;
	ff_u8_t left;
} *ff_compactorp;

void ff_compact(ff_compactorp);
void ff_decompact(ff_compactorp);
# endif /*__ffly__compactor__h*/
