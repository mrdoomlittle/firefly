# include "mal_track.h"
# include "io.h"
# include "../ffly_def.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
# include "errno.h"

struct blk {
	ff_uint_t ptr_c;
	void **ptrs;
	ff_uint_t *uoc_locs;
	ff_uint_t no_uoc_locs;
};

ff_err_t ffly_mal_track_init(struct ffly_mal_track *__mal_track) {
	__mal_track->table = (void**)ffly_mem_alloc(0x100*sizeof(void*), ffly_true);
	void **itr = __mal_track->table;
	while(itr != __mal_track->table+0x100) {
		*itr = (void*)ffly_mem_alloc(sizeof(struct blk), ffly_true);
		*((struct blk*)*(itr++)) = (struct blk) {
			.ptr_c = 0,
			.ptrs = NULL,
			.uoc_locs = NULL,
			.no_uoc_locs = 0,
		};
	}
	__mal_track->inited = ffly_true;
	__mal_track->lock = FFLY_MUTEX_INIT;
	return FFLY_SUCCESS;
}

ff_err_t ffly_mal_track_de_init(struct ffly_mal_track *__mal_track) {
	void **itr = __mal_track->table;
	while(itr != __mal_track->table+0x100) {
		struct blk *_blk = (struct blk*)*itr;
		if (_blk->ptrs != NULL)
			ffly_mem_free(_blk->ptrs, ffly_true);
		if (_blk->uoc_locs != NULL)
			ffly_mem_free(_blk->uoc_locs, ffly_true);
		ffly_mem_free(*itr, ffly_true);
		itr++;
	}

	ffly_mem_free(__mal_track->table, ffly_true);
	__mal_track->inited = ffly_false;
	return FFLY_SUCCESS;
}

# ifdef __ffly_debug
void ffly_mal_track_dump(struct ffly_mal_track *__mal_track) {
	void **itr = __mal_track->table;
	while(itr != __mal_track->table+0x100) {
		struct blk *_blk = (struct blk*)*itr;

		if (!_blk->ptr_c) {
			itr++;
			continue;
		}

		ff_uint_t loc = 0 ;
		for (;loc != _blk->ptr_c; loc++) {
			ff_u8_t used = 1;
			ff_uint_t *_itr = _blk->uoc_locs;
			for (;_itr != _blk->uoc_locs+_blk->no_uoc_locs;_itr++) {
				if (loc == *_itr) {
					used = 0;
					break;
				}
			}

			ffly_fprintf(ffly_out, "mal_tracker: mem_ptr: %p, point: %d, state: %s\n", _blk->ptrs[loc], loc, used? "used" : "unused");
		}
		itr++;
	}
}
# endif
//void* ffly_mal_track_get_nxt(struct ffly_mal_track *__mal_track) {
/*
	if (__mal_track->nxt_pnt == __mal_track->ptr_c-1) return NULL;
	for (ff_uint_t loc = 0; loc != __mal_track->ptr_c; loc ++) {
		ff_u8_t used = 1;
		for (ff_uint_t uu_point = 0; uu_point != __mal_track->no_uoc_locs; uu_point ++) {
			if (loc == __mal_track->uoc_locs[uu_point]) {used = 0; break;}
		}
		if (used) break;
		if (__mal_track->nxt_pnt == __mal_track->ptr_c-1) return NULL;
		__mal_track->nxt_pnt++;
	}

	void *nxt_ptr = __mal_track->table[__mal_track->nxt_pnt];
	__mal_track->nxt_pnt++;
	return nxt_ptr;
*/
//}

//void ffly_mal_track_reset(struct ffly_mal_track *__mal_track) {
//	__mal_track->nxt_pnt = 0;
//}

ff_err_t ffly_mal_track_update(struct ffly_mal_track *__mal_track, void *__p, void *__np) {
	if (!__mal_track->inited) return FFLY_FAILURE;
	ffly_mutex_lock(&__mal_track->lock);
	struct blk *_blk = *(__mal_track->table+(((ff_u64_t)__p)&0xff));
	if (((ff_u64_t)__p&0xff) != ((ff_u64_t)__np&0xff)) {
		ffly_mal_track_free(__mal_track, __p, ffly_true);
		ffly_mal_track_alloc(__mal_track, __np);
		ffly_mutex_unlock(&__mal_track->lock);
		return FFLY_SUCCESS;
	}

	void **itr = _blk->ptrs;
	for(;itr != _blk->ptrs+_blk->ptr_c;itr++) {
		if (*itr == __p) {*itr = __np;break;}}

	ffly_mutex_unlock(&__mal_track->lock);
	return FFLY_SUCCESS;
}

ff_err_t ffly_mal_track_alloc(struct ffly_mal_track *__mal_track, void *__p) {
	if (!__mal_track->inited) return FFLY_FAILURE;
	ffly_mutex_lock(&__mal_track->lock);

	struct blk *_blk = *(__mal_track->table+(((ff_u64_t)__p)&0xff));
	ff_uint_t loc = 0;
	// get unoccupied location if any
	if (_blk->no_uoc_locs>0) {
		loc = _blk->uoc_locs[_blk->no_uoc_locs-1];
		if (!(_blk->no_uoc_locs-1)) {
			if (_blk->uoc_locs != NULL) {
				ffly_mem_free(_blk->uoc_locs, ffly_true);
				_blk->uoc_locs = NULL;
			}
			_blk->no_uoc_locs--;
		} else {
			if ((_blk->uoc_locs = (ff_uint_t*)ffly_mem_realloc(_blk->uoc_locs, (--_blk->no_uoc_locs)*sizeof(ff_uint_t), ffly_true)) == NULL) {
				ffly_fprintf(ffly_err, "mal_tracker: failed to realloc memory for 'uoc_locs', errno: %d\n", errno);
				return FFLY_FAILURE;
			}
		}
		goto _sk_resize;
	}

	if (!_blk->ptrs) {
		if ((_blk->ptrs = (void**)ffly_mem_alloc(sizeof(void*), ffly_true)) == NULL) {
			ffly_fprintf(ffly_err, "mal_tracker: failed to alloc memory for 'ptrs', errno: %d\n", errno);
			return FFLY_FAILURE;
		}
	} else {
		if ((_blk->ptrs = (void**)ffly_mem_realloc(_blk->ptrs, (_blk->ptr_c+1)*sizeof(void*), ffly_true)) == NULL) {
			ffly_fprintf(ffly_err, "mal_tracker: failed to realloc memory for 'ptrs', errno: %d\n", errno);
			return FFLY_FAILURE;
		}
	}
	loc = _blk->ptr_c;
	_blk->ptr_c++;
_sk_resize:

	*(_blk->ptrs+loc) = __p;
	ffly_mutex_unlock(&__mal_track->lock);
	return FFLY_SUCCESS;
}

ff_err_t ffly_mal_track_free(struct ffly_mal_track *__mal_track, void *__p, ff_u8_t __hard) {
	ff_err_t err;
	if (!__mal_track->inited) return FFLY_FAILURE;
	err = FFLY_SUCCESS;
	ffly_mutex_lock(&__mal_track->lock);
	struct blk *_blk = *(__mal_track->table+(((ff_u64_t)__p)&0xff));

	ff_uint_t loc = 0;
	void **itr = _blk->ptrs;
	while(itr != _blk->ptrs+_blk->ptr_c) {
		ff_uint_t off = itr-_blk->ptrs;
		if (*(itr++) == __p) {
			loc = off;
			break;
		}

		if (off == _blk->ptr_c-1) {
			ffly_fprintf(ffly_err, "mal_tracker: failed to locate memory with ptr: %p.\n", __p);
			err = FFLY_FAILURE;
			goto _end;
		}
	}

	if (__hard) goto _sk_soft;

	if (!_blk->uoc_locs) {
		if ((_blk->uoc_locs = (ff_uint_t*)ffly_mem_alloc(sizeof(ff_uint_t), ffly_true)) == NULL) {
			ffly_fprintf(ffly_err, "mal_tracker: failed to alloc memory for 'uoc_locs', errno: %d\n", errno);
			return FFLY_FAILURE;
		}
	} else {
		if ((_blk->uoc_locs = (ff_uint_t*)ffly_mem_realloc(_blk->uoc_locs, (_blk->no_uoc_locs+1)*sizeof(ff_uint_t), ffly_true)) == NULL) {
			ffly_fprintf(ffly_err, "mal_tracker: failed to realloc memory for 'uoc_locs', errno: %d\n", errno);
			return FFLY_FAILURE;
		}
	}

	_blk->ptrs[loc] = NULL;
	_blk->uoc_locs[_blk->no_uoc_locs] = loc;
	_blk->no_uoc_locs++;
	goto _end;
_sk_soft:
	if (!(_blk->ptr_c-1)) {
		if (_blk->ptrs != NULL) {
			ffly_mem_free(_blk->ptrs, ffly_true);
			_blk->ptrs = NULL;
		}
	} else {
		_blk->ptrs[loc] = _blk->ptrs[_blk->ptr_c-1];
		if ((_blk->ptrs = (void**)ffly_mem_realloc(_blk->ptrs, (_blk->ptr_c-1)*sizeof(void*), ffly_true)) == NULL) {
			ffly_fprintf(ffly_err, "mal_tracker: failed to realloc memory for 'ptrs', errno: %d\n", errno);
			return FFLY_FAILURE;
		}
	}
	_blk->ptr_c--;
_end:
	ffly_mutex_unlock(&__mal_track->lock);
	return err;
}

struct ffly_mal_track __ffly_mal_track__ = {.inited=ffly_false};
