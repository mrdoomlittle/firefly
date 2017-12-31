# include "chunk_keeper.h"
# include "memory/mem_alloc.h"
# include "memory/mem_realloc.h"
# include "memory/mem_free.h"
# include "system/errno.h"
# include "system/io.h"
# include "data/mem_cpy.h"
# include "data/find.h"
# include "memory/alloc_pixmap.h"
# include "ffly_config.h"
ffly_err_t ffly_ck_de_init(struct ffly_ck *__ck) {
	for (__ffly_id_t id = 0; id != __ck->cnk_c; id++) {
		struct ffly_chunk *cnk = __ck->cnk+id;
		if (__ck->uu_ids != NULL && __ck->uu_id_c > 0) {
			if (ffly_find((void*)__ck->uu_ids, (void*)&cnk->info.id, sizeof(ffly_id_t), __ck->uu_id_c) != NULL) {
				__ffly_mem_free(cnk->info.id);
				continue;
			}
		}

		__ffly_mem_free(cnk->data._1d_pm);

		if (cnk->info.id != NULL)
			__ffly_mem_free(cnk->info.id);
	}

	if (__ck->cnk != NULL)
		__ffly_mem_free(__ck->cnk);
	if (__ck->uu_ids != NULL)
		__ffly_mem_free(__ck->uu_ids);
}

ffly_err_t ffly_ck_create(struct ffly_ck *__ck, ffly_id_t *__id, ffly_bool_t __overwrite) {
	if (!__ck->cnk_off) {
		if ((__ck->cnk = __ffly_mem_alloc(sizeof(struct ffly_chunk))) == NULL) {
			ffly_printf(stderr, "chunk_keeper: failed to alloc memory.\n");
			return FFLY_FAILURE;
		}
	} else {
		if ((__ck->cnk = __ffly_mem_realloc(__ck->cnk, (__ck->cnk_off+1)*sizeof(struct ffly_chunk))) == NULL) {
			ffly_printf(stderr, "chunk_keeper: failed to realloc memory.\n");
			return FFLY_FAILURE;
		}
	}

	if (!__ck->uu_id_c) {
		(__ck->cnk+__ck->cnk_off)->info.id = *__id = (ffly_id_t)__ffly_mem_alloc(sizeof(__ffly_id_t));
		if (*__id == NULL) {
			ffly_printf(stderr, "chunk_keeper: failed to alloc memory for chunk id.\n");
			return FFLY_FAILURE;
		}

		*(__ck->cnk+__ck->cnk_off)->info.id = **__id = __ck->cnk_off;
	} else {
		*__id = *(__ck->uu_ids+(__ck->uu_id_c-1));

		if (!(__ck->uu_id_c-1)) {
			__ffly_mem_free(__ck->uu_ids);
			__ck->uu_ids = NULL;
			__ck->uu_id_c--;
		} else {
			if ((__ck->uu_ids = __ffly_mem_realloc(__ck->uu_ids, (__ck->uu_id_c--)*sizeof(ffly_id_t))) == NULL) {
				ffly_printf(stderr, "chunk_keeper: failed to realloc memory.\n");
				return FFLY_FAILURE;
			}
		}
	}

	struct ffly_chunk *cnk = __ck->cnk+(**__id);

	mdl_uint_t rxa_len = __ck->xa_len*UNI_PAR_XLEN;
	mdl_uint_t rya_len = __ck->ya_len*UNI_PAR_YLEN;
	mdl_uint_t rza_len = __ck->za_len*UNI_PAR_ZLEN;

	if ((cnk->data._1d_pm = ffly_alloc_pixmap(rxa_len, rya_len, rza_len, 4)) == NULL) {
		ffly_printf(stderr, "chunk_keeper: failed to alloc memory for pixel map.\n");
		return FFLY_FAILURE;
	}

	__ck->cnk_off++;
	__ck->cnk_c++;
	return FFLY_SUCCESS;
}


ffly_err_t ffly_ck_del(struct ffly_ck *__ck, ffly_id_t __id, ffly_bool_t __hard_del) {
	struct ffly_chunk *cnk = __ck->cnk+(*__id);

	__ffly_mem_free(cnk->data._1d_pm);

	if (__hard_del) {
		__ffly_id_t id = *__id;
		__ffly_mem_free(cnk->info.id);

		if (!(__ck->cnk_off-1)) {
			__ffly_mem_free(__ck->cnk);
			__ck->cnk = NULL;
		} else {
			if (id != __ck->cnk_off-1) {
				ffly_mem_cpy((void*)cnk, __ck->cnk+(__ck->cnk_off-1), sizeof(struct ffly_chunk));
				printf("mov %u to %u\n", *cnk->info.id, id);
				*cnk->info.id = id;
			}

			if ((__ck->cnk = __ffly_mem_realloc(__ck->cnk, (__ck->cnk_off-1)*sizeof(struct ffly_chunk))) == NULL) {
				ffly_printf(stderr, "chunk_keeper: failed to realloc memory.\n");
				return FFLY_FAILURE;
			}
		}

		__ck->cnk_off--;
		__ck->cnk_c--;
		return FFLY_SUCCESS;
	}

	if (!__ck->uu_id_c) {
		if ((__ck->uu_ids = (ffly_id_t*)__ffly_mem_alloc(sizeof(ffly_id_t))) == NULL) {
			ffly_printf(stderr, "chunk_keeper: failed to alloc memory.\n");
			return FFLY_FAILURE;
		}
	} else {
		if ((__ck->uu_ids = (ffly_id_t*)__ffly_mem_realloc(__ck->uu_ids, (__ck->uu_id_c+1)*sizeof(ffly_id_t))) == NULL) {
			ffly_printf(stderr, "chunk_keeper: failed to realloc memory.\n");
			return FFLY_FAILURE;
		}
	}

	*(__ck->uu_ids+(__ck->uu_id_c++)) = __id;
	__ck->cnk_c--;
	return FFLY_SUCCESS;
}

