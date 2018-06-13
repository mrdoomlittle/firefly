# include "dus.h"
# include "../ffly_def.h"
# include "../stdio.h"
# include "../malloc.h"
void static emit(nodep);

objp static top = NULL;
objp static end = NULL;
objp static
next_obj(void) {
	objp o = (objp)malloc(sizeof(struct obj));
	if (!top)
		top = o;
	if (end != NULL)
		end->next = o;
	o->next = NULL;
	end = o;
	return o;
}

void static
op_assign(objp __to, ff_u8_t *__from, ff_uint_t __len) {
	objp o = next_obj();
	o->op = _op_assign;
	o->to = __to;
	o->p = __from;
	o->len = __len;
}

objp static
op_fresh(ff_uint_t __size) {
	objp o = next_obj();
	o->op = _op_fresh;
	o->size = __size;
	return o;
}

void static
op_free(objp __obj) {
	objp o = next_obj();
	o->op = _op_free;
}

void static
op_push(objp __obj) {
	objp o = next_obj();
	o->op = _op_push;
	o->p = __obj;
}

void static
op_pop(objp **__obj) {
	objp o = next_obj();
	o->op = _op_pop;
	*__obj = (objp*)&o->p;
}

void static
op_out(objp *__p) {
	objp o = next_obj();
	o->op = _op_out;
	o->p = __p;
}

void push(objp __obj) {
	op_push(__obj);
}

objp* pop(void) {
	objp *ret;
	op_pop(&ret);
	return ret;
}

void static
emit_decl_init(nodep __node, objp __to) {
	op_assign(__to, __node->p, __node->len);
}

void static
emit_decl(nodep __node) {
	nodep init = __node->init;
	nodep var = __node->var;
	objp m = op_fresh(init->len);
	emit_decl_init(init, m);
	var->_obj = m;
}

void static
emit_var(nodep __node) {
	push(__node->_obj);
}

void static
emit_out(nodep __node) {
	emit((nodep)__node->p);
	objp *o;
	o = pop();
	op_out(o);
}

void
emit(nodep __node) {
	switch(__node->kind) {
		case _decl:
			emit_decl(__node);
		break;
		case _var:
			emit_var(__node);
		break;
		case _assign:
		//	emit_assign(__node);
		break;
		case _out:
			emit_out(__node);
		break;
	}
}

objp ff_dus_gen(nodep __top) {
	nodep cur = __top;
	while(cur != NULL) {
		emit(cur);
		cur = cur->next;
	}
	return top;
}
