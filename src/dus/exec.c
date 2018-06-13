# include "dus.h"
# include "../ffly_def.h"
# include "../stdio.h"
# include "../string.h"
char const *opstr(ff_u8_t __op) {
	switch(__op) {
		case _op_copy:		return "copy";
		case _op_assign:	return "assign";
		case _op_push:		return "push";
		case _op_pop:		return "pop";
		case _op_fresh:		return "fresh";
		case _op_free:		return "free";
		case _op_out:		return "out";
	}
	return "unknown";
}

objp static objs[20];
objp static* top = objs;

ff_u8_t static stack[689];
ff_u8_t static *fresh = stack;

void static
op_copy(objp __obj) {

}

void static
op_fresh(objp __obj) {
	__obj->p = fresh;
	fresh+=__obj->size;
}

void static
op_free(objp __obj) {

}

void static
op_push(objp __obj) {
	*(top++) = __obj->p;
}

void static
op_pop(objp __obj) {
	__obj->p = *(--top);
}

void static
op_assign(objp __obj) {
	ff_u8_t *dst;
	dst = __obj->to->p;
	memcpy(dst, __obj->p, __obj->len);
}

void static
op_out(objp __obj) {
	objp src = *(objp*)__obj->p;
	char *p = (char*)src->p;
	printf("%s\n", p);
}

static void(*op[])(objp) = {	
	op_copy,
	op_assign,
	op_push,
	op_pop,
	op_fresh,
	op_free,
	op_out
};

void ff_dus_run(objp __top) {
	objp cur = __top;
	while(cur != NULL) {
		op[cur->op](cur);
		printf("op: %s\n", opstr(cur->op));
		cur = cur->next;
	}
}
