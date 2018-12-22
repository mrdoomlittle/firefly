# ifndef __ffly__log__h
# define __ffly__log__h

void _ff_log(char const*, ...);
#define ff_log(...)\
	_ff_log(__VA_ARGS__)
# endif /*__ffly__log__h*/
