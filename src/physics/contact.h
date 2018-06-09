# ifndef __ffly__contact__h
# define __ffly__contact__h
# include "../ffint.h"
# include "body.h"

enum {
	_ffly_contact_c0,
	_ffly_contact_c1,
	_ffly_contact_c2,
	_ffly_contact_c3
};

ff_uint_t ffly_phy_contact(ffly_phy_bodyp, ffly_phy_bodyp, ff_i8_t(*)(ff_u8_t, ffly_phy_bodyp, ffly_phy_bodyp));
void ffly_sentinel_cleanup(void);
# endif /*__ffly__contect__h*/
