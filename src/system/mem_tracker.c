# include "mem_tracker.h"
# include "io.h"
# include "../ffly_def.h"
struct blk {
	mdl_uint_t ptr_c;
	void **ptrs;
	mdl_uint_t *uoc_locs;
	mdl_uint_t no_uoc_locs;
};

ffly_err_t ffly_mem_track_init(struct ffly_mem_track *__mem_track) {
	__mem_track->table = (void**)ffly_mem_alloc(0xFF*sizeof(void*), ffly_true);
	void **itr = __mem_track->table;
	while(itr != __mem_track->table+0xFF) {
		*itr = (void*)ffly_mem_alloc(sizeof(struct blk), ffly_true);
		*((struct blk*)*(itr++)) = (struct blk) {
			.ptr_c = 0,
			.ptrs = NULL,
			.uoc_locs = NULL,
			.no_uoc_locs = 0,
		};}
	__mem_track->inited = ffly_true;
	__mem_track->mutex = FFLY_MUTEX_INIT;
	return FFLY_SUCCESS;
}
ffly_err_t ffly_mem_track_de_init(struct ffly_mem_track *__mem_track) {
	void **itr = __mem_track->table;
	while(itr != __mem_track->table+0xFF) {
		struct blk *_blk = (struct blk*)*itr;
		if (_blk->ptrs != NULL)
			ffly_mem_free(_blk->ptrs, ffly_true);
		if (_blk->uoc_locs != NULL)
			ffly_mem_free(_blk->uoc_locs, ffly_true);
		ffly_mem_free(*itr, ffly_true);
		itr++;
	}
	ffly_mem_free(__mem_track->table, ffly_true);
	__mem_track->inited = ffly_false;
	return FFLY_SUCCESS;
}

# ifdef __DEBUG_ENABLED
void ffly_mem_track_dump(struct ffly_mem_track *__mem_track) {
	void **itr = __mem_track->table;
	while(itr != __mem_track->table+0xFF) {
		struct blk *_blk = (struct blk*)*itr;

		if (!_blk->ptr_c) {
			ffly_printf(stdout, "empty.\n");
			itr++;
			continue;
		}

		mdl_uint_t loc = 0 ;
		for (;loc != _blk->ptr_c; loc++) {
			mdl_u8_t used = 1;
			mdl_uint_t *_itr = _blk->uoc_locs;
			for (;_itr != _blk->uoc_locs+_blk->no_uoc_locs;_itr++) {
				if (loc == *_itr) {
					used = 0;
					break;
				}
			}

			ffly_printf(stdout, "mem_tracker: mem_ptr: %p, point: %d, state: %s\n", _blk->ptrs[loc], loc, used? "used" : "unused");
		}
		itr++;
	}
}
# endif
//void* ffly_mem_track_get_nxt(struct ffly_mem_track *__mem_track) {
/*
	if (__mem_track->nxt_pnt == __mem_track->ptr_c-1) return NULL;
	for (mdl_uint_t loc = 0; loc != __mem_track->ptr_c; loc ++) {
		mdl_u8_t used = 1;
		for (mdl_uint_t uu_point = 0; uu_point != __mem_track->no_uoc_locs; uu_point ++) {
			if (loc == __mem_track->uoc_locs[uu_point]) {used = 0; break;}
		}
		if (used) break;
		if (__mem_track->nxt_pnt == __mem_track->ptr_c-1) return NULL;
		__mem_track->nxt_pnt++;
	}

	void *nxt_ptr = __mem_track->table[__mem_track->nxt_pnt];
	__mem_track->nxt_pnt++;
	return nxt_ptr;
*/
//}

//void ffly_mem_track_reset(struct ffly_mem_track *__mem_track) {
//	__mem_track->nxt_pnt = 0;
//}

ffly_err_t ffly_mem_track_update(struct ffly_mem_track *__mem_track, void *__p, void *__np) {
	if (!__mem_track->inited) return FFLY_FAILURE;
	ffly_mutex_lock(&__mem_track->mutex);
	struct blk *_blk = *(__mem_track->table+(((mdl_u64_t)__p)&0xFF));
	if (((mdl_u64_t)__p&0xFF) != ((mdl_u64_t)__np&0xFF)) {
		ffly_mutex_unlock(&__mem_track->mutex);

		ffly_mem_track_free(__mem_track, __p, ffly_true);
		ffly_mem_track_alloc(__mem_track, __np);
		return FFLY_SUCCESS;
	}

	void **itr = _blk->ptrs;
	for(;itr != _blk->ptrs+_blk->ptr_c;itr++) {
		if (*itr == __p) {*itr = __np;break;}}

	ffly_mutex_unlock(&__mem_track->mutex);
	__asm__("nop");
	return FFLY_SUCCESS;
}

ffly_err_t ffly_mem_track_alloc(struct ffly_mem_track *__mem_track, void *__p) {
	if (!__mem_track->inited) return FFLY_FAILURE;
	ffly_mutex_lock(&__mem_track->mutex);

	struct blk *_blk = *(__mem_track->table+(((mdl_u64_t)__p)&0xFF));
	mdl_uint_t loc = 0;
	if (_blk->no_uoc_locs != 0) {
		loc = _blk->uoc_locs[_blk->no_uoc_locs-1];
		if (!(_blk->no_uoc_locs-1)) {
			if (_blk->uoc_locs != NULL) {
				ffly_mem_free(_blk->uoc_locs, ffly_true);
				_blk->uoc_locs = NULL;
			}
			_blk->no_uoc_locs--;
		} else {
			if ((_blk->uoc_locs = (mdl_uint_t*)ffly_mem_realloc(_blk->uoc_locs, (--_blk->no_uoc_locs)*sizeof(mdl_uint_t), ffly_true)) == NULL) {
				ffly_printf(stderr, "mem_tracker: failed to realloc memory for 'uoc_locs', errno: %d\n", errno);
				return FFLY_FAILURE;
			}
		}
		goto _sk_resize;
	}

	if (!_blk->ptrs) {
		if ((_blk->ptrs = (void**)ffly_mem_alloc(sizeof(void*), ffly_true)) == NULL) {
			ffly_printf(stderr, "mem_tracker: failed to alloc memory for 'ptrs', errno: %d\n", errno);
			return FFLY_FAILURE;
		}
	} else {
		if ((_blk->ptrs = (void**)ffly_mem_realloc(_blk->ptrs, (_blk->ptr_c+1)*sizeof(void*), ffly_true)) == NULL) {
			ffly_printf(stderr, "mem_tracker: failed to realloc memory for 'ptrs', errno: %d\n", errno);
			return FFLY_FAILURE;
		}
	}
	loc = _blk->ptr_c;
	_blk->ptr_c++;
	_sk_resize:

	*(_blk->ptrs+loc) = __p;
	ffly_mutex_unlock(&__mem_track->mutex);
	__asm__("nop");
	return FFLY_SUCCESS;
}

ffly_err_t ffly_mem_track_free(struct ffly_mem_track *__mem_track, void *__p, mdl_u8_t __hard) {
	if (!__mem_track->inited) return FFLY_FAILURE;
	ffly_mutex_lock(&__mem_track->mutex);
	struct blk *_blk = *(__mem_track->table+(((mdl_u64_t)__p)&0xFF));

	mdl_uint_t loc = 0;
	void **itr = _blk->ptrs;
	while(itr != _blk->ptrs+_blk->ptr_c) {
		mdl_uint_t off = itr-_blk->ptrs;
		if (*(itr++) == __p) {
			loc = off;
			break;
		}

		if (off == _blk->ptr_c-1) {
			ffly_printf(stderr, "mem_tracker: failed to locate memory with ptr: %p.\n", __p);
			return FFLY_FAILURE;
		}
	}

	if (__hard) goto _sk_soft;

	if (!_blk->uoc_locs) {
		if ((_blk->uoc_locs = (mdl_uint_t*)ffly_mem_alloc(sizeof(mdl_uint_t), ffly_true)) == NULL) {
			ffly_printf(stderr, "mem_tracker: failed to alloc memory for 'uoc_locs', errno: %d\n", errno);
			return FFLY_FAILURE;
		}
	} else {
		if ((_blk->uoc_locs = (mdl_uint_t*)ffly_mem_realloc(_blk->uoc_locs, (_blk->no_uoc_locs+1)*sizeof(mdl_uint_t), ffly_true)) == NULL) {
			ffly_printf(stderr, "mem_tracker: failed to realloc memory for 'uoc_locs', errno: %d\n", errno);
			return FFLY_FAILURE;
		}
	}

	_blk->ptrs[loc] = NULL;
	_blk->uoc_locs[_blk->no_uoc_locs] = loc;
	_blk->no_uoc_locs++;
	ffly_mutex_unlock(&__mem_track->mutex);
	__asm__("nop");
	return FFLY_SUCCESS;

	_sk_soft:
	if (!(_blk->ptr_c-1)) {
		if (_blk->ptrs != NULL) {
			ffly_mem_free(_blk->ptrs, ffly_true);
			_blk->ptrs = NULL;
		}
	} else {
		_blk->ptrs[loc] = _blk->ptrs[_blk->ptr_c-1];
		if ((_blk->ptrs = (void**)ffly_mem_realloc(_blk->ptrs, (_blk->ptr_c-1)*sizeof(void*), ffly_true)) == NULL) {
			ffly_printf(stderr, "mem_tracker: failed to realloc memory for 'ptrs', errno: %d\n", errno);
			return FFLY_FAILURE;
		}
	}
	_blk->ptr_c--;
	ffly_mutex_unlock(&__mem_track->mutex);
	__asm__("nop");
	return FFLY_SUCCESS;
}

struct ffly_mem_track __ffly_mem_track__ = {.inited=ffly_false};