# ifndef __ffly__atomic__h
# define __ffly__atomic__h
# include <mdlint.h>
void extern ffly_atomic_add(mdl_u64_t*, mdl_u64_t);
void extern ffly_atomic_sub(mdl_u64_t*, mdl_u64_t);
void extern ffly_atomic_set(mdl_u64_t*, mdl_u64_t);
mdl_u64_t extern ffly_atomic_get(mdl_u64_t*);
void extern ffly_atomic_incr(mdl_u64_t*);
void extern ffly_atomic_decr(mdl_u64_t*);

typedef mdl_u64_t ffly_atomic_u64_t;
typedef mdl_u64_t ffly_atomic_u32_t;
typedef mdl_u64_t ffly_atomic_u16_t;
typedef mdl_u64_t ffly_atomic_u8_t;
typedef mdl_u64_t ffly_atomic_uint_t;
# endif /*__ffly__atomic__h*/
