# ifndef __ffly__typo__raise__h
# define __ffly__typo__raise__h
# include "../typo.h"
// get everything ready so we can build shit
void ffly_traise_prime(void);
void ffly_typo_raise(struct typo_glyph*);

ff_u8_t extern *raise_eo_sz;
extern void(**raise_eo)(void);
# endif /*__ffly__typo__raise__h*/
