# define __ffly_compiler_internal
# define __ffly_script_internal
# include "script.h"
# include "compiler.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/io.h"
# include "data/mem_cpy.h"
# include "data/mem_set.h"
# include "system/nanosleep.h"
# include "data/bcopy.h"

// move to dir named script
char const static* 
opstr(mdl_u8_t __op) {
	switch(__op) {
		case _op_fresh_: return "fresh";
        case _op_free_: return "free";
		case _op_assign_: return "assign";
		case _op_copy_: return "copy";
		case _op_print_: return "print";
        case _op_jump_: return "jump";
        case _op_cond_jump_: return "cond jump";
        case _op_incr_: return "incr";
        case _op_decr_: return "decr";
        case _op_zero_: return "zero";
        case _op_push_: return "push";
        case _op_pop_: return "pop";
        case _op_compare_: return "compare";
	}
	return "unknown";
}

void static 
op_fresh(ffscriptp __script, struct obj *__obj) {
    __obj->p = (void*)__script->fresh;
    __script->fresh+=__obj->size; 
}

void static 
op_assign(ffscriptp __script, struct obj *__obj) {
	ffly_mem_cpy((*__obj->to)->p, __obj->p, __obj->size);
}

void static 
op_copy(ffscriptp __script, struct obj *__obj) {
	ffly_mem_cpy((*__obj->to)->p, (*__obj->from)->p, __obj->size);
}

void static 
op_compare(ffscriptp __script, struct obj *__obj) {
    mdl_u8_t l[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    mdl_u8_t r[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    ffly_mem_cpy(l, (*__obj->l)->p, (*__obj->l)->size);
    ffly_mem_cpy(r, (*__obj->r)->p, (*__obj->r)->size);
    mdl_u8_t *flags = (mdl_u8_t*)__obj->flags->p;
    *flags = 0x0;
    switch((*__obj->l)->_type) {
        case _64l_u: case _32l_u: case _16l_u: case _8l_u:
            if (*(mdl_u64_t*)l == *(mdl_u64_t*)r) *flags |= _flg_eq; else *flags |= _flg_neq;
            if (*(mdl_u64_t*)l > *(mdl_u64_t*)r) *flags |= _flg_gt;
            if (*(mdl_u64_t*)l < *(mdl_u64_t*)r) *flags |= _flg_lt;
        break;
        case _64l_s: case _32l_s: case _16l_s: case _8l_s:
            if (*(mdl_u64_t*)l > (1<<((*__obj->l)->size*8))>>1) {
                *(mdl_u64_t*)l |= (~(mdl_u64_t)0)>>((*__obj->l)->size*8);
            }
            if (*(mdl_u64_t*)r > (1<<((*__obj->r)->size*8))>>1) {
                *(mdl_u64_t*)r |= (~(mdl_u64_t)0)>>((*__obj->r)->size*8);
            }

            if (*(mdl_i64_t*)l == *(mdl_i64_t*)r) *flags |= _flg_eq; else *flags |= _flg_neq;
            if (*(mdl_i64_t*)l > *(mdl_i64_t*)r) *flags |= _flg_gt;
            if (*(mdl_i64_t*)l < *(mdl_i64_t*)r) *flags |= _flg_lt;
        break;
    }
}

void static 
op_jump(ffscriptp __script, struct obj *__objp) {
    struct obj *_obj = *(struct obj**)__objp;
    if (!_obj->jmp) return;
    if (!*_obj->jmp) return;
    *(struct obj**)__objp = **_obj->jmp;
}

void static 
op_cond_jump(ffscriptp __script, struct obj *__objp) {
    struct obj *_obj = *(struct obj**)__objp;
    if (((*(mdl_u8_t*)_obj->flags->p)&_obj->cond)>0)
        op_jump(__script, __objp);
}
// debug
void static 
op_print(ffscriptp __script, struct obj *__obj) {
    void *p = (*__obj->val)->p;
	switch(__obj->_type) {
        case _float64:
            ffly_printf("%lf\n", *(double*)p);
        break;
		case _64l_u:
			ffly_printf("%lu\n", *(mdl_u64_t*)p);
		break;
		case _64l_s:
			ffly_printf("%ld\n", *(mdl_i64_t*)p);
		break;
		case _32l_u:
			ffly_printf("%u\n", *(mdl_u32_t*)p);
		break;
		case _32l_s:
			ffly_printf("%d\n", *(mdl_i32_t*)p);
		break;
		case _16l_u:
			ffly_printf("%u\n", *(mdl_u16_t*)p);
		break;
		case _16l_s:
			ffly_printf("%d\n", *(mdl_i16_t*)p);
		break;
		case _8l_u:
			ffly_printf("%u\n", *(mdl_u8_t*)p);
		break;
		case _8l_s:
			ffly_printf("%d\n", *(mdl_i8_t*)p);
		break;
        default:
            ffly_printf("unknown type.\n");
	}
}

void static 
op_zero(ffscriptp __script, struct obj *__obj) {
    ffly_mem_set((*__obj->dst)->p, 0x0, (*__obj->dst)->size);
}

/*
    object passing.
*/
static struct obj *stack[20];
static struct obj **top = stack;
void static 
op_push(ffscriptp __script, struct obj *__obj) {
    *(top++) = __obj->_obj;
}

void static 
op_pop(ffscriptp __script, struct obj *__obj) {
    __obj->_obj = *(--top);
}

void static 
op_incr(ffscriptp __script, struct obj *__obj) {
    mdl_u8_t val[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    mdl_u8_t size = (*__obj->val)->size;
    ffly_mem_cpy(val, (*__obj->val)->p, size);

    mdl_u8_t type = (*__obj->val)->_type;
    if (type >= _64l_u && type <= _8l_u) {
        (*(mdl_u64_t*)val)++;
    } else if (type >= _64l_s && type <= _8l_s) {
        if (*(mdl_u64_t*)val > ((mdl_u64_t)1<<(size*8))>>1)
            *(mdl_i64_t*)val = ((mdl_i64_t)((*(mdl_u64_t*)val)|((~(mdl_u64_t)0)<<((*__obj->val)->size*8))))+1;
        else
            (*(mdl_u64_t*)val)++;
    }

    ffly_mem_cpy((*__obj->val)->p, val, (*__obj->val)->size);
}

void static 
op_decr(ffscriptp __script, struct obj *__obj) {
       
}

void static 
op_free(ffscriptp __script, struct obj *__obj) {
    __script->fresh-=__obj->size;
}

void static 
nanosleep(struct obj ***__params) {
    mdl_u64_t sec = *(mdl_u64_t*)(**(__params++))->p;
    mdl_u64_t nsec = *(mdl_u64_t*)(**__params)->p;
    ffly_nanosleep(sec, nsec);
}

void static
mutex_lock(struct obj ***__params) {
    ffly_mutex_t *mutex = (ffly_mutex_t*)(*((struct obj**)(**__params)->p))->p;
    ffly_mutex_lock(mutex);
}

void static 
mutex_unlock(struct obj ***__params) {
    ffly_mutex_t *mutex = (ffly_mutex_t*)(*((struct obj**)(**__params)->p))->p;
    ffly_mutex_unlock(mutex);
}

static struct ffly_buff buffers[20];
static ffly_buffp fresh_buff = buffers;
static ffly_buffp free_buffs[20];
static ffly_buffp *next_buff = free_buffs;

void static 
creat_buff(struct obj ***__params) {
    mdl_uint_t *id = (mdl_uint_t*)(*((struct obj**)(**(__params++))->p))->p;
    mdl_uint_t size = *(mdl_uint_t*)(**(__params++))->p;
    mdl_uint_t blk_size = *(mdl_uint_t*)(**__params)->p;

    ffly_buffp buff = fresh_buff++;
    *id = buff-buffers;
    ffly_buff_init(buff, size, blk_size); 
}

void static 
del_buff(struct obj ***__params) {
    mdl_uint_t id = *(mdl_uint_t*)(**__params)->p;    
    ffly_buff_de_init(buffers+id);
}

void static 
buff_incr(struct obj ***__params) {
    mdl_uint_t id = *(mdl_uint_t*)(**__params)->p;
    ffly_buff_incr(buffers+id);
}

void static 
buff_decr(struct obj ***__params) {
    mdl_uint_t id = *(mdl_uint_t*)(**__params)->p;
    ffly_buff_decr(buffers+id);
}

void static 
buff_off(struct obj ***__params) {
    mdl_uint_t id = *(mdl_uint_t*)(**(__params++))->p;
    mdl_uint_t *off = (mdl_uint_t*)(*((struct obj**)(**__params)->p))->p;
    *off = ffly_buff_off(buffers+id);
}

void static 
buff_resize(struct obj ***__params) {
    mdl_uint_t id = *(mdl_uint_t*)(**(__params++))->p;
    mdl_uint_t size = *(mdl_uint_t*)(**__params)->p;
    ffly_buff_resize(buffers+id, size);
}

void static 
buff_size(struct obj ***__params) {
    mdl_uint_t id = *(mdl_uint_t*)(**(__params++))->p;
    mdl_uint_t *size = (mdl_uint_t*)(*((struct obj**)(**__params)->p))->p;
    *size = ffly_buff_size(buffers+id);
}

void static(*call[])(struct obj***) = {
    &nanosleep,
    &mutex_lock,
    &mutex_unlock,
    &creat_buff,
    &del_buff,
    &buff_incr,
    &buff_decr,
    &buff_off,
    &buff_resize,
    &buff_size
};

void static 
op_call(ffscriptp __script, struct obj *__obj) {
    mdl_u8_t no = *(mdl_u8_t*)(*__obj->no)->p;
    ffly_printf("called no %u\n", no);
    if (!no && __script->call != NULL) { // self
        void *params[12];
        mdl_uint_t i = 0;
        while(__obj->params[1+i] != NULL) {
            params[i] = (*(__obj->params[1+i]))->p;
            i++;
        }
        params[i] = NULL;
        __script->call(*(mdl_u8_t*)(**__obj->params)->p, __script->arg_p, params);
        return;
    }

    call[no-1](__obj->params);
    /*
        internal calls
    */
}

void static
op_frame(ffscriptp __script, struct obj *__obj) {
    __obj->p = (void*)__script->fresh;
}

void static
op_free_frame(ffscriptp __script, struct obj *__obj) {
    __script->fresh = (ffly_byte_t*)__obj->frame->p;
}

void static
op_conv(ffscriptp __script, struct obj *__obj) {
    struct obj *dst = *__obj->dst;
    struct obj *src = *__obj->src;
    if ((src->_type >= _64l_u && src->_type <= _8l_u)
        && (dst->_type >= _64l_u && dst->_type <= _8l_u)) {
        mdl_u64_t tmp = 0;
        ffly_bcopy(&tmp, src->p, src->size);
        ffly_bcopy(dst->p, &tmp, dst->size);
    }
}

void static
op_add(ffscriptp __script, struct obj *__obj) {
    struct obj *l = *__obj->l;
    struct obj *r = *__obj->r;
    if ((l->_type >= _64l_u && l->_type <= _8l_u) && (r->_type >= _64l_u && r->_type <= _8l_u)) {
    }   
}

void static
op_sub(ffscriptp __script, struct obj *__obj) {
    struct obj *l = *__obj->l;
    struct obj *r = *__obj->r;
    if ((l->_type >= _64l_u && l->_type <= _8l_u) && (r->_type >= _64l_u && r->_type <= _8l_u)) {
    }
}

void static
op_mul(ffscriptp __script, struct obj *__obj) {
    struct obj *l = *__obj->l;
    struct obj *r = *__obj->r;
    if ((l->_type >= _64l_u && l->_type <= _8l_u) && (r->_type >= _64l_u && r->_type <= _8l_u)) {
    }
}

void static
op_div(ffscriptp __script, struct obj *__obj) {
    struct obj *l = *__obj->l;
    struct obj *r = *__obj->r;
    if ((l->_type >= _64l_u && l->_type <= _8l_u) && (r->_type >= _64l_u && r->_type <= _8l_u)) {
    }
}

static void(*op[])(ffscriptp, struct obj*) = {
	&op_fresh,
    &op_free,
	&op_assign,
	&op_copy,
	&op_print,
    &op_compare,
    &op_jump,
    &op_cond_jump,
    &op_zero,
    &op_push,
    &op_pop,
    &op_incr,
    &op_decr,
    &op_call,
    NULL,
    &op_frame,
    &op_free_frame,
    &op_conv,
    &op_add,
    &op_sub,
    &op_mul,
    &op_div
};

ffly_err_t ffscript_exec(ffscriptp __script, void*(*__call)(mdl_u8_t, void*, void**), void *__arg_p, void *__entry, void *__end) {
    __script->call = __call;
    __script->arg_p = __arg_p;
	struct obj *_obj = (struct obj*)(!__entry?__script->top:__entry);
	while(_obj != (struct obj*)__end) {
        ffly_fprintf(ffly_log, "op: %s\n", opstr(_obj->opcode));
        if (_obj->opcode == _op_exit_) break;
		if (_obj->opcode >= _op_fresh_ && _obj->opcode <= _op_div_) {
            mdl_u8_t isjmp = (_obj->opcode == _op_jump_ || _obj->opcode == _op_cond_jump_);
            if (!isjmp)
			    op[_obj->opcode](__script, _obj);
            else {
                struct obj *p = _obj;
                op[_obj->opcode](__script, (struct obj*)&p);
                if (p != _obj) {
                    _obj = p;
                    continue;
                }
            }
            if (!_obj) break;
        }
		_obj = _obj->next;
	}
    return FFLY_SUCCESS;
}
