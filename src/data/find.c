# include "find.h"
# include "mem_cmp.h"
# include "../ffly_def.h"
void* ffly_find(void *__p, void *__d, ffly_size_t __blk_size, ffly_size_t __size) {
	mdl_u8_t *itr = (mdl_u8_t*)__p;
	while(itr != ((mdl_u8_t*)__p)+(__size*__blk_size)) {
		if (ffly_mem_cmp(itr, __d, __blk_size))
			return (void*)itr;
		itr+=__blk_size;
	}
	return NULL;
}
