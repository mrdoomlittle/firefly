# ifndef __ffly__lot__h
# define __ffly__lot__h
# include "ffint.h"
# include "physics/body.h"
# include "system/mutex.h"
/*
	x*x*x sized block where objects are grouped into a nice package
	so we dont need to look a large area for it.
*/
// expand
enum {
    _ffly_lotsize_8 = 3,
    _ffly_lotsize_16,
    _ffly_lotsize_32,
    _ffly_lotsize_64,
    _ffly_lotsize_128
};

struct ffly_lot {
    ffly_phy_bodypp top;
	ffly_phy_bodypp end;
	ffly_phy_bodyppp bodies;
    ff_uint_t xl, yl, zl;
    ff_uint_t x, y, z;
	ff_uint_t size;
	ff_mlock_t lock;
	struct ffly_lot ***p;
	/*
        cleanup
    */
	struct ffly_lot *prev;
	struct ffly_lot **bk, *next;
};

typedef struct ffly_lot* ffly_lotp;
typedef ffly_lotp* ffly_lotpp;
typedef ffly_lotpp* ffly_lotppp;

# ifdef __cplusplus
extern "C" {
# endif
ffly_lotp ffly_lot_top(void);
void ffly_lot_add(ffly_lotp, ffly_phy_bodyp);
void ffly_lot_rm(ffly_lotp, ffly_phy_bodyp);
ffly_phy_bodypp ffly_lot_obj(ffly_lotp, ffly_phy_bodyp);
void ffly_lot_prepare(ffly_lotp, ff_uint_t, ff_uint_t, ff_uint_t);
ffly_lotp ffly_lot_alloc(ff_uint_t, ff_uint_t, ff_uint_t);
void ffly_lot_cleanup(void);
void ffly_lot_free(ffly_lotp);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__lot__h*/
