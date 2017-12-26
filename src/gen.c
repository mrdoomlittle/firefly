# define __ffly_script_internal
# include "script.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/buff.h"
struct obj *rg_8l_u, *rg_16l_u, *rg_32l_u, *rg_64l_u;

struct ffly_vec to_free;
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

void push_free(struct obj *__obj) {
    struct obj **_obj;
    ffly_vec_push_back(&to_free, (void**)&_obj);
    *_obj = __obj;
}

struct obj obj_tmpl = {
    .off = 0, .opcode = 0, .cond = 0,
    .p = NULL, ._type = NULL, .to = NULL, .from = NULL,
    .l = NULL, .r = NULL, .val = NULL, .jmp = NULL,
    .flags = NULL, .next = NULL
};

struct obj mk_op_copy(struct type *__type, struct obj *__to, struct obj *__from) {
    struct obj _obj = obj_tmpl;
    _obj.opcode = _op_copy;
    _obj._type = __type;
    _obj.to = __to;
    _obj.from = __from;
    return _obj;
}

struct obj mk_op_alloc(struct type *__type) {
    struct obj _obj = obj_tmpl;
    _obj.opcode = _op_alloc;
    _obj._type = __type;
    return _obj;
}

struct obj mk_op_assign(void *__p, struct type *__type, struct obj *__to) {
    struct obj _obj = obj_tmpl;
    _obj.opcode = _op_assign;
    _obj.p = __p;
    _obj._type = __type;
    _obj.to = __to;
    return _obj;
}

struct obj mk_op_compare(struct obj *__flags, struct obj *__l, struct obj *__r) {
    struct obj _obj = obj_tmpl;
    _obj.opcode = _op_compare;
    _obj.l = __l;
    _obj.r = __r;
    _obj.flags = __flags;
    return _obj;
}

struct obj mk_op_cond_jump(struct obj *__flags, mdl_u8_t __cond) {
    struct obj _obj = obj_tmpl;
    _obj.opcode = _op_cond_jump;
    _obj.flags = __flags;
    _obj.cond = __cond;
    return _obj;
}

struct obj mk_op_zero(struct obj *__dst) {
    struct obj _obj = obj_tmpl;
    _obj.opcode = _op_zero;
    _obj.dst = __dst;
    return _obj;
}

struct obj mk_op_free(struct obj *__obj) {
    struct obj _obj = obj_tmpl;
    _obj.opcode = _op_alloc;
    _obj.p = __obj->p;
    return _obj;
}

void emit(struct ffly_script*, struct node*);
void emit_decl_init(struct ffly_script *__script, struct node *__node, struct obj *__to) {
	emit(__script, __node->init);
	struct obj *from;
	obj_pop(&from);
    if (__to->_type->size > from->_type->size)
        next_obj(__script, mk_op_zero(__to));
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

void emit_if(struct ffly_script *__script, struct node *__node) {
    emit(__script, __node->cond);
    mdl_u8_t flag = 0x0;
    if (__node->cond->kind == _op_eq)
        flag = _flg_neq;
    else if (__node->cond->kind == _op_neq)
        flag = _flg_eq;
    else if (__node->cond->kind == _op_gt)
        flag = _flg_lt|_flg_eq;
    else if (__node->cond->kind == _op_lt)
        flag = _flg_gt|_flg_eq;

    struct obj *_obj = next_obj(__script, mk_op_cond_jump(rg_8l_u, flag));

    struct node **itr = NULL;
    if (ffly_vec_size(&__node->vec)>0) {
        itr = (struct node**)ffly_vec_begin(&__node->vec);
        while(itr <= (struct node**)ffly_vec_end(&__node->vec))
            emit(__script, *(itr++));
    }

    _obj->jmp = &end->next;
}

void emit_binop(struct ffly_script *__script, struct node *__node) {
    emit(__script, __node->l);
    emit(__script, __node->r);
    struct obj *l, *r;
    obj_pop(&r);
    obj_pop(&l);
    next_obj(__script, mk_op_compare(rg_8l_u, l, r));
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
        case _ast_if:
            emit_if(__script, __node);
        break;
        case _op_eq: case _op_neq: case _op_gt: case _op_lt:
            emit_binop(__script, __node);
        break;
	}
}

ffly_err_t ffly_script_gen_free() {
    __ffly_mem_free(rg_8l_u);
    __ffly_mem_free(rg_16l_u);
    __ffly_mem_free(rg_32l_u);
    __ffly_mem_free(rg_64l_u);
}

ffly_err_t ffly_script_gen(struct ffly_script *__script) {
	if (!ffly_vec_size(&__script->nodes)) return FFLY_FAILURE;
    ffly_vec_set_flags(&to_free, VEC_AUTO_RESIZE);
    ffly_vec_init(&to_free, sizeof(struct obj*));
    rg_8l_u = next_obj(__script, mk_op_alloc(NULL));
    *(rg_8l_u->_type = (struct type*)__ffly_mem_alloc(sizeof(struct type))) = (struct type){.kind=_u8_t, .size=1};

    rg_16l_u = next_obj(__script, mk_op_alloc(NULL));
    *(rg_16l_u->_type = (struct type*)__ffly_mem_alloc(sizeof(struct type))) = (struct type){.kind=_u16_t, .size=2};

    rg_32l_u = next_obj(__script, mk_op_alloc(NULL));
    *(rg_32l_u->_type = (struct type*)__ffly_mem_alloc(sizeof(struct type))) = (struct type){.kind=_u32_t, .size=4};

    rg_64l_u = next_obj(__script, mk_op_alloc(NULL));
    *(rg_64l_u->_type = (struct type*)__ffly_mem_alloc(sizeof(struct type))) = (struct type){.kind=_u64_t, .size=8};

	ffly_buff_init(&obj_pass, 20, sizeof(struct obj*));
    if (ffly_vec_size(&__script->nodes)>0) {
    	struct node **itr = (struct node**)ffly_vec_begin(&__script->nodes);
    	while(itr <= (struct node**)ffly_vec_end(&__script->nodes)) {
    		emit(__script, *itr);
    		itr++;
    	}
    }
	ffly_buff_de_init(&obj_pass);

    if (ffly_vec_size(&to_free)>0) {
        struct obj **itr = (struct obj**)ffly_vec_begin(&to_free);
        while(itr <= (struct obj**)ffly_vec_end(&to_free)) {
            next_obj(__script, mk_op_free(*itr));   
        }
    }

    ffly_vec_de_init(&to_free);
    return FFLY_SUCCESS;
}
