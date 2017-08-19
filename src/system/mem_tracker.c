# include "mem_tracker.h"
# include "io.h"
ffly_err_t ffly_mem_track_init(struct ffly_mem_track *__mem_track) {
	__mem_track->mptr_lst = NULL;
	__mem_track->lst_size = 0;
	__mem_track->unused_pnts = NULL;
	__mem_track->unused_pnts_c = 0;
	__mem_track->nxt_pnt = 0;
	return FFLY_SUCCESS;
}

ffly_err_t ffly_mem_track_de_init(struct ffly_mem_track *__mem_track) {
	if (__mem_track->mptr_lst != NULL) {
		ffly_mem_free(__mem_track->mptr_lst, 1);
		__mem_track->mptr_lst = NULL;
	}

	if (__mem_track->unused_pnts != NULL) {
		ffly_mem_free(__mem_track->unused_pnts, 1);
		__mem_track->unused_pnts = NULL;
	}
	return FFLY_SUCCESS;
}

# ifdef __DEBUG_ENABLED
void ffly_mem_track_dmp_ptr_lst(struct ffly_mem_track *__mem_track) {
	if (!__mem_track->lst_size) {
		ffly_printf(stdout, "empty.\n");
		return;
	}

	for (mdl_uint_t lst_loc = 0; lst_loc != __mem_track->lst_size; lst_loc ++) {
		mdl_u8_t used = 1;
		for (mdl_uint_t uu_point = 0; uu_point != __mem_track->unused_pnts_c; uu_point ++) {
			if (lst_loc == __mem_track->unused_pnts[uu_point]) {used = 0; break;}
		}

		ffly_printf(stdout, "mem_tracker: mem_ptr: %p, point: %d, state: %s\n", __mem_track->mptr_lst[lst_loc], lst_loc, used == 1? "used" : "unused");
	}
}
# endif

void* ffly_mem_track_get_nxt(struct ffly_mem_track *__mem_track) {
	if (__mem_track->nxt_pnt == __mem_track->lst_size-1) return NULL;
	for (mdl_uint_t lst_loc = 0; lst_loc != __mem_track->lst_size; lst_loc ++) {
		mdl_u8_t used = 1;
		for (mdl_uint_t uu_point = 0; uu_point != __mem_track->unused_pnts_c; uu_point ++) {
			if (lst_loc == __mem_track->unused_pnts[uu_point]) {used = 0; break;}
		}
		if (used) break;
		if (__mem_track->nxt_pnt == __mem_track->lst_size-1) return NULL;
		__mem_track->nxt_pnt++;
	}

	void *nxt_ptr = __mem_track->mptr_lst[__mem_track->nxt_pnt];
	__mem_track->nxt_pnt++;
	return nxt_ptr;
}

void ffly_mem_track_reset(struct ffly_mem_track *__mem_track) {
	__mem_track->nxt_pnt = 0;
}

void ffly_mem_track_update(struct ffly_mem_track *__mem_track, void *__mptr, void *__n_mptr) {
	for (mdl_uint_t lst_loc = 0; lst_loc != __mem_track->lst_size; lst_loc ++) {
		if (__mem_track->mptr_lst[lst_loc] == __mptr) {
			__mem_track->mptr_lst[lst_loc] = __n_mptr;
			break;
		}
	}
}

ffly_err_t ffly_mem_track_alloc(struct ffly_mem_track *__mem_track, void *__mptr) {
	mdl_uint_t lst_loc = 0;
	if (__mem_track->unused_pnts_c != 0) {
		lst_loc = __mem_track->unused_pnts[__mem_track->unused_pnts_c];

		if (__mem_track->unused_pnts_c-1 == 0) {
			if (__mem_track->unused_pnts != NULL) ffly_mem_free(__mem_track->unused_pnts, 1);
			__mem_track->unused_pnts_c--;
		} else {
			if ((__mem_track->unused_pnts = (mdl_uint_t*)ffly_mem_realloc(__mem_track->unused_pnts, (--__mem_track->unused_pnts_c)*sizeof(mdl_uint_t))) == NULL) {
				ffly_printf(stderr, "mem_tracker: failed to realloc memory for 'unused_pnts', errno: %d\n", errno);
				return FFLY_FAILURE;
			}
		}

		goto _sk_resize;
	}

	if (__mem_track->lst_size == 0) {
		if ((__mem_track->mptr_lst = (void**)ffly_mem_alloc(sizeof(void*), 1)) == NULL) {
			ffly_printf(stderr, "mem_tracker: failed to alloc memory for 'mptr_lst', errno: %d\n", errno);
			return FFLY_FAILURE;
		}
	} else {
		if ((__mem_track->mptr_lst = (void**)ffly_mem_realloc(__mem_track->mptr_lst, (__mem_track->lst_size+1)*sizeof(void*))) == NULL) {
			ffly_printf(stderr, "mem_tracker: failed to realloc memory for 'mptr_lst', errno: %d\n", errno);
			return FFLY_FAILURE;
		}
	}
	lst_loc = __mem_track->lst_size;
	_sk_resize:

	*(__mem_track->mptr_lst+lst_loc) = __mptr;
	__mem_track->lst_size++;
	return FFLY_SUCCESS;
}

ffly_err_t ffly_mem_track_free(struct ffly_mem_track *__mem_track, void *__mptr, mdl_u8_t __hard) {
	mdl_uint_t lst_loc = 0;
	void **itr = __mem_track->mptr_lst;
	while(itr != __mem_track->mptr_lst+__mem_track->lst_size) {
		mdl_uint_t off = itr-__mem_track->mptr_lst;
		if (*itr == __mptr) {
			lst_loc = off;
			break;
		}

		if (off == __mem_track->lst_size-1) {
			ffly_printf(stderr, "mem_tracker: failed to locate memory with ptr: %p.\n", __mptr);
			return FFLY_FAILURE;
		}

		itr++;
	}

	if (__hard) goto _sk_soft;

	if (__mem_track->unused_pnts_c == 0) {
		if ((__mem_track->unused_pnts = (mdl_uint_t*)ffly_mem_alloc(sizeof(mdl_uint_t), 1)) == NULL) {
			ffly_printf(stderr, "mem_tracker: failed to alloc memory for 'unused_pnts', errno: %d\n", errno);
			return FFLY_FAILURE;
		}
	} else {
		if ((__mem_track->unused_pnts = (mdl_uint_t*)ffly_mem_realloc(__mem_track->unused_pnts, (__mem_track->unused_pnts_c+1)*sizeof(mdl_uint_t))) == NULL) {
			ffly_printf(stderr, "mem_tracker: failed to realloc memory for 'unused_pnts', errno: %d\n", errno);
			return FFLY_FAILURE;
		}
	}
	__mem_track->mptr_lst[lst_loc] = NULL;
	__mem_track->unused_pnts[__mem_track->unused_pnts_c] = lst_loc;
	__mem_track->unused_pnts_c++;
	return FFLY_SUCCESS;
	_sk_soft:

	if (__mem_track->lst_size-1 == 0)
		if (__mem_track->mptr_lst != NULL) {
			ffly_mem_free(__mem_track->mptr_lst, 1);
			__mem_track->mptr_lst = NULL;
		}
	else {
		__mem_track->mptr_lst[lst_loc] = __mem_track->mptr_lst[__mem_track->lst_size-1];
		if ((__mem_track->mptr_lst = (void**)ffly_mem_realloc(__mem_track->mptr_lst, (__mem_track->lst_size-1)*sizeof(void*))) == NULL) {
			ffly_printf(stderr, "mem_tracker: failed to realloc memory for 'mptr_lst', errno: %d\n", errno);
			return FFLY_FAILURE;
		}
	}

	__mem_track->lst_size--;
	return FFLY_SUCCESS;
}

struct ffly_mem_track __ffly_mem_track__;
