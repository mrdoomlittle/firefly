# define __ffly_script_internal
# include "script.h"
# include "memory/mem_alloc.h"
# include "system/buff.h"
struct ffly_buff obj_pass;
void obj_push(struct obj *__obj) {
	ffly_buff_put(&obj_pass, &__obj);
	ffly_buff_incr(&obj_pass);
}

void obj_pop(struct obj **__obj) {
	ffly_buff_decr(&obj_pass);
	ffly_buff_get(&obj_pass, __obj);
}

void build_obj(struct obj **__obj, struct obj *__tmpl) {
	*__obj = (struct obj*)__ffly_mem_alloc(sizeof(struct obj));
	**__obj = *__tmpl;
}

struct obj *end;
struct obj* next_obj(struct ffly_script *__script, struct obj __tmpl) {
	struct obj *_obj;
	build_obj(&_obj, &__tmpl);
	if (__script->top == NULL)
		__script->top = (void*)_obj;
	else {
		end->next = _obj;
		end = _obj;
	}

	end = _obj;
	return _obj;
}

struct obj mk_op_copy(struct type *__type, struct obj *__to, struct obj *__from) {
    return (struct obj){.opcode=_op_copy, .p = NULL, ._type=__type, .to=__to, .from=__from, .val=NULL};
}

struct obj mk_op_alloc(struct type *__type) {
    return (struct obj){.opcode=_op_alloc, .p = NULL, ._type=__type, .to=NULL, .from=NULL, .val=NULL};
}

struct obj mk_op_assign(void *__p, struct type *__type, struct obj *__to) {
    return (struct obj){.opcode=_op_assign, .p=__p, ._type=__type, .to=__to, .from=NULL, .val=NULL};
}

void emit(struct ffly_script*, struct node*);
void emit_decl_init(struct ffly_script *__script, struct node *__node, struct obj *__to) {
	emit(__script, __node->init);
	struct obj *from;
	obj_pop(&from);
	next_obj(__script, mk_op_copy(__node->init->_type, __to, from));
}

void emit_decl(struct ffly_script *__script, struct node *__node) {
    struct obj *m = next_obj(__script, mk_op_alloc(__node->var->_type));
	if (__node->init != NULL)
		emit_decl_init(__script, __node, m);
    __node->var->_obj = m;

}

void emit_literal(struct ffly_script *__script, struct node *__node) {
	struct obj *m = next_obj(__script, mk_op_alloc(__node->_type));
	next_obj(__script, mk_op_assign(__node->val, __node->_type, m));
	obj_push(m);
}

void emit_print_call(struct ffly_script *__script, struct node *__node) {
	struct obj *arg;
	emit(__script, __node->arg);
	obj_pop(&arg);
	next_obj(__script, (struct obj){.opcode=_op_print, .p=NULL, ._type=__node->arg->_type, .to=NULL, .from=NULL, .val=arg});
}

void emit_var(struct ffly_script *__script, struct node *__node) {
    obj_push(__node->_obj);    
}

void emit(struct ffly_script *__script, struct node *__node) {
	switch(__node->kind) {
		case _ast_decl:
			emit_decl(__script, __node);
		break;
		case _ast_literal:
			emit_literal(__script, __node);
		break;
		case _ast_print_call:
			emit_print_call(__script, __node);
		break;
        case _ast_var:
            emit_var(__script, __node);
        break;
	}
}

ffly_err_t ffly_script_gen(struct ffly_script *__script) {
	if (!ffly_vec_size(&__script->nodes)) return FFLY_FAILURE;
	ffly_buff_init(&obj_pass, 20, sizeof(struct obj*));
	struct node **itr = (struct node**)ffly_vec_begin(&__script->nodes);
	while(itr <= (struct node**)ffly_vec_end(&__script->nodes)) {
		emit(__script, *itr);
		itr++;
	}
	ffly_buff_de_init(&obj_pass);
}
