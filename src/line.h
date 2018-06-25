# ifndef __ffly__line__h
# define __ffly__line__h
# include "ffint.h"
# include "system/file.h"
extern ff_int_t l_cursor_pos;

# define ffly_l_forward \
	l_cursor_pos++
# define ffly_l_backward \
	l_cursor_pos--

void ffly_l_show(FF_FILE*);
void ffly_l_put(char);
void ffly_l_del(void);
ff_uint_t ffly_l_load(char*);
void ffly_l_reset(void);
# endif /*__ffly__line__h*/
