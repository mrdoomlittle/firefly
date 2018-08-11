# include "texture.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "memory/mem_realloc.h"
# include "system/io.h"

ffly_texturep 
ffly_texture_creat(ff_uint_t __size) {
	ffly_texturep tx;

	tx = (ffly_texturep)__ffly_mem_alloc(sizeof(ffly_texture));
	tx->innards = __ffly_mem_alloc(__size*4);
	return tx;
}

void ffly_texture_resize(ffly_texturep __tex, ff_uint_t __size) {
	void **p;
	p = &__tex->innards;

	*p = __ffly_mem_realloc(*p, __size);
}

void ffly_texture_bulldoze(ffly_texturep __tex) {
	__ffly_mem_free(__tex->innards);
	__ffly_mem_free(__tex);
}
