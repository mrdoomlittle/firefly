# include "obj.h"
# include "memory/mem_free.h"
# include "memory/mem_alloc.h"
# include "system/errno.h"
# include "system/io.h"
# include "dep/bcopy.h"
# include "dep/bzero.h"

/*
	using malloc for allocation is fine for now.
	it's only going to be slow when creating and destroying objs repeatedly.
*/
// TODO: update
# define FASTSIZE 88
// needs testing
ffly_objp fastpool[FASTSIZE];
ffly_objp static *fast = fastpool;

ffly_objp static top = NULL;
void ffly_obj_rotate(ffly_objp __obj, float __angle) {
	__obj->angle = __angle;
}

ff_err_t ffly_obj_prepare(ffly_objp __obj) {
	__obj->angle = 0.0;
	__obj->script = NULL;
}

ffly_objp ffly_obj_alloc(ff_err_t *__err) {
	*__err = FFLY_SUCCESS;
	/*
		should have its own memory allocation region for this later,
		as malloc can be slow when calling lots of times.
	*/

	ffly_objp obj;
	if (fast > fastpool)
		obj = *(--fast);
	else
		obj = (ffly_objp)__ffly_mem_alloc(sizeof(struct ffly_obj));
	ffly_fprintf(ffly_log, "alloced new object.\n");
	if (top != NULL) {
		obj->bk = &top->next;
	}
	obj->next = top;
	obj->bk = &top;
	top = obj;
	return obj;
}

ff_err_t ffly_obj_free(ffly_objp __obj) {
	ffly_fprintf(ffly_log, "freed object %u.\n", __obj->no);
	*__obj->bk = __obj->next;
_sk:
	if (fast < fastpool+FASTSIZE)
		*(fast++) = __obj;
	else
		__ffly_mem_free(__obj);
	return FFLY_SUCCESS;
}

ff_err_t ffly_obj_cleanup() {
	ffly_fprintf(ffly_log, "cleaning up objects.\n");
	ffly_objp cur = top, bk;
	while(cur != NULL) {
		bk = cur;
		cur = cur->next;

		__ffly_mem_free(bk);
	}

	top = NULL;
	if (fast > fastpool) {
		ffly_objp *p = fastpool;
		while(p != fast) {
			__ffly_mem_free(*p);
			p++;
		}
	}
	fast = fastpool;
}
# include "physics/body.h"
ff_err_t ffly_obj_handle(ffly_unip __uni, ff_uint_t __delta, ffly_objp __obj) {
	ffly_physical_body_update(__uni, __delta, __obj->phy_body);
}
