# define __ffly_script_internal
# include "script.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/buff.h"
# include "data/mem_cpy.h"
struct obj *rg_8l_u, *rg_16l_u, *rg_32l_u, *rg_64l_u;
void build_obj(struct obj **__obj, struct obj *__tmpl) {
	*__obj = (struct obj*)__ffly_mem_alloc(sizeof(struct obj));
	**__obj = *__tmpl;
}

struct obj** objpp(struct obj *__obj) {
    __obj->objpp = __obj;
    return &__obj->objpp;
}


ffly_byte_t static **stack;
static struct obj *top = NULL;
static struct obj *end = NULL;
struct obj* next_obj(struct ffly_script *__script, struct obj __tmpl) {
	struct obj *_obj;
	build_obj(&_obj, &__tmpl);
    _obj->next = NULL;
	if (top == NULL)
		top = (void*)_obj;
	else {
		end->next = _obj;
		end = _obj;
	}

	end = _obj;
	return _obj;
}

mdl_u8_t convtk(mdl_u8_t __kind) {
    switch(__kind) {
        case _u64_t: return _64l_u;
        case _u32_t: return _32l_u;
        case _u16_t: return _16l_u;
        case _u8_t: return _8l_u;
        case _i64_t: return _64l_s;
        case _i32_t: return _32l_s;
        case _i16_t: return _16l_s;
        case _i8_t: return _8l_s;
        case _float: return _float64;
    }
    return 0;
}

struct obj obj_tmpl = {
    .off = 0, .opcode = 0, .cond = 0,
    .p = NULL, .to = NULL, .from = NULL,
    .l = NULL, .r = NULL, .val = NULL, .jmp = NULL,
    .flags = NULL, .next = NULL
};

struct obj mk_op_copy(mdl_uint_t __size, struct obj **__to, struct obj **__from) {
    struct obj _obj = obj_tmpl;
    _obj.opcode = _op_copy;
    _obj.size = __size;
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

struct obj mk_exit() {
    struct obj _obj = obj_tmpl;
    _obj.opcode = _op_exit;
    return _obj;
}

struct obj mk_op_assign(void *__p, mdl_uint_t __size, struct obj **__to) {
    struct obj _obj = obj_tmpl;
    _obj.opcode = _op_assign;
    _obj.p = __p;
    _obj.size = __size;
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

struct obj mk_incr(struct obj **__obj) {
    struct obj _obj = obj_tmpl;
    _obj.opcode = _op_incr;
    _obj.val = __obj;
    return _obj;
}

struct obj mk_decr(struct obj **__obj) {
    struct obj _obj = obj_tmpl;
    _obj.opcode = _op_decr;
    _obj.val = __obj;
    return _obj;
}

struct obj mk_op_frame() {
    struct obj _obj = obj_tmpl;
    _obj.opcode = _op_frame;
    return _obj;
}

struct obj mk_op_call(struct obj **__no) {
    struct obj _obj = obj_tmpl;
    _obj.opcode = _op_call;
    _obj.no = __no;
    return _obj;
}

struct obj mk_op_free_frame(struct obj *__frame) {
    struct obj _obj = obj_tmpl;
    _obj.opcode = _op_free_frame;
    _obj.frame = __frame;
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

struct obj*** call(struct ffly_script *__script, struct obj **__no) {
    return next_obj(__script, mk_op_call(__no))->params;
}

struct obj *create_frame(struct ffly_script *__script) {
    return next_obj(__script, mk_op_frame());
}

void free_frame(struct ffly_script *__script, struct obj *__frame) {
    next_obj(__script, mk_op_free_frame(__frame));
}

struct obj *__fresh(struct ffly_script *__script, mdl_uint_t __size) {
    return next_obj(__script, mk_op_fresh(__size));
}

void __free(struct ffly_script *__script, mdl_uint_t __n) {
    next_obj(__script, mk_op_free(__n));
}

void *stack_push(void *__p, mdl_uint_t __n) {
    void *p = *stack;
    (*stack)+=__n;
    ffly_mem_cpy(p, __p, __n);
    return p;
}

void emit(struct ffly_script*, struct node*);
void emit_decl_init(struct ffly_script *__script, struct node *__node, struct obj *__to) {
	emit(__script, __node->init);
	struct obj **from;
	pop(__script, &from);
    if (__node->var->_type->size > __node->init->_type->size)
        next_obj(__script, mk_op_zero(objpp(__to)));
	next_obj(__script, mk_op_copy(__node->init->_type->size, objpp(__to), from));
}

void emit_decl(struct ffly_script *__script, struct node *__node) { 
    if (__node->var->_type->kind == _struct) {
        ffly_vec_init(&__node->var->fields, sizeof(struct obj*));
        ffly_vec_resize(&__node->var->fields, __node->var->_type->size);
        void *itr = ffly_map_begin(&__node->var->_type->fields);
        while(itr != NULL) {
            struct type *_type = (struct type*)ffly_map_getp(itr);
            *(struct obj**)ffly_vec_at(&__node->var->fields, _type->off) = __fresh(__script, _type->size); 
            ffly_map_itr(&__node->var->_type->fields, &itr, MAP_ITR_FD);
        }
    } else {
        struct obj *m = __fresh(__script, __node->var->_type->size);
        m->_type = convtk(__node->var->_type->kind);
        m->size = __node->var->_type->size;
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
    next_obj(__script, mk_op_copy(__node->_type->size, l, r));
}

void emit_literal(struct ffly_script *__script, struct node *__node) {
	struct obj *m = __fresh(__script, __node->_type->size);
    void *p = stack_push(__node->val, __node->_type->size);
	next_obj(__script, mk_op_assign(p, __node->_type->size, objpp(m)));
    m->_type = convtk(__node->_type->kind);
	push(__script, m);
}

void emit_print_call(struct ffly_script *__script, struct node *__node) {
	struct obj **arg;
	emit(__script, __node->arg);
    pop(__script, &arg);
    next_obj(__script, (struct obj){.opcode=_op_print, .p=NULL, ._type=convtk(__node->arg->_type->kind), .to=NULL, .from=NULL, .val=arg});
}

void emit_var(struct ffly_script *__script, struct node *__node) {
    push(__script, __node->_obj);    
}

void emit_if(struct ffly_script *__script, struct node *__node) {
    struct obj *frame = create_frame(__script);
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

    struct obj *jmp = next_obj(__script, mk_op_cond_jump(rg_8l_u, flag));
    struct node **itr = NULL;
    if (ffly_vec_size(&__node->_do)>0) {
        itr = (struct node**)ffly_vec_begin(&__node->_do);
        while(itr <= (struct node**)ffly_vec_end(&__node->_do))
            emit(__script, *(itr++));
    }

    struct obj **p;
    struct obj *sk = next_obj(__script, mk_op_jump());

    p = &end->next;
    jmp->jmp = (struct obj***)stack_push(&p, sizeof(struct obj**));

    if (ffly_vec_size(&__node->_else)>0) {
        itr = (struct node**)ffly_vec_begin(&__node->_else);
        while(itr <= (struct node**)ffly_vec_end(&__node->_else))
            emit(__script, *(itr++));
    }

    p = &end->next;
    sk->jmp = (struct obj***)stack_push(&p, sizeof(struct obj**));
    free_frame(__script, frame);
}

void emit_while(struct ffly_script *__script, struct node *__node) {
    struct obj *frame = create_frame(__script);
    struct obj **top = &end->next;
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

    struct obj **p;
    struct obj *jmp = next_obj(__script, mk_op_jump());
    p = top;
    jmp->jmp = (struct obj***)stack_push(&p, sizeof(struct obj**));
    p = &end->next;
    _obj->jmp = (struct obj***)stack_push(&p, sizeof(struct obj**));
    free_frame(__script, frame);
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
    __node->pair.p0 = &end->next;
    struct obj *frame = create_frame(__script);
    struct node **itr = NULL;
    if (ffly_vec_size(&__node->args)>0) {
        itr = (struct node**)ffly_vec_end(&__node->args);
        while(itr >= (struct node**)ffly_vec_begin(&__node->args)) {
            emit(__script, *itr);
            emit(__script, (*itr)->var);
            struct obj **to, **from;
            pop(__script, &to);
            pop(__script, &from);
            next_obj(__script, mk_op_copy((*itr)->var->_type->size, to, from));
            ffly_printf("----------------------------\n");
            itr--;
        }
    }

    struct obj **ret_to;
    pop(__script, &ret_to);

    if (ffly_vec_size(&__node->block)>0) {
        itr = (struct node**)ffly_vec_begin(&__node->block);
        while(itr <= (struct node**)ffly_vec_end(&__node->block))
            emit(__script, *(itr++));
    }

    free_frame(__script, frame);
    struct obj *ret = next_obj(__script, mk_op_jump());
    ret->jmp = (struct obj***)ret_to;

    struct obj **p = &end->next;
    jmp->jmp = (struct obj***)stack_push(&p, sizeof(struct obj**));
    __node->pair.p1 = *jmp->jmp; 
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

    struct obj **p = __node->call->jmp;
    jmp->jmp = (struct obj***)stack_push(&p, sizeof(struct obj**));
    ret->_obj = (struct obj*)&end->next;
}

void emit_struct_ref(struct ffly_script *__script, struct node *__node) {
    //emit(__script, __node->_struct);
  //  __node->_struct->_obj->off = __node->_type->off;
    struct obj *m = *(struct obj**)ffly_vec_at(&__node->_struct->fields, __node->_type->off);
    push(__script, m);
}

void emit_exit(struct ffly_script *__script, struct node *__node) {
    next_obj(__script, mk_exit());
}

void emit_incr(struct ffly_script *__script, struct node *__node) {
    emit(__script, __node->operand);
    struct obj **_obj;
    pop(__script, &_obj);
    next_obj(__script, mk_incr(_obj));
}

void emit_decr(struct ffly_script *__script, struct node *__node) {
    emit(__script, __node->operand);
    struct obj **_obj;
    pop(__script, &_obj);
    next_obj(__script, mk_decr(_obj));
}

void emit_call(struct ffly_script *__script, struct node *__node) {
    struct obj **no;
    emit(__script, __node->no);
    pop(__script, &no);

    struct node **itr;
    struct obj **params[12] = {NULL};
    mdl_uint_t i = 0;
    if (ffly_vec_size(&__node->params)>0) {
        itr = (struct node**)ffly_vec_begin(&__node->params);
        while(itr <= (struct node**)ffly_vec_end(&__node->params)) {
            emit(__script, *itr);
            pop(__script, params+(i++));
            itr++;
        }
        params[i] = NULL;
    }

    i = 0;
    struct obj ***param = call(__script, no);
    while((*(param++) = params[i++]) != NULL);
}

void emit_addrof(struct ffly_script *__script, struct node *__node) {
    void *p;
    if (__node->operand->kind == _ast_func)
        p = &__node->operand->pair;
    else
        p = __node->operand->_obj;
    p = stack_push(&p, sizeof(void*));
    next_obj(__script, mk_op_assign(p, rg_64l_u->size, objpp(rg_64l_u)));
    push(__script, rg_64l_u);
}

void emit_ret(struct ffly_script *__script, struct node *__node) {
    emit(__script, __node->ret);
    struct obj **from;
    pop(__script, &from);

    next_obj(__script, mk_op_copy(__node->_type->size, objpp(rg_64l_u), from));
    push(__script, rg_64l_u);
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
        case _ast_exit:
            emit_exit(__script, __node);
        break;
        case _ast_while:
            emit_while(__script, __node);
        break;
        case _ast_incr:
            emit_incr(__script, __node);
        break;
        case _ast_decr:
            emit_decr(__script, __node);
        break;
        case _ast_call:
            emit_call(__script, __node);
        break;
        case _ast_addrof:
            emit_addrof(__script, __node);
        break;
        case _ast_ret:
            emit_ret(__script, __node);
        break;
        default:    
            ffly_printf("unknown.\n");
	}
}

ffly_err_t ffly_script_gen(struct ffly_script *__script, void **__top, ffly_byte_t **__stack) {
	if (!ffly_vec_size(&__script->nodes)) return FFLY_FAILURE;
    stack = __stack;
    rg_8l_u = __fresh(__script, 1);
    rg_8l_u->_type = _8l_u;

    rg_16l_u = __fresh(__script, 2);
    rg_8l_u->_type = _16l_u;

    rg_32l_u = __fresh(__script, 4);
    rg_8l_u->_type = _32l_u;

    rg_64l_u = __fresh(__script, 8);
    rg_8l_u->_type = _64l_u;

    if (ffly_vec_size(&__script->nodes)>0) {
    	struct node **itr = (struct node**)ffly_vec_begin(&__script->nodes);
    	while(itr <= (struct node**)ffly_vec_end(&__script->nodes)) {
    		emit(__script, *itr);
    		itr++;
    	}
    }
    *__top = (void*)top;
    return FFLY_SUCCESS;
}
