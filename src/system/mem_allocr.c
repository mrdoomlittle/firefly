# include "mem_allocr.h"
struct mem_blk_desc {
	mdl_uint_t size, data_size;
	mdl_u8_t *above, *below;
	mdl_u8_t blk_state;
	mdl_uint_t uul;
};

struct unused_blks {
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
	struct unused_blks *_unused_blks = (struct unused_blks*)(__mem_allocr->unused_blks = __ffly_mem_alloc(sizeof(struct unused_blks)));
	_unused_blks->blks = (mdl_u8_t**)__ffly_mem_alloc(PAGE_SIZE*sizeof(mdl_u8_t*));
	_unused_blks->page_c = 1;
	_unused_blks->blk_c = 0;
	_unused_blks->largest_blk = 0;
}

ffly_err_t ffly_mem_allocr_de_init(struct ffly_mem_allocr *__mem_allocr) {
	__ffly_mem_free(__mem_allocr->mem_stack);
	__ffly_mem_free(((struct unused_blks*)__mem_allocr->unused_blks)->blks);
}

# define __outof_bounds_err(__mem_allocr, __is) \
	if (__mem_allocr->next_addr-__mem_allocr->mem_stack > __mem_allocr->stack_size) { \
		printf("last alloc was out of bounds.\n"); \
		__is = 1; \
	}

ffly_err_t ffly_mem_allocr_alloc(struct ffly_mem_allocr *__mem_allocr, void **__mptr, mdl_uint_t __bc) {
	struct unused_blks *_unused_blks = (struct unused_blks*)__mem_allocr->unused_blks;
	if (_unused_blks->blk_c != 0 && _unused_blks->largest_blk >= __bc) {
		mdl_u8_t **itr = _unused_blks->blks;
		mdl_u8_t **end = _unused_blks->blks+_unused_blks->blk_c;
		while(itr != end) {
			struct mem_blk_desc *blk_desc = (struct mem_blk_desc*)*itr;
			printf("size: %u\n", blk_desc->data_size);
			if (blk_desc->data_size >= __bc) {
				mdl_uint_t left_over = blk_desc->data_size-__bc;
				if (left_over > 0) {
					blk_desc->data_size = __bc;
					blk_desc->size = sizeof(struct mem_blk_desc)+__bc;
					blk_desc->blk_state = BLK_USED;
					blk_desc = (struct mem_blk_desc*)(itr+blk_desc->size);

					*blk_desc = (struct mem_blk_desc) {
						.size = left_over+sizeof(struct mem_blk_desc),
						.data_size = left_over,
						.above = *itr,
						.below = ((struct mem_blk_desc*)itr)->below,
						.blk_state = BLK_FREE,
						.uul = 0
					};

					_unused_blks->blks[(*itr)-(*_unused_blks->blks)] = (mdl_u8_t*)blk_desc;
				} else {
					if (itr != end)
						_unused_blks->blks[(*itr)-(*_unused_blks->blks)] = *(_unused_blks->blks+(_unused_blks->blk_c--));
					else _unused_blks->blk_c--;
				}

				return FFLY_SUCCESS;
			}
			itr++;
		}
	}


	mdl_u8_t is = 0;
	__outof_bounds_err(__mem_allocr, is);
	if (is) return FFLY_FAILURE;

	struct mem_blk_desc *blk_desc = (struct mem_blk_desc*)__mem_allocr->next_addr;
	__mem_allocr->next_addr += sizeof(struct mem_blk_desc);
	*__mptr = __mem_allocr->next_addr;

	*blk_desc = (struct mem_blk_desc) {
		.size = sizeof(struct mem_blk_desc)+__bc,
		.data_size = __bc,
		.above = __mem_allocr->last_blk != NULL? (mdl_u8_t*)__mem_allocr->last_blk:NULL,
		.below = NULL,
		.blk_state = BLK_USED,
		.uul = 0
	};

	if (__mem_allocr->last_blk != NULL)
		((struct mem_blk_desc*)__mem_allocr->last_blk)->below = (mdl_u8_t*)blk_desc;

	__mem_allocr->last_blk = (void*)blk_desc;
	__mem_allocr->next_addr += __bc;
}

ffly_err_t ffly_mem_allocr_free(struct ffly_mem_allocr *__mem_allocr, void *__mptr) {
	struct mem_blk_desc *blk_desc = (struct mem_blk_desc*)((mdl_u8_t*)__mptr-sizeof(struct mem_blk_desc));
	struct unused_blks *_unused_blks = (struct unused_blks*)__mem_allocr->unused_blks;

	mdl_uint_t uul = 0, is_more = 0x0;
	for (;;) {
		struct mem_blk_desc *above = (struct mem_blk_desc*)blk_desc->above;
		struct mem_blk_desc *below = (struct mem_blk_desc*)blk_desc->below;

		printf("%u\n", is_more);
		if (is_more == 0x3) break;

		if (above != NULL) {
			if (above->blk_state == BLK_FREE) {
				printf("free space found above.\n");
				above->size += blk_desc->size;
				above->data_size += blk_desc->size;
				blk_desc = above;
				if (!(uul & 1))
					uul |= (blk_desc->uul << 1) | 1;
				else
					_unused_blks->blks[blk_desc->uul] = *(_unused_blks->blks+(_unused_blks->blk_c--));
			} else is_more |= 1;
		} else is_more |= 1;

		if (below != NULL) {
			if (below->blk_state == BLK_FREE) {
				printf("free space found below.\n");
				blk_desc->below = (mdl_u8_t*)below;
				blk_desc->size += below->size;
				blk_desc->data_size += below->size;

				_unused_blks->blks[below->uul] = *(_unused_blks->blks+(_unused_blks->blk_c--));
 			} else is_more |= 1 << 1;
		} else is_more |= 1 << 1;
	}

	printf("freed %u bytes or %u real byte.\n", blk_desc->data_size, blk_desc->size);
	blk_desc->blk_state = BLK_FREE;
	if (blk_desc->data_size > _unused_blks->largest_blk)
		_unused_blks->largest_blk = blk_desc->data_size;

	if (uul & 1) {
		_unused_blks->blks[uul >> 1] = (mdl_u8_t*)blk_desc;
		return FFLY_SUCCESS;
	}

	mdl_uint_t new_size = 0;
	if (_unused_blks->blk_c >= (new_size = _unused_blks->page_c*PAGE_SIZE)) {
		_unused_blks->page_c++;
		_unused_blks->blks = (mdl_u8_t**)realloc(_unused_blks->blks, new_size);
	}

	*(_unused_blks->blks+_unused_blks->blk_c) = (mdl_u8_t*)blk_desc;
	_unused_blks->blk_c++;
}
