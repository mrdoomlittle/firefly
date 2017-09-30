# ifndef __ffly__pair__h
# define __ffly__pair__h
struct ffly_pair {
	void *p1, *p2;
};
# include "../memory/mem_alloc.h"
struct ffly_pair static __inline__* ffly_mk_pair(void *__p1, void *__p2) {
	struct ffly_pair *ret = (struct ffly_pair*)__ffly_mem_alloc(sizeof(struct ffly_pair));
	ret->p1 = __p1;
	ret->p2 = __p2;
	return ret;
}
# endif /*__ffly__pair__h*/
