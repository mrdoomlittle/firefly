# include "texture.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "memory/mem_realloc.h"
# include "system/io.h"
# include "bron/tex.h"
#define is_flag(__flags, __flag) \
	(((__flags)&(__flag))>0)
ffly_texturep 
ffly_texture_creat(ff_uint_t __size) {
	ffly_texturep tx;

	tx = (ffly_texturep)__ffly_mem_alloc(sizeof(ffly_texture));
	tx->innards = __ffly_mem_alloc(__size*4);
	tx->flags = 0x00;
	tx->size = __size;
	return tx;
}

void ffly_texture_resize(ffly_texturep __tex, ff_uint_t __size) {
	void **p;
	p = &__tex->innards;

	*p = __ffly_mem_realloc(*p, __size);
}

ff_u16_t ffly_tex_get(ffly_texturep __tex) {
	if (!is_flag(__tex->flags, TX_INO)) {
		// buffer where data will be stored
		__tex->txb = ffly_bron_texbuf_new(__tex->size);

		ffly_bron_texbuf_write(__tex->txb, 0, __tex->size*4, __tex->innards);
		// texture data+info - full package
		__tex->tx = ffly_bron_tex_new(__tex->txb);
		__tex->flags |= TX_INO;
	}

	return __tex->tx;
}

void ffly_texture_bulldoze(ffly_texturep __tex) {
	__ffly_mem_free(__tex->innards);
	__ffly_mem_free(__tex);
}
