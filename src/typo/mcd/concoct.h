# ifndef __ffly__typo__mcd__concoct__h
# define __ffly__typo__mcd__concoct__h
# include "../../ffint.h"
extern void(*ffly_mcd_cc_write)(void*, ff_uint_t, ff_u32_t);

void ffly_mcd_cc_prime(void);

// concoct it / output to write / make final product
void ffly_mcd_cc(void);

# endif /*__ffly__typo__mcd__concoct__h*/
