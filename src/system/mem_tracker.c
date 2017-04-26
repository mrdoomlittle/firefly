# include "mem_tracker.h"
ffly_err_t ffly_mem_track_init(struct ffly_mem_track_t *__mem_track) {
	__mem_track-> mptr_lst = NULL;
	__mem_track-> lst_size = 0;
	__mem_track-> unused_pnts = NULL;
	__mem_track-> unused_pnts_c = 0;
	__mem_track-> nxt_pnt = 0;
	return FFLY_SUCCESS;
}

ffly_err_t ffly_mem_track_de_init(struct ffly_mem_track_t *__mem_track) {
	if (__mem_track-> mptr_lst != NULL) {
		ffly_mem_free(__mem_track-> mptr_lst, 1);
		__mem_track-> mptr_lst = NULL;
	}

	if (__mem_track-> unused_pnts != NULL) {
		ffly_mem_free(__mem_track-> unused_pnts, 1);
		__mem_track-> unused_pnts = NULL;
	}
	return FFLY_SUCCESS;
}

# ifdef __DEBUG_ENABLED
void ffly_mem_track_dmp_ptr_lst(struct ffly_mem_track_t *__mem_track) {
	for (mdl_uint_t lst_point = 0; lst_point != __mem_track-> lst_size; lst_point ++) {
		mdl_u8_t used = 1;
		for (mdl_uint_t uu_point = 0; uu_point != __mem_track-> unused_pnts_c; uu_point ++) {
			if (lst_point == __mem_track-> unused_pnts[uu_point]) {used = 0; break;}
		}

		printf("mem_tracker: mem_ptr: %p, point: %d, state: %s\n", __mem_track-> mptr_lst[lst_point], lst_point, used == 1? "used" : "unused");
	}
}
# endif

void *ffly_mem_track_get_nxt(struct ffly_mem_track_t *__mem_track) {
	if (__mem_track-> nxt_pnt == __mem_track-> lst_size - 1) return NULL;
	for (mdl_uint_t lst_point = 0; lst_point != __mem_track-> lst_size; lst_point ++) {
		mdl_u8_t used = 1;
		for (mdl_uint_t uu_point = 0; uu_point != __mem_track-> unused_pnts_c; uu_point ++) {
			if (lst_point == __mem_track-> unused_pnts[uu_point]) {used = 0; break;}
		}
		if (used) break;
		if (__mem_track-> nxt_pnt == __mem_track-> lst_size - 1) return NULL;
		__mem_track-> nxt_pnt++;
	}

	void *nxt_ptr = __mem_track-> mptr_lst[__mem_track-> nxt_pnt];
	__mem_track-> nxt_pnt++;
	return nxt_ptr;
}

void ffly_mem_track_reset(struct ffly_mem_track_t *__mem_track) {
	__mem_track-> nxt_pnt = 0;
}

void ffly_mem_track_update(struct ffly_mem_track_t *__mem_track, void *__mptr, void *__n_mptr) {
	for (mdl_uint_t lst_point = 0; lst_point != __mem_track-> lst_size; lst_point ++) {
		if (__mem_track-> mptr_lst[lst_point] == __mptr) {
			__mem_track-> mptr_lst[lst_point] = __n_mptr;
			break;
		}
	}
}

ffly_err_t ffly_mem_track_alloc(struct ffly_mem_track_t *__mem_track, void *__mptr) {
	mdl_uint_t lst_point = 0;

	if (__mem_track-> unused_pnts_c != 0) {
		lst_point = __mem_track-> unused_pnts[__mem_track-> unused_pnts_c];

		if (__mem_track-> unused_pnts_c - 1 == 0)
			if (__mem_track-> unused_pnts != NULL) ffly_mem_free(__mem_track-> unused_pnts, 1);
		else {
			if ((__mem_track-> unused_pnts = ffly_mem_realloc(__mem_track-> unused_pnts, (__mem_track-> unused_pnts_c - 1) * sizeof(mdl_uint_t))) == NULL) {
				fprintf(stderr, "mem_tracker: failed to realloc memory for 'unused_pnts', errno: %d\n", errno);
				return FFLY_FAILURE;
			}
		}
		__mem_track-> unused_pnts_c--;
		goto sk_resize;
	}

	if (__mem_track-> lst_size == 0) {
		if ((__mem_track-> mptr_lst = (void **)ffly_mem_alloc(sizeof(void *), 1)) == NULL) {
			fprintf(stderr, "mem_tracker: failed to alloc memory for 'mptr_lst', errno: %d\n", errno);
			return FFLY_FAILURE;
		}
	} else {
		if ((__mem_track-> mptr_lst = (void **)ffly_mem_realloc(__mem_track-> mptr_lst, (__mem_track-> lst_size + 1) * sizeof(void *))) == NULL) {
			fprintf(stderr, "mem_tracker: failed to realloc memory for 'mptr_lst', errno: %d\n", errno);
			return FFLY_FAILURE;
		}
	}
	lst_point = __mem_track-> lst_size;
	sk_resize:
	__mem_track-> mptr_lst[lst_point] = __mptr;
	__mem_track-> lst_size++;
	return FFLY_SUCCESS;
}

ffly_err_t ffly_mem_track_free(struct ffly_mem_track_t *__mem_track, void *__mptr, mdl_u8_t __hard) {
	mdl_uint_t lst_point = 0;
	for (mdl_uint_t point = 0; point != __mem_track-> lst_size; point ++) {
		if (__mem_track-> mptr_lst[point] == __mptr) {
			lst_point = point;
			break;
		}
		if (point = __mem_track-> lst_size - 1) return FFLY_FAILURE;
	}


	if (__hard) goto sk_soft;

	if (__mem_track-> unused_pnts_c == 0) {
		if ((__mem_track-> unused_pnts = (mdl_uint_t *)ffly_mem_alloc(sizeof(mdl_uint_t), 1)) == NULL) {
			fprintf(stderr, "mem_tracker: failed to alloc memory for 'unused_pnts', errno: %d\n", errno);
			return FFLY_FAILURE;
		}
	} else {
		if ((__mem_track-> unused_pnts = (mdl_uint_t *)ffly_mem_realloc(__mem_track-> unused_pnts, (__mem_track-> unused_pnts_c + 1) * sizeof(mdl_uint_t))) == NULL) {
			fprintf(stderr, "mem_tracker: failed to realloc memory for 'unused_pnts', errno: %d\n", errno);
			return FFLY_FAILURE;
		}
	}
	__mem_track-> mptr_lst[lst_point] = NULL;
	__mem_track-> unused_pnts[__mem_track-> unused_pnts_c] = lst_point;
	__mem_track-> unused_pnts_c++;
	return FFLY_SUCCESS;
	sk_soft:

	if (__mem_track-> lst_size - 1 == 0)
		if (__mem_track-> mptr_lst != NULL) ffly_mem_free(__mem_track-> mptr_lst, 1);
	else {
		__mem_track-> mptr_lst[lst_point] = __mem_track-> mptr_lst[__mem_track-> lst_size - 1];
		if ((__mem_track-> mptr_lst = (void **)ffly_mem_realloc(__mem_track-> mptr_lst, (__mem_track-> lst_size - 1) * sizeof(void *))) == NULL) {
			fprintf(stderr, "mem_tracker: failed to realloc memory for 'mptr_lst', errno: %d\n", errno);
			return FFLY_FAILURE;
		}
	}

	__mem_track-> lst_size--;
	return FFLY_SUCCESS;
}

struct ffly_mem_track_t __ffly_mem_track__;
