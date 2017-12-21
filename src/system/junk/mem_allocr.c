# include "mem_allocr.h"
struct mem_blkd {
	mdl_uint_t size, data_size;
	mdl_u8_t *above, *below;
	mdl_u8_t blk_state;
	mdl_uint_t uul;
};

struct uu_blks {
	mdl_u8_t **blks;
	mdl_uint_t page_c, blk_c;
	mdl_uint_t largest_blk;
};

enum blk_state {
	BLK_USED,
	BLK_FREE
};

# define PAGE_SIZE 20
ffly_err_t ffly_mem_allocr_init(struct ffly_mem_allocr *__mem_allocr, mdl_uint_t __stack_size) {
	if ((__mem_allocr->mem_stack = __ffly_mem_alloc(__stack_size)) == NULL) {
		fprintf(stderr, "mem_allocr: failed to alloc mem stack, errno: %d\n", errno);
		return FFLY_FAILURE;
	}

	__mem_allocr->last_blk = NULL;
	__mem_allocr->next_addr = __mem_allocr->mem_stack;
	__mem_allocr->stack_size = __stack_size;
	struct uu_blks *_uu_blks = (struct uu_blks*)(__mem_allocr->uu_blks = __ffly_mem_alloc(sizeof(struct uu_blks)));
	_uu_blks->blks = (mdl_u8_t**)__ffly_mem_alloc(PAGE_SIZE*sizeof(mdl_u8_t*));
	_uu_blks->page_c = 1;
	_uu_blks->blk_c = 0;
	_uu_blks->largest_blk = 0;
}

ffly_err_t ffly_mem_allocr_de_init(struct ffly_mem_allocr *__mem_allocr) {
	__ffly_mem_free(__mem_allocr->mem_stack);
	__ffly_mem_free(((struct uu_blks*)__mem_allocr->uu_blks)->blks);
}

# define __outof_bounds_err(__mem_allocr, __is) \
	if (__mem_allocr->next_addr-__mem_allocr->mem_stack > __mem_allocr->stack_size) { \
		printf("last alloc was out of bounds.\n"); \
		__is = 1; \
	}

ffly_err_t ffly_mem_allocr_alloc(struct ffly_mem_allocr *__mem_allocr, void **__p, mdl_uint_t __bc) {
	*__p = NULL;
	struct uu_blks *_uu_blks = (struct uu_blks*)__mem_allocr->uu_blks;
	printf("------> %u\n", _uu_blks->blk_c);
	if (_uu_blks->blk_c != 0 && _uu_blks->largest_blk >= __bc) {
		mdl_u8_t **itr = _uu_blks->blks;
		mdl_u8_t **end = _uu_blks->blks+_uu_blks->blk_c;
		while(itr != end) {
			struct mem_blkd *blk_desc = (struct mem_blkd*)*itr;
			printf("size: %u\n", blk_desc->data_size);
			if (blk_desc->data_size >= __bc) {
				*__p = (*itr)+sizeof(struct mem_blkd);
				blk_desc->uul = 0;
				blk_desc->blk_state = BLK_USED;
				mdl_uint_t left_over = blk_desc->data_size-__bc;
				if (left_over > 0 && left_over > sizeof(struct mem_blkd)) {
					blk_desc->data_size = __bc;
					blk_desc->size = sizeof(struct mem_blkd)+__bc;
					blk_desc = (struct mem_blkd*)((*itr)+blk_desc->size);

					*blk_desc = (struct mem_blkd) {
						.size = left_over,
						.data_size = left_over-sizeof(struct mem_blkd),
						.above = *itr,
						.below = ((struct mem_blkd*)*itr)->below,
						.blk_state = BLK_FREE,
						.uul = ((itr-_uu_blks->blks)<<1)|1
					};

					((struct mem_blkd*)*itr)->below = (mdl_u8_t*)blk_desc;
					_uu_blks->blks[itr-_uu_blks->blks] = (mdl_u8_t*)blk_desc;
				} else {
					if (itr != end) {
						_uu_blks->blks[itr-_uu_blks->blks] = *(_uu_blks->blks+(--_uu_blks->blk_c));
					} else _uu_blks->blk_c--;
					while(_uu_blks->blk_c < (_uu_blks->page_c-1)*PAGE_SIZE && _uu_blks->page_c > 1) {
						_uu_blks->blks = (mdl_u8_t**)__ffly_mem_realloc(_uu_blks->blks, ((--_uu_blks->page_c)*PAGE_SIZE)*sizeof(mdl_u8_t*));
					}
				}
				return FFLY_SUCCESS;
			}
			itr++;
		}
	}


	mdl_u8_t is = 0;
	__outof_bounds_err(__mem_allocr, is);
	if (is) return FFLY_FAILURE;

	struct mem_blkd *blk_desc = (struct mem_blkd*)__mem_allocr->next_addr;
	__mem_allocr->next_addr += sizeof(struct mem_blkd);
	*__p = __mem_allocr->next_addr;

	*blk_desc = (struct mem_blkd) {
		.size = sizeof(struct mem_blkd)+__bc,
		.data_size = __bc,
		.above = __mem_allocr->last_blk != NULL? (mdl_u8_t*)__mem_allocr->last_blk:NULL,
		.below = NULL,
		.blk_state = BLK_USED,
		.uul = 0
	};

	if (__mem_allocr->last_blk != NULL)
		((struct mem_blkd*)__mem_allocr->last_blk)->below = (mdl_u8_t*)blk_desc;

	__mem_allocr->last_blk = (void*)blk_desc;
	__mem_allocr->next_addr += __bc;
}

ffly_err_t ffly_mem_allocr_free(struct ffly_mem_allocr *__mem_allocr, void *__p) {
	struct mem_blkd *blk_desc = (struct mem_blkd*)((mdl_u8_t*)__p-sizeof(struct mem_blkd));
	struct uu_blks *_uu_blks = (struct uu_blks*)__mem_allocr->uu_blks;

//	mdl_uint_t uul = 0;
	struct mem_blkd *above = (struct mem_blkd*)blk_desc->above;
	struct mem_blkd *below = (struct mem_blkd*)blk_desc->below;

	if (above != NULL) {
		while(above->blk_state == BLK_FREE) {
			printf("free space found above.\n");
			above->size += blk_desc->size;
			above->data_size += blk_desc->size;
			blk_desc = above;
//			if (!(uul&0x1))
//				uul = ((blk_desc->uul>>1)<<1)|1;
//			else
			_uu_blks->blks[blk_desc->uul>>1] = *(_uu_blks->blks+(--_uu_blks->blk_c));
			above = (struct mem_blkd*)blk_desc->above;
			if (above == NULL) break;
		}
	}

	if (below != NULL) {
		while(below->blk_state == BLK_FREE) {
			printf("free space found below.\n");
			blk_desc->below = (mdl_u8_t*)below;
			blk_desc->size += below->size;
			blk_desc->data_size += below->size;

			_uu_blks->blks[below->uul>>1] = *(_uu_blks->blks+(--_uu_blks->blk_c));
			below = (struct mem_blkd*)blk_desc->below;
			if (below == NULL) break;
		}
	}

	printf("freed %u bytes or %u real byte.\n", blk_desc->data_size, blk_desc->size);
	blk_desc->blk_state = BLK_FREE;
	if (blk_desc->data_size > _uu_blks->largest_blk)
		_uu_blks->largest_blk = blk_desc->data_size;

//	if (uul&0x1) {
//		_uu_blks->blks[uul>>1] = (mdl_u8_t*)blk_desc;
//		return FFLY_SUCCESS;
//	}

	while(_uu_blks->blk_c+1 >= _uu_blks->page_c*PAGE_SIZE)
		_uu_blks->blks = (mdl_u8_t**)__ffly_mem_realloc(_uu_blks->blks, ((++_uu_blks->page_c)*PAGE_SIZE)*sizeof(mdl_u8_t*));
/*
	mdl_uint_t new_size = 0;
	if (_uu_blks->blk_c >= (new_size = _uu_blks->page_c*PAGE_SIZE)) {
		_uu_blks->page_c++;
		_uu_blks->blks = (mdl_u8_t**)realloc(_uu_blks->blks, new_size*sizeof(mdl_u8_t*));
	}
*/
	*(_uu_blks->blks+_uu_blks->blk_c) = (mdl_u8_t*)blk_desc;
	_uu_blks->blk_c++;
}
