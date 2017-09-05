# include "is_inside.h"
ffly_bool_t ffly_is_inside(ffly_3d_coords_t *__cord_a, ffly_3d_coords_t *__cord_b, mdl_uint_t __xa_len, mdl_uint_t __ya_len, mdl_uint_t __za_len, mdl_u8_t __flgs) {
	return (((__cord_a->xa >= __cord_b->xa && __cord_a->xa <= __cord_b->xa+__xa_len) || __flgs&0x1)
		&& ((__cord_a->ya >= __cord_b->ya && __cord_a->ya <= __cord_b->ya+__ya_len) || (__flgs&0x2)>>1)
		&& ((__cord_a->za >= __cord_b->xa && __cord_a->za <= __cord_b->za+__za_len) || (__flgs&0x4)>>2));
}
