# ifndef __ffly__graphics__struc__h
# define __ffly__graphics__struc__h
# include "../ffint.h"
struct ffly_tri2 {
	// keep packet as we copy full to bron_tri2
	struct {
		ff_i32_t x0, y0;
		ff_i32_t x1, y1;
		ff_i32_t x2, y2;
	} coords __attribute__((packed));
};

# endif /*__ffly__graphics__struc__h*/
