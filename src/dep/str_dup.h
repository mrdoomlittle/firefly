# ifndef __ffly__str__dup__h
# define __ffly__str__dup__h
char const* ffly_str_dup(char const*);
char const __inline__ static* ffly_str_dupe(char const *__s) {
	return ffly_str_dup(__s);
}
# endif /*__ffly__str__dup__h*/
