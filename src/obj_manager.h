# ifndef __ffly__obj__manager__h
# define __ffly__obj__manager__h
# include "obj.h"
# include "types/err_t.h"
# include "types/id_t.h"
struct ffly_obj_man {
    ffly_objpp top, end;
    mdl_uint_t off, page_c;
};

typedef struct ffly_obj_man* ffly_obj_manp;
ffly_err_t ffly_obj_man_prepare(ffly_obj_manp);
ffly_id_t ffly_obj_man_add(ffly_obj_manp, ffly_objp);
ffly_err_t ffly_obj_man_rm(ffly_obj_manp, ffly_id_t);
ffly_objp ffly_obj_man_get(ffly_obj_manp , ffly_id_t);
ffly_err_t ffly_obj_man_free(ffly_obj_manp);
# ifdef __cplusplus
namespace mdl {
namespace firefly {
struct obj_manager {



};
}
}
# endif
# endif /*__ffly__obj__manager__h*/
