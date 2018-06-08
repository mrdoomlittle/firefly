# include "uni.h"
# include "system/errno.h"
# include "system/io.h"
# include "system/error.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "dep/mem_set.h"
# include "physics/light.h"
/*
	TODO:
		dealloc 'lots' that are not inuse
*/

# define is_sliceable(__no, __by) \
	!(__no-((__no>>__by)*(1<<__by)))

ff_uint_t static
get_cnk_off(ffly_unip __uni, ff_uint_t __cnk_no) {
	return __cnk_no*(1<<__uni->splice);
}

ff_uint_t static
get_cnk_no(ffly_unip __uni, ff_uint_t __off) {
	return __off>>__uni->splice;
}

ffly_chunkp static
get_chunk(ffly_unip __uni, ff_uint_t __x, ff_uint_t __y, ff_uint_t __z) {
	return ffly_cnk_man_fetch(&__uni->chunk_man, ffly_uni_chunk(__uni, __x, __y, __z));
}

ffly_lotpp static
get_lot(ffly_unip __uni, ff_uint_t __x, ff_uint_t __y, ff_uint_t __z) {
	ffly_chunkp chunk = get_chunk(__uni, __x, __y, __z);
	return ffly_fetch_lot(chunk, __x, __y, __z);
}

ffly_lotp ffly_uni_get_lot(ffly_unip __uni, ff_uint_t __x, ff_uint_t __y, ff_uint_t __z) {
	return *get_lot(__uni, __x, __y, __z);
}

ff_err_t
ffly_uni_body_move(ffly_unip __uni, ffly_phy_bodyp __body, ff_uint_t __x, ff_uint_t __y, ff_uint_t __z) {
	ffly_uni_detach_body(__uni, __body);
	*__body->x = __x;
	*__body->y = __y;
	*__body->z = __z;
	ffly_uni_attach_body(__uni, __body);
}

/* should to rename to copy or somthing along that line. 
*/
ff_err_t
ffly_uni_frame(ffly_unip __uni, ff_byte_t *__dst,
	ff_uint_t __xl, ff_uint_t __yl, ff_uint_t __zl,
	ff_uint_t __x, ff_uint_t __y, ff_uint_t __z)
{
	ff_uint_t cnk_xl = ffly_uni_chunk_xal(__uni);
	ff_uint_t cnk_yl = ffly_uni_chunk_yal(__uni);
	ff_uint_t cnk_zl = ffly_uni_chunk_zal(__uni);

	ff_uint_t x = __x, y = __y, z = __z;
	ffly_chunkp chunk;
	while(z < __z+__zl) {
		y = __y;
		while(y < __y+__yl) {
			x = __x;
			while(x < __x+__xl) {
				chunk = get_chunk(__uni, x, y, z);
				ffly_lotp lot = *ffly_fetch_lot(chunk, x, y, z);
				if (lot != NULL) {
					ffly_phy_bodypp itr = lot->top;
					ffly_phy_bodyp body;
					while(itr != lot->end) {
						body = *itr;
						if ((*body->x < __x+__xl && *body->x >= __x) && (*body->y < __y+__yl && *body->y >= __y) && (*body->z < __z+__zl && *body->z >= __z)) {
							ffly_draw_polygon(&body->shape, __dst, body->texture, body->xl, *body->x-__x, *body->y-__y, 0, __xl, __yl, __x+__xl, __y+__yl, 0.0);
							//ffly_light_emit(__dst, __xl, __yl, *body->x-__x, *body->y-__y, 0, *body->light);
						}
						itr++;
					}
				}
				x+= 1<<chunk->lotsize;
			}
			y+= 1<<chunk->lotsize;
		}
		z+= 1<<chunk->lotsize;
	}
}

ff_id_t ffly_uni_chunk(ffly_unip __uni, ff_off_t __x, ff_off_t __y, ff_off_t __z) {
	return *(__uni->chunks+(__x>>__uni->splice)+((__y>>__uni->splice)
		*__uni->xcnk_c)+((__z>>__uni->splice)*(__uni->ycnk_c*__uni->xcnk_c)));
}

ff_err_t ffly_uni_free(ffly_unip __uni) {
	ffly_cnk_man_de_init(&__uni->chunk_man);
	__ffly_mem_free(__uni->chunks);
}

ff_err_t
ffly_uni_attach_body(ffly_unip __uni, ffly_phy_bodyp __body) {
	ff_uint_t x = *__body->x;
	ff_uint_t y = *__body->y;
	ff_uint_t z = *__body->z;
	if (x>=__uni->xl || y>=__uni->yl || z>=__uni->zl) {
		ffly_fprintf(ffly_err, "body is out of bounds, can't attach.\n");
		reterr;
	}

	/*
		later we will deallocate lots that are not in use
	*/
	ffly_lotpp lot = get_lot(__uni, x, y, z);
	if (!*lot) {
		ffly_fprintf(ffly_log, "new lot.\n");
		ffly_chunkp chunk = get_chunk(__uni, x, y, z);
		*lot = ffly_lot_alloc(1<<chunk->lotsize, 1<<chunk->lotsize, 1<<chunk->lotsize);
		ffly_lot_prepare(*lot, (x>>chunk->lotsize)*(1<<chunk->lotsize),
			(y>>chunk->lotsize)*(1<<chunk->lotsize),
			(z>>chunk->lotsize)*(1<<chunk->lotsize));
	}

//	  ffly_fprintf(ffly_log, "added to lot.\n");
	ffly_lot_add(*lot, __body);
	__body->lot = *lot;
	retok;
}
	
ff_err_t ffly_uni_detach_body(ffly_unip __uni, ffly_phy_bodyp __body) {
	if (!__uni || !__body) {
		ffly_fprintf(ffly_err, "failed to detatch body.\n");
		reterr;
	}
	ffly_lot_rm(__body->lot, __body);
	retok;
}

# include "gravity.h"
void
ffly_uni_update(ffly_unip __uni, ff_uint_t __delta) {
	ffly_phy_bodyp cur = ffly_phy_body_top();
	while(cur != NULL) {
		ffly_gravity_apply(__uni, cur, __delta);
		ffly_phy_body_fd(&cur);
	}
}

ff_err_t
ffly_uni_build(ffly_unip __uni, ff_uint_t __xl, ff_uint_t __yl, ff_uint_t __zl, ff_u8_t __splice, ff_u8_t __lotsize) {
	if (!is_sliceable(__xl, __splice)) {
		ffly_fprintf(ffly_err, "length of x not sliceable by %u.\n", 1<<__splice);
		return FFLY_FAILURE;
	}

	if (!is_sliceable(__yl, __splice)) {
		ffly_fprintf(ffly_err, "length of y not sliceable by %u.\n", 1<<__splice);
		return FFLY_FAILURE;

	}

	if (!is_sliceable(__zl, __splice)) {
		ffly_fprintf(ffly_err, "length of z sliceable by %u.\n", 1<<__splice);
		return FFLY_FAILURE;
	}
	
	ffly_cnk_man_init(&__uni->chunk_man, 1<<__splice, 1<<__splice, 1<<__splice);
	__uni->xcnk_c = __xl>>__splice;
	__uni->ycnk_c = __yl>>__splice;
	__uni->zcnk_c = __zl>>__splice;
	ffly_fprintf(ffly_log, "building univirse, xcnk %u, ycnk %u, zcnk %u\n", __uni->xcnk_c, __uni->ycnk_c, __uni->zcnk_c);
	ff_uint_t size = __uni->xcnk_c*__uni->ycnk_c*__uni->zcnk_c;
	__uni->chunks = (ff_id_t*)__ffly_mem_alloc(size*sizeof(ff_id_t));
	__uni->chunk_c = 0;
	ff_uint_t xcnk, ycnk, zcnk = 0;
	while(zcnk != __uni->zcnk_c) {
		ycnk = 0;
		while(ycnk != __uni->ycnk_c) {
			xcnk = 0;
			while(xcnk != __uni->xcnk_c) {
				ff_id_t id;
				ffly_cnk_man_create(&__uni->chunk_man, &id, xcnk*(1<<__splice), ycnk*(1<<__splice), zcnk*(1<<__splice), __lotsize);
				*(__uni->chunks+xcnk+(ycnk*__uni->xcnk_c)+(zcnk*(__uni->ycnk_c*__uni->xcnk_c))) = id;
				xcnk++;
				__uni->chunk_c++;
			}
			ycnk++;
		}
		zcnk++;
	}
	__uni->splice = __splice;
	return FFLY_SUCCESS;
}
