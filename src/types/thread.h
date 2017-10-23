# ifndef __ffly__thread__h_
# define __ffly__thread__h_
# include "id_t.h"
typedef __ffly_id_t ffly_tid_t;
typedef mdl_u8_t ffly_thread_cond_t;

# ifdef __cplusplus
namespace mdl {
namespace firefly {
namespace types {
typedef ffly_tid_t tid_t;
}
}
}
# endif
# endif /*__ffly__thread__h_*/
