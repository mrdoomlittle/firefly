# ifndef __ffly_chunk__keeper__h
# define __ffly_chunk__keeper__h
# include "types/id_t.h"
# include "chunk_info.h"
# include "chunk_data.h"
# include "types/err_t.h"
# include "types/id_t.h"
# include "types/bool_t.h"
# include "ffly_def.h"
struct ffly_chunk {
	struct ffly_chunk_info info;
	struct ffly_chunk_data data;
};

struct ffly_ck {
	ffly_id_t *uu_ids;
	mdl_uint_t uu_id_c;
	mdl_uint_t cnk_off;
	struct ffly_chunk *cnk;
	mdl_uint_t xa_len, ya_len, za_len;
	mdl_uint_t cnk_c;
};

ffly_err_t static __inline__ ffly_ck_init(struct ffly_ck *__ck, mdl_uint_t __xa_len, mdl_uint_t __ya_len, mdl_uint_t __za_len) {
	__ck->xa_len = __xa_len;
	__ck->ya_len = __ya_len;
	__ck->za_len = __za_len;
	__ck->uu_ids = NULL;
	__ck->cnk = NULL;
	__ck->cnk_c = 0;
	__ck->uu_id_c = 0;
	__ck->cnk_off = 0;
}

# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_ck_de_init(struct ffly_ck*);
ffly_err_t ffly_ck_create(struct ffly_ck*, ffly_id_t*, ffly_bool_t);
ffly_err_t ffly_ck_del(struct ffly_ck*, ffly_id_t, ffly_bool_t);
# ifdef __cplusplus
}
# endif

mdl_uint_t static __inline__ ffly_ck_get_xa_len(struct ffly_ck *__ck) {return __ck->xa_len;}
mdl_uint_t static __inline__ ffly_ck_get_ya_len(struct ffly_ck *__ck) {return __ck->ya_len;}
mdl_uint_t static __inline__ ffly_ck_get_za_len(struct ffly_ck *__ck) {return __ck->za_len;}

struct ffly_chunk_info __inline__* ffly_ck_get_info(struct ffly_ck *__ck, ffly_id_t __id) {
	return &(__ck->cnk+*__id)->info;
}
struct ffly_chunk_data __inline__* ffly_ck_get_data(struct ffly_ck *__ck, ffly_id_t __id) {
	return &(__ck->cnk+*__id)->data;
}

# ifdef __cplusplus
namespace mdl {
namespace firefly {
struct chunk_keeper {
	chunk_keeper(uint_t __xa_len, uint_t __ya_len, uint_t __za_len) {
		ffly_ck_init(&this->raw_ck, __xa_len, __ya_len, __za_len);}

	types::err_t __inline__ create(types::id_t& __id, types::bool_t __overwrite) {
		return ffly_ck_create(&this->raw_ck, &__id, __overwrite);}

	types::err_t __inline__ del(types::id_t __id, types::bool_t __hard){
		return ffly_ck_del(&this->raw_ck, __id, __hard);}

	struct ffly_chunk_info __inline__& get_info(types::id_t __id){return *ffly_ck_get_info(&this->raw_ck, __id);}
	struct ffly_chunk_data __inline__& get_data(types::id_t __id){return *ffly_ck_get_data(&this->raw_ck, __id);}

	types::err_t de_init() {return ffly_ck_de_init(&this->raw_ck);}
	uint_t __inline__ get_xa_len(){return ffly_ck_get_xa_len(&this->raw_ck);}
	uint_t __inline__ get_ya_len(){return ffly_ck_get_ya_len(&this->raw_ck);}
	uint_t __inline__ get_za_len(){return ffly_ck_get_za_len(&this->raw_ck);}
	struct ffly_ck raw_ck;
};
}
}
# endif
# endif /*__ffly_chunk__keeper__hpp*/
