# ifndef __ffly__def__h
# define __ffly__def__h

#ifdef __fflib
#ifndef offsetof
#define offsetof(__type, __member) \
	((ff_u64_t)&((__type*)0)->__member)
#endif
#endif

# ifndef FF_ZERO
#	define FF_ZERO 0
# endif
# ifndef NULL
#	define NULL ((void*)0)
# endif

# ifndef ffly_null_id
#	define ffly_null_id NULL
# endif

# ifndef ffly_true
#	define ffly_true 1
# endif

# ifndef ffly_false
#	define ffly_false 0
# endif

# define __force_inline__ __attribute__((always_inline)) inline
# ifndef retnull
#	define retnull return (void*)0;
# endif
# ifndef null
#	define null(__p) (__p == (void*)0)
# endif
# endif /*__ffly__def__h*/
