# define __ffly_script_internal
# include "script.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/buff.h"
struct obj *rg_8l_u, *rg_16l_u, *rg_32l_u, *rg_64l_u;
void build_obj(struct obj **__obj, struct obj *__tmpl) {
	*__obj = (struct obj*)__ffly_mem_alloc(sizeof(struct obj));
	**__obj = *__tmpl;
}

struct obj** objpp(struct obj *__obj) {
    __obj->objpp = __obj;
    return &__obj->objpp;
}

struct obj *end = NULL;
struct obj* next_obj(struct ffly_script *__script, struct obj __tmpl) {
	struct obj *_obj;
	build_obj(&_obj, &__tmpl);
    _obj->next = NULL;
	if (__script->top == NULL)
		__script->top = (void*)_obj;
	else {
		end->next = _obj;
		end = _obj;
	}

	end = _obj;
	return _obj;
}

struct obj obj_tmpl = {
    .off = 0, .opcode = 0, .cond = 0,
    .p = NULL, ._type = NULL, .to = NULL, .from = NULL,
    .l = NULL, .r = NULL, .val = NULL, .jmp = NULL,
    .flags = NULL, .next = NULL
};

struct obj mk_op_copy(struct type *__type, struct obj **__to, struct obj **__from) {
    struct obj _obj = obj_tmpl;
    _obj.opcode = _op_copy;
    _obj._type = __type;
    _obj.to = __to;
    _obj.from = __from;
    return _obj;
}

struct obj mk_op_fresh(mdl_uint_t __size) {
    struct obj _obj = obj_tmpl;
    _obj.opcode = _op_fresh;
    _obj.size = __size;
    return _obj;
}

struct obj mk_op_free(mdl_uint_t __size) {
    struct obj _obj = obj_tmpl;
    _obj.opcode = _op_free;
    _obj.size = __size;
    return _obj;
}

struct obj mk_op_assign(void *__p, struct type *__type, struct obj **__to) {
    struct obj _obj = obj_tmpl;
    _obj.opcode = _op_assign;
    _obj.p = __p;
    _obj._type = __type;
    _obj.to = __to;
    return _obj;
}

struct obj mk_op_compare(struct obj *__flags, struct obj **__l, struct obj **__r) {
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

struct obj mk_op_zero(struct obj **__dst) {
    struct obj _obj = obj_tmpl;
    _obj.opcode = _op_zero;
    _obj.dst = __dst;
    return _obj;
}

struct obj mk_op_jump() {
    struct obj _obj = obj_tmpl;
    _obj.opcode = _op_jump;
    return _obj;
}

struct obj mk_op_push(struct obj *__obj) {
    struct obj _obj = obj_tmpl;
    _obj.opcode = _op_push;
    _obj._obj = __obj;
    return _obj;
}

struct obj mk_op_pop() {
    struct obj _obj = obj_tmpl;
    _obj.opcode = _op_pop;
    return _obj;
}

struct obj* push(struct ffly_script *__script, struct obj *__obj) {
    return next_obj(__script, mk_op_push(__obj));
}

struct obj* pop(struct ffly_script *__script, struct obj ***__obj) {
    struct obj *_obj = next_obj(__script, mk_op_pop());
    *__obj = &_obj->_obj;
    return _obj;
}

void emit(struct ffly_script*, struct node*);
void emit_decl_init(struct ffly_script *__script, struct node *__node, struct obj *__to) {
	emit(__script, __node->init);
	struct obj **from;
	pop(__script, &from);
    if (__node->var->_type->size > __node->init->_type->size)
        next_obj(__script, mk_op_zero(objpp(__to)));
	next_obj(__script, mk_op_copy(__node->init->_type, objpp(__to), from));
}

void emit_decl(struct ffly_script *__script, struct node *__node) { 
    if (__node->var->_type->kind == _struct) {
        ffly_vec_init(&__node->var->fields, sizeof(struct obj*));
        ffly_vec_resize(&__node->var->fields, __node->var->_type->size);
        void *itr = ffly_map_begin(&__node->var->_type->fields);
        while(itr != NULL) {
            ffly_printf("----------\n");
            struct type *_type = (struct type*)ffly_map_getp(itr);
            *(struct obj**)ffly_vec_at(&__node->var->fields, _type->off) = next_obj(__script, mk_op_fresh(_type->size)); 
            ffly_map_itr(&__node->var->_type->fields, &itr, MAP_ITR_FD);
        }
    } else {
        struct obj *m = next_obj(__script, mk_op_fresh(__node->var->_type->size));
        m->_type = __node->var->_type;
        __node->var->_obj = m;
        if (__node->init != NULL)
    	    emit_decl_init(__script, __node, m);
    }
}

void emit_assign(struct ffly_script *__script, struct node *__node) {
    emit(__script, __node->l);
    emit(__script, __node->r);
    struct obj **l, **r;
    pop(__script, &r);
    pop(__script, &l);
    next_obj(__script, mk_op_copy(__node->_type, l, r));
}

void emit_literal(struct ffly_script *__script, struct node *__node) {
	struct obj *m = next_obj(__script, mk_op_fresh(__node->_type->size));
	next_obj(__script, mk_op_assign(__node->val, __node->_type, objpp(m)));
    m->_type = __node->_type;
	push(__script, m);
}

void emit_print_call(struct ffly_script *__script, struct node *__node) {
	struct obj **arg;
	emit(__script, __node->arg);
    pop(__script, &arg);
    next_obj(__script, (struct obj){.opcode=_op_print, .p=NULL, ._type=__node->arg->_type, .to=NULL, .from=NULL, .val=arg});
}

void emit_var(struct ffly_script *__script, struct node *__node) {
    push(__script, __node->_obj);    
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
    if (ffly_vec_size(&__node->block)>0) {
        itr = (struct node**)ffly_vec_begin(&__node->block);
        while(itr <= (struct node**)ffly_vec_end(&__node->block))
            emit(__script, *(itr++));
    }

    _obj->jmp = (struct obj***)__ffly_mem_alloc(sizeof(struct obj***));
    cleanup(__script, (void*)_obj->jmp);
    *_obj->jmp = &end->next;
}

void emit_binop(struct ffly_script *__script, struct node *__node) {
    emit(__script, __node->l);
    emit(__script, __node->r);
    struct obj **l, **r;
    pop(__script, &r);
    pop(__script, &l);
    next_obj(__script, mk_op_compare(rg_8l_u, l, r));
}

void emit_func(struct ffly_script *__script, struct node *__node) {
    ffly_printf("emit func.\n");
    struct obj *jmp = next_obj(__script, mk_op_jump());
    __node->jmp = &end->next;

    struct node **itr = NULL;
    if (ffly_vec_size(&__node->args)>0) {
        itr = (struct node**)ffly_vec_end(&__node->args);
        while(itr >= (struct node**)ffly_vec_begin(&__node->args)) {
            emit(__script, *itr);
            emit(__script, (*itr)->var);
            struct obj **to, **from;
            pop(__script, &to);
            pop(__script, &from);
            next_obj(__script, mk_op_copy((*itr)->var->_type, to, from));
            ffly_printf("----------------------------\n");
            itr--;
        }
    }

    if (ffly_vec_size(&__node->block)>0) {
        itr = (struct node**)ffly_vec_begin(&__node->block);
        while(itr <= (struct node**)ffly_vec_end(&__node->block))
            emit(__script, *(itr++));
    }

    struct obj **ret_to;
    pop(__script, &ret_to);

    struct obj *ret = next_obj(__script, mk_op_jump());
    ret->jmp = (struct obj***)ret_to;
    jmp->jmp = (struct obj***)__ffly_mem_alloc(sizeof(struct obj**));
    cleanup(__script, (void*)jmp->jmp);
    *jmp->jmp = &end->next; 
}

void emit_func_call(struct ffly_script *__script, struct node *__node) {
    ffly_printf("emit func call.\n");
    struct node **itr = NULL;
    struct obj *ret = push(__script, NULL);

    if (ffly_vec_size(&__node->params)>0) {
        itr = (struct node**)ffly_vec_begin(&__node->params);
        while(itr <= (struct node**)ffly_vec_end(&__node->params)) {
            emit(__script, *itr); 
            ffly_printf("-------> size: %u\n", (*itr)->_type->size);
            itr++;
        }
    }

    struct obj *jmp = next_obj(__script, mk_op_jump());
    jmp->jmp = (struct obj***)__ffly_mem_alloc(sizeof(struct obj**));
    cleanup(__script, (void*)jmp->jmp);
    *jmp->jmp = __node->call->jmp;
    ret->_obj = (struct obj*)&end->next;
}

void emit_struct_ref(struct ffly_script *__script, struct node *__node) {
    //emit(__script, __node->_struct);
  //  __node->_struct->_obj->off = __node->_type->off;
    struct obj *m = *(struct obj**)ffly_vec_at(&__node->_struct->fields, __node->_type->off);
    push(__script, m);
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
        case _ast_func:
            emit_func(__script, __node);
        break;
        case _ast_func_call:
            emit_func_call(__script, __node);
        break;
        case _ast_struct_ref:
            emit_struct_ref(__script, __node);
        break;
        case _ast_assign:
            emit_assign(__script, __node);
        break;
	}
}

ffly_err_t ffly_script_gen_free() {
    __ffly_mem_free(rg_8l_u->_type);
    __ffly_mem_free(rg_16l_u->_type);
    __ffly_mem_free(rg_32l_u->_type);
    __ffly_mem_free(rg_64l_u->_type);
}

ffly_err_t ffly_script_gen(struct ffly_script *__script) {
	if (!ffly_vec_size(&__script->nodes)) return FFLY_FAILURE;
    rg_8l_u = next_obj(__script, mk_op_fresh(1));
    *(rg_8l_u->_type = (struct type*)__ffly_mem_alloc(sizeof(struct type))) = (struct type){.kind=_u8_t, .size=1};

    rg_16l_u = next_obj(__script, mk_op_fresh(2));
    *(rg_16l_u->_type = (struct type*)__ffly_mem_alloc(sizeof(struct type))) = (struct type){.kind=_u16_t, .size=2};

    rg_32l_u = next_obj(__script, mk_op_fresh(4));
    *(rg_32l_u->_type = (struct type*)__ffly_mem_alloc(sizeof(struct type))) = (struct type){.kind=_u32_t, .size=4};

    rg_64l_u = next_obj(__script, mk_op_fresh(8));
    *(rg_64l_u->_type = (struct type*)__ffly_mem_alloc(sizeof(struct type))) = (struct type){.kind=_u64_t, .size=8};

    if (ffly_vec_size(&__script->nodes)>0) {
    	struct node **itr = (struct node**)ffly_vec_begin(&__script->nodes);
    	while(itr <= (struct node**)ffly_vec_end(&__script->nodes)) {
    		emit(__script, *itr);
    		itr++;
    	}
    }
    return FFLY_SUCCESS;
}
