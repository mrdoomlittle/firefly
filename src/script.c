# define __ffly_script_internal
# include "script.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/io.h"
# include "system/file.h"
# include "data/str_cmp.h"
# include "data/mem_cpy.h"
# include "data/mem_set.h"
# include "data/str_len.h"
# include "system/errno.h"
# include "data/bcopy.h"
# include "system/nanosleep.h"
# include "data/str_cpy.h"
# include <stdlib.h>
# include <stdarg.h>
void _errmsg(char const *__file, int unsigned __line, char const *__s, ...) {
    char buf[200];
    *buf = '\0';
    va_list args;
    va_start(args, __s);
    ffly_vsprintf(buf, __s, args);
    va_end(args);
    ffly_fprintf(ffly_out, "%s:%u, %s\n", __file, __line, buf);
}

ffly_err_t ffly_script_ld(struct ffly_script *__script, char *__file) {
	ffly_err_t err;
	FF_FILE *f = ffly_fopen(__file, FF_O_RDONLY, 0, &err);
    __script->file->path = realpath(__file, NULL);

	struct ffly_stat st;
	ffly_fstat(__file, &st);

	__script->file->p = (ffly_byte_t*)__ffly_mem_alloc(st.size);
	__script->file->end = __script->file->p+st.size;
	ffly_fread(f, __script->file->p, st.size);
	ffly_fclose(f);
    return FFLY_SUCCESS;
}

char const* tokk_str(mdl_u8_t __kind) {
	switch(__kind) {
		case TOK_IDENT: return "ident";
		case TOK_KEYWORD: return "keyword";
		case TOK_NO: return "number";
        case TOK_NEWLINE: return "newline";
	}

	return "unknown";
}

char const* tokid_str(mdl_u8_t __id) {
	switch(__id) {
        case _k_void: return "void";
        case _k_float: return "float";
		case _k_print: return "print";
		case _k_uint_t: return "uint_t";
		case _k_int_t: return "int_t";
		case _k_u64_t: return "u64_t";
		case _k_i64_t: return "i64_t";
		case _k_u32_t: return "u32_t";
		case _k_i32_t: return "i32_t";
		case _k_u16_t: return "u16_t";
		case _k_i16_t: return "i16_t";
		case _k_u8_t: return "u8_t";
		case _k_i8_t: return "i8_t";
        case _k_var: return "var";
		case _semicolon: return ";";
		case _eq: return "=";
        case _incr: return "incr";
        case _decr: return "decr";
        case _colon: return "colon";
        case _comma: return "comma";
        case _l_arrow: return "left arrow";
        case _r_arrow: return "right arrow";
        case _k_if: return "if";
        case _l_brace: return "left brace";
        case _r_brace: return "right brace";
        case _l_paren: return "left paren";
        case _r_paren: return "right paren";
        case _k_struct: return "struct";
        case _k_else: return "else";
        case _k_ret: return "return";
        case _period: return ".";
        case _k_match: return "match";
        case _gt: return "grater then";
        case _lt: return "less then";
        case _k_fn: return "function";
        case _k_while: return "while";
        case _k_exit: return "exit";
        case _k_typedef: return "typedef";
	}
	return "unknown";
}

void pr_tok(struct token *__tok) {
	if (!__tok) return;
	ffly_printf("tok kind: %s\n", tokk_str(__tok->kind));
	ffly_printf("tok id: %s\n", tokid_str(__tok->id));
}

char const* opstr(mdl_u8_t __op) {
	switch(__op) {
		case _op_fresh: return "fresh";
        case _op_free: return "free";
		case _op_assign: return "assign";
		case _op_copy: return "copy";
		case _op_print: return "print";
        case _op_jump: return "jump";
        case _op_cond_jump: return "cond jump";
        case _op_incr: return "incr";
        case _op_decr: return "decr";
        case _op_zero: return "zero";
        case _op_push: return "push";
        case _op_pop: return "pop";
        case _op_compare: return "compare";
	}
	return "unknown";
}

void op_fresh(ffscriptp __script, struct obj *__obj) {
    __obj->p = (void*)__script->fresh;
    __script->fresh+=__obj->size; 
}

void op_assign(ffscriptp __script, struct obj *__obj) {
	ffly_mem_cpy((*__obj->to)->p, __obj->p, __obj->size);
}

void op_copy(ffscriptp __script, struct obj *__obj) {
	ffly_mem_cpy((*__obj->to)->p, (*__obj->from)->p, __obj->size);
}

void vec_cleanup(struct ffly_script *__script, struct ffly_vec *__vec) {
    struct ffly_vec *vec;
    ffly_vec_push_back(&__script->vecs, (void**)&vec);
    *vec = *__vec;
}

void map_cleanup(struct ffly_script *__script, struct ffly_map *__map) {
    struct ffly_map *map;
    ffly_vec_push_back(&__script->maps, (void**)&map);
    *map = *__map;
}

void cleanup(struct ffly_script *__script, void *__p) {
    void **p;
    ffly_vec_push_back(&__script->to_free, (void**)&p);
    *p = __p;
}

void op_compare(ffscriptp __script, struct obj *__obj) {
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

void op_jump(ffscriptp __script, struct obj *__objp) {
    struct obj *_obj = *(struct obj**)__objp;
    if (!_obj->jmp) return;
    if (!*_obj->jmp) return;
    *(struct obj**)__objp = **_obj->jmp;
}

void op_cond_jump(ffscriptp __script, struct obj *__objp) {
    struct obj *_obj = *(struct obj**)__objp;
    if (((*(mdl_u8_t*)_obj->flags->p)&_obj->cond)>0)
        op_jump(__script, __objp);
}

// debug
void op_print(ffscriptp __script, struct obj *__obj) {
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

void op_zero(ffscriptp __script, struct obj *__obj) {
    ffly_mem_set((*__obj->dst)->p, 0x0, (*__obj->dst)->size);
}

/*
    object passing.
*/
static struct obj *stack[20];
static struct obj **top = stack;
void op_push(ffscriptp __script, struct obj *__obj) {
    *(top++) = __obj->_obj;
}

void op_pop(ffscriptp __script, struct obj *__obj) {
    __obj->_obj = *(--top);
}

void op_incr(ffscriptp __script, struct obj *__obj) {
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

void op_decr(ffscriptp __script, struct obj *__obj) {
       
}

void op_free(ffscriptp __script, struct obj *__obj) {
    __script->fresh-=__obj->size;
}

void nanosleep(struct obj ***__params) {
    mdl_u64_t sec = *(mdl_u64_t*)(**(__params++))->p;
    mdl_u64_t nsec = *(mdl_u64_t*)(**__params)->p;
    ffly_nanosleep(sec, nsec);
}

void mutex_lock(struct obj ***__params) {
    ffly_mutex_t *mutex = (ffly_mutex_t*)(*((struct obj**)(**__params)->p))->p;
    ffly_mutex_lock(mutex);
}

void mutex_unlock(struct obj ***__params) {
    ffly_mutex_t *mutex = (ffly_mutex_t*)(*((struct obj**)(**__params)->p))->p;
    ffly_mutex_unlock(mutex);
}

struct ffly_buff buffers[20];
ffly_buffp fresh_buff = buffers;
ffly_buffp free_buffs[20];
ffly_buffp *next_buff = free_buffs;

void creat_buff(struct obj ***__params) {
    mdl_uint_t *id = (mdl_uint_t*)(*((struct obj**)(**(__params++))->p))->p;
    mdl_uint_t size = *(mdl_uint_t*)(**(__params++))->p;
    mdl_uint_t blk_size = *(mdl_uint_t*)(**__params)->p;

    ffly_buffp buff = fresh_buff++;
    *id = buff-buffers;
    ffly_buff_init(buff, size, blk_size); 
}

void del_buff(struct obj ***__params) {
    mdl_uint_t id = *(mdl_uint_t*)(**__params)->p;    
    ffly_buff_de_init(buffers+id);
}

void buff_incr(struct obj ***__params) {
    mdl_uint_t id = *(mdl_uint_t*)(**__params)->p;
    ffly_buff_incr(buffers+id);
}

void buff_decr(struct obj ***__params) {
    mdl_uint_t id = *(mdl_uint_t*)(**__params)->p;
    ffly_buff_decr(buffers+id);
}

void buff_off(struct obj ***__params) {
    mdl_uint_t id = *(mdl_uint_t*)(**(__params++))->p;
    mdl_uint_t *off = (mdl_uint_t*)(*((struct obj**)(**__params)->p))->p;
    *off = ffly_buff_off(buffers+id);
}

void buff_resize(struct obj ***__params) {
    mdl_uint_t id = *(mdl_uint_t*)(**(__params++))->p;
    mdl_uint_t size = *(mdl_uint_t*)(**__params)->p;
    ffly_buff_resize(buffers+id, size);
}

void buff_size(struct obj ***__params) {
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

void op_call(ffscriptp __script, struct obj *__obj) {
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

void op_frame(ffscriptp __script, struct obj *__obj) {
    __obj->p = (void*)__script->fresh;
}

void op_free_frame(ffscriptp __script, struct obj *__obj) {
    __script->fresh = (ffly_byte_t*)__obj->frame->p;
}

void op_conv(ffscriptp __script, struct obj *__obj) {
    struct obj *dst = *__obj->dst;
    struct obj *src = *__obj->src;
    if ((src->_type >= _64l_u && src->_type <= _8l_u)
        && (dst->_type >= _64l_u && dst->_type <= _8l_u)) {
        mdl_u64_t tmp = 0;
        ffly_bcopy(&tmp, src->p, src->size);
        ffly_bcopy(dst->p, &tmp, dst->size);
    }
}

void(*op[])(ffscriptp, struct obj*) = {
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
    &op_conv
};

ffly_err_t ffscript_exec(ffscriptp __script, void*(*__call)(mdl_u8_t, void*, void**), void *__arg_p, void *__entry, void *__end) {
    __script->call = __call;
    __script->arg_p = __arg_p;
	struct obj *_obj = (struct obj*)(!__entry?__script->top:__entry);
	while(_obj != (struct obj*)__end) {
        ffly_fprintf(ffly_log, "op: %s\n", opstr(_obj->opcode));
        if (_obj->opcode == _op_exit) break;
		if (_obj->opcode >= _op_fresh && _obj->opcode <= _op_conv) {
            mdl_u8_t isjmp = (_obj->opcode == _op_jump || _obj->opcode == _op_cond_jump);
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

void* ffscript_call(ffscriptp __script, void *__func) {
    ffly_pair *func = *(ffly_pair**)__func;
    ffscript_exec(__script, NULL, NULL, *(struct obj**)func->p0, *(struct obj**)func->p1);    
    return NULL;
}

ffly_bool_t is_keyword(struct token *__tok, mdl_u8_t __id) {
	return (__tok->kind == TOK_KEYWORD && __tok->id == __id);
}

ffly_off_t toklo(struct token *__tok) {
    return __tok->lo;
}

ffly_off_t curlo(struct ffly_script *__script) {
    return __script->file->lo;
}

ffly_bool_t next_token_is(struct ffly_script *__script, mdl_u8_t __kind, mdl_u8_t __id) {
	struct token *tok = next_token(__script);
	if (!tok) return 0;
	if (tok->kind == __kind && tok->id == __id)
		return 1;
	ffly_script_ulex(__script, tok);
	return 0;
}

mdl_uint_t tokcol(struct token *__tok) {
    return __tok->off-toklo(__tok);
}

void print_line(struct ffly_script *__script, mdl_uint_t __off) {
    char *p = (char*)__script->file->p+__off;
    while(*p != '\n' && *p != '\0') {
        ffly_printf("%c", *p);
        p++;
    }
    ffly_printf("\n");
}

# define expectmsg(__s) ffly_fprintf(ffly_out, "%s:%u;%u: %s, got %s.\n", __script->file->path, tokl(tok), tokcol(tok), __s, tokid_str(tok->id))
ffly_bool_t expect_token(struct ffly_script *__script, mdl_u8_t __kind, mdl_u8_t __id) {
	struct token *tok = next_token(__script);
    if (!tok) {
        errmsg("token is null.");
        return 0;
    }

	mdl_u8_t ret;
    if (!(ret = (tok->kind == __kind && tok->id == __id))) {
        if (__kind == TOK_KEYWORD) {
            switch(__id) {
                case _colon:
                    expectmsg("expected colon");
                break;
                case _l_brace:
                    expectmsg("expected left side brace");
                break;
                case _r_brace:
                    expectmsg("expected right side brace");
                break;
                case _semicolon:
                    expectmsg("expected semicolon");
                break;
                case _l_paren:
                    expectmsg("expected left side paren");
                break;
                case _r_paren:
                    expectmsg("expected right side paren");
                break;
                case _l_arrow:
                    expectmsg("expected left arrow");
                break;
                case _r_arrow:
                    expectmsg("expected right arrow");
                break;
            }
            print_line(__script, tok->lo);
            mdl_uint_t pad = tok->off-toklo(tok);
            while(pad != 0) {
                ffly_printf(" ");
                pad--;
            }

            ffly_printf("^\n");
        }
    }
    return ret;
}

ffly_err_t ffscript_free(ffscriptp __script) {
    struct obj *_obj = (struct obj*)__script->top, *prev = NULL;
    while(_obj != NULL) {
        if (prev != NULL)
            __ffly_mem_free(prev);
        prev = _obj;
        _obj = _obj->next;
    }

    if (prev != NULL)
        __ffly_mem_free(prev);
    __ffly_mem_free(__script->stack);
    retok;
}

ffly_err_t ffly_script_free(struct ffly_script *__script) {
	struct ffly_vec *vec;
	if (ffly_vec_size(&__script->vecs)>0) {
        vec = (struct ffly_vec*)ffly_vec_begin(&__script->vecs);
		while(vec <= (struct ffly_vec*)ffly_vec_end(&__script->vecs)) {
            ffly_vec_de_init(vec);
			vec++;
		}
	}

	struct ffly_map *map;
	if (ffly_vec_size(&__script->maps)>0) {
        map = (struct ffly_map*)ffly_vec_begin(&__script->maps);
		while(map <= (struct ffly_map*)ffly_vec_end(&__script->maps)) {
            ffly_map_de_init(map);
	        map++;
		}
	}

    ffly_map_de_init(&__script->env);
    ffly_buff_de_init(&__script->sbuf);
    ffly_vec_de_init(&__script->vecs);
	ffly_vec_de_init(&__script->maps);
    ffly_vec_de_init(&__script->nodes);
	ffly_buff_de_init(&__script->iject_buff);

    void **p;
    if (ffly_vec_size(&__script->to_free)>0) {
        p = (void**)ffly_vec_begin(&__script->to_free);
        while(p <= (void**)ffly_vec_end(&__script->to_free)) {
            __ffly_mem_free(*p);
            p++;
        }
    }
    ffly_vec_de_init(&__script->to_free);
    __ffly_mem_free(__script->file->p);
    ffly_map_de_init(&__script->macros);
    ffly_map_de_init(&__script->typedefs);
    free(__script->file->path);
    __ffly_mem_free(__script->file);
    return FFLY_SUCCESS;
}

struct token* peek_token(struct ffly_script *__script) {
	struct token *tok = next_token(__script);
	ffly_script_ulex(__script, tok);
	return tok;
}

ffly_bool_t next_tok_nl(struct ffly_script *__script) {
    ffly_err_t err;
    struct token *tok = ffly_script_lex(__script, &err);
    if (tok != NULL) {
        if (tok->kind == TOK_NEWLINE) {
            __ffly_mem_free(tok);
            return 1;
        }
        ffly_script_ulex(__script, tok);
    }
    return 0;
}

void read_define(struct ffly_script *__script) {
    struct token *name = next_token(__script);
    void *p = NULL;
    if (!next_tok_nl(__script)) {
        struct ffly_vec *toks = (ffly_vecp)__ffly_mem_alloc(sizeof(struct ffly_vec));
        ffly_vec_set_flags(toks, VEC_AUTO_RESIZE);
        ffly_vec_init(toks, sizeof(struct token*));
        p = toks;
        struct token **tok;
        _next:
        ffly_vec_push_back(toks, (void**)&tok);
        *tok = next_token(__script); 
 
        if (!next_tok_nl(__script)) {
            goto _next;
        }
        vec_cleanup(__script, toks);
        cleanup(__script, toks);
    }
    ffly_map_put(&__script->macros, name->p, ffly_str_len((char*)name->p), p);
}

ffly_bool_t is_endif(struct ffly_script *__script, struct token *__tok) {
    if (is_keyword(__tok, _percent)) {
        ffly_err_t err;
        struct token *tok = ffly_script_lex(__script, &err);
        return !ffly_str_cmp(tok->p, "endif");
    }
    return 0;
}

void skip_until_endif(struct ffly_script *__script) {
    ffly_err_t err;
    struct token *tok = NULL;
    for(;;) {
        if (!(tok = ffly_script_lex(__script, &err))) break;
        if (_err(err)) break;
        if (is_endif(__script, tok)) break;
        if (tok->kind == TOK_NEWLINE)
            __ffly_mem_free(tok);
    }
}

void read_ifdef(struct ffly_script *__script) {
    struct token *name = next_token(__script);
    ffly_err_t err;
    ffly_map_get(&__script->macros, name->p, ffly_str_len((char*)name->p), &err);
    if (_err(err)) skip_until_endif(__script);
}

void read_ifndef(struct ffly_script *__script) {
    struct token *name = next_token(__script);
    ffly_err_t err;
    ffly_map_get(&__script->macros, name->p, ffly_str_len((char*)name->p), &err);
    if (_ok(err)) skip_until_endif(__script);
}

# include <unistd.h>
void read_include(struct ffly_script *__script) {
    struct token *file = next_token(__script);
    __script->file++;
    ffly_printf("include: %s\n", (char*)file->p);

    if (access((char*)file->p, F_OK) == -1) {
        ffly_fprintf(ffly_err, "include file doesen't exist.\n");
        return;
    }

    __script->file->line = 0;
    __script->file->lo = 0;
    __script->file->off = 0;
    ffly_script_ld(__script, (char*)file->p);  
    ffly_script_parse(__script);
    free(__script->file->path);
    __ffly_mem_free(__script->file->p);
    __script->file--;
}

void read_macro(struct ffly_script *__script) {
    struct token *tok = next_token(__script);
    if (tok->kind != TOK_IDENT) return;

    if (!ffly_str_cmp(tok->p, "define")) {
        read_define(__script);
    } else if (!ffly_str_cmp(tok->p, "ifdef")) {
        read_ifdef(__script);
    } else if (!ffly_str_cmp(tok->p, "ifndef")) {
        read_ifndef(__script);
    } else if (!ffly_str_cmp(tok->p, "include")) {
        read_include(__script);
    }
}

ffly_bool_t maybe_keyword(struct token*);
struct token* next_token(struct ffly_script *__script) {
	ffly_err_t err;
	struct token *tok;
    _back:
    tok = ffly_script_lex(__script, &err);
    if (!tok) return NULL;

    if (tok->kind == TOK_NEWLINE) {
        __ffly_mem_free(tok);
        goto _back;
    }

    if (is_keyword(tok, _percent)) {
        read_macro(__script);
        goto _back;
    }

    if (tok->kind == TOK_IDENT) {
        struct ffly_vec *toks = (struct ffly_vec*)ffly_map_get(&__script->macros, tok->p, ffly_str_len((char*)tok->p), &err);
        if (toks != NULL && _ok(err)) {
            struct token **itr = (struct token**)ffly_vec_end(toks);
            while(itr >= (struct token**)ffly_vec_begin(toks)) {
                ffly_script_ulex(__script, *itr); 
                itr--;
            }
            tok = next_token(__script);
        }
    }

	maybe_keyword(tok);
	return tok;
}

mdl_uint_t tokl(struct token *__tok) {
    return __tok->line;
}

void to_keyword(struct token *__tok, mdl_u8_t __id) {
	__tok->kind = TOK_KEYWORD;
	__tok->id = __id;
}

ffly_bool_t maybe_keyword(struct token *__tok) {
	if (!__tok) return 0;
	if (__tok->kind != TOK_IDENT || __tok->p == NULL) return 0;
	if (!ffly_str_cmp(__tok->p, "print"))
		to_keyword(__tok, _k_print);
    else if (!ffly_str_cmp(__tok->p, "if"))
        to_keyword(__tok, _k_if);
	else if (!ffly_str_cmp(__tok->p, "uint_t"))
		to_keyword(__tok, _k_uint_t);
	else if (!ffly_str_cmp(__tok->p, "int_t"))
		to_keyword(__tok, _k_int_t);
	else if (!ffly_str_cmp(__tok->p, "u64_t"))
		to_keyword(__tok, _k_u64_t);
	else if (!ffly_str_cmp(__tok->p, "i64_t"))
		to_keyword(__tok, _k_i64_t);
	else if (!ffly_str_cmp(__tok->p, "u32_t"))
		to_keyword(__tok, _k_u32_t);
	else if (!ffly_str_cmp(__tok->p, "i32_t"))
		to_keyword(__tok, _k_i32_t);
	else if (!ffly_str_cmp(__tok->p, "u16_t"))
		to_keyword(__tok, _k_u16_t);
	else if (!ffly_str_cmp(__tok->p, "i16_t"))
		to_keyword(__tok, _k_i16_t);
	else if (!ffly_str_cmp(__tok->p, "u8_t"))
		to_keyword(__tok, _k_u8_t);
	else if (!ffly_str_cmp(__tok->p, "i8_t"))
		to_keyword(__tok, _k_i8_t);
    else if (!ffly_str_cmp(__tok->p, "fn"))
        to_keyword(__tok, _k_fn);
    else if (!ffly_str_cmp(__tok->p, "extern"))
        to_keyword(__tok, _k_extern);
    else if (!ffly_str_cmp(__tok->p, "struct"))
        to_keyword(__tok, _k_struct);
    else if (!ffly_str_cmp(__tok->p, "void"))
        to_keyword(__tok, _k_void);
    else if (!ffly_str_cmp(__tok->p, "var"))
        to_keyword(__tok, _k_var);
    else if (!ffly_str_cmp(__tok->p, "exit"))
        to_keyword(__tok, _k_exit);
    else if (!ffly_str_cmp(__tok->p, "while"))
        to_keyword(__tok, _k_while);
    else if (!ffly_str_cmp(__tok->p, "match"))
        to_keyword(__tok, _k_match);
    else if (!ffly_str_cmp(__tok->p, "else"))
        to_keyword(__tok, _k_else);
    else if (!ffly_str_cmp(__tok->p, "float"))
        to_keyword(__tok, _k_float);
    else if (!ffly_str_cmp(__tok->p, "typedef"))
        to_keyword(__tok, _k_typedef);
    else if (!ffly_str_cmp(__tok->p, "ret"))
        to_keyword(__tok, _k_ret);
    else if (!ffly_str_cmp(__tok->p, "brk"))
        to_keyword(__tok, _k_brk);
    else {
		return 0;
	}
	return 1;
}

ffly_err_t ffscript_init(ffscriptp __script, mdl_uint_t __stack_size) {
    __script->stack = (ffly_byte_t*)__ffly_mem_alloc(__stack_size);
    __script->fresh = __script->stack;
    retok;
}

ffly_err_t ffly_script_prepare(struct ffly_script *__script) {
    ffly_err_t err;
    __script->file = (struct ffly_script_file*)__ffly_mem_alloc(12*sizeof(struct ffly_script_file));
	if (_err(err = ffly_buff_init(&__script->sbuf, 100, 1))) {
        errmsg("failed to init buffer.");
        return err;
    }

    ffly_vec_set_flags(&__script->to_free, VEC_AUTO_RESIZE);
    if (_err(err = ffly_vec_init(&__script->to_free, sizeof(void*)))) {
        errmsg("failed to init vec.");
        return err;
    }

    ffly_vec_set_flags(&__script->vecs, VEC_AUTO_RESIZE);
	if (_err(err = ffly_vec_init(&__script->vecs, sizeof(struct ffly_vec)))) {
        errmsg("failed to init vec.");
        return err;
    }

    ffly_vec_set_flags(&__script->vecs, VEC_AUTO_RESIZE);
	if (_err(err = ffly_vec_init(&__script->maps, sizeof(struct ffly_map)))) {
        errmsg("failed to init vec.");
        return err;
    }

    ffly_vec_set_flags(&__script->nodes, VEC_AUTO_RESIZE);
    if (_err(err = ffly_vec_init(&__script->nodes, sizeof(struct node*)))) {
        errmsg("failed to init vec.");
        return err;
    }

	if (_err(err = ffly_buff_init(&__script->iject_buff, 100, sizeof(struct token*)))) {
        errmsg("failed to init buffer.");
        return err;
    }

    if (_err(err = ffly_map_init(&__script->env, _ffly_map_127))) {
        errmsg("failed to init map.");
        return err;
    }

    if (_err(err = ffly_map_init(&__script->typedefs, _ffly_map_127))) {
        errmsg("failed to init map.");
        return err;
    }

    __script->brkp = __script->brk;
	__script->file->off = 0;
    __script->file->line = 0;
    __script->file->lo = 0;
    __script->local = NULL;
    if (_err(err = ffly_map_init(&__script->macros, _ffly_map_127))) {
        errmsg("failed to init map.");
        return err;
    }
    __script->ret_type = NULL;
    return FFLY_SUCCESS;
}

ffly_err_t ffly_script_finalize(struct ffly_script *__script) {

}

ffly_err_t ffly_script_build(struct ffly_script *__script, void ** __top, ffly_byte_t **__stack) {
    ffly_err_t err;
    if (_err(err = ffly_script_parse(__script))) {
        errmsg("failed to parse.");
        return err;
    }
    if (_err(err = ffly_script_gen(__script, __top, __stack))) {
        errmsg("failed to generate.");
        return err;
    }

    ffly_script_finalize(__script);
    ffly_fprintf(ffly_out, "build successful.\n");
    return FFLY_SUCCESS;
}

ffly_bool_t is_eof(struct ffly_script *__script) {
	return (__script->file->p+__script->file->off) >= __script->file->end;
}

mdl_uint_t curl(struct ffly_script *__script) {
    return __script->file->line;
}

void skip_token(struct ffly_script *__script) {
    next_token(__script);
}

ffly_err_t ffly_script_save_bin(struct ffly_script *__script, char *__file) {
/*
    ffly_err_t err;
    FF_FILE *f = ffly_fopen(__file, FF_O_WRONLY|FF_O_TRUNC|FF_O_CREAT, FF_S_IRUSR|FF_S_IWUSR, &err);
    ffly_fseek(f, sizeof(mdl_u32_t), FF_SEEK_SET);
    mdl_u32_t top = ffly_fseek(f, 0, FF_SEEK_CUR);
    mdl_u32_t off = top;

    mdl_u32_t prev;
    struct obj *_obj = (struct obj*)__script->top;
    struct obj o;
    while(_obj != NULL) {
        ffly_printf("\n");
        pr_obj(_obj);
        _obj->off = off;
        o = *_obj;
        off+= sizeof(struct obj);
        ffly_fseek(f, off, FF_SEEK_SET);
        if (_obj->_type != NULL) {            
            o._type = (void*)off;
            ffly_fwrite(f, _obj->_type, sizeof(struct type));
            off+= sizeof(struct type);

            if (_obj->p != NULL) {
                o.p = (void*)off;
                ffly_fwrite(f, _obj->p, _obj->_type->size);
                ffly_printf("data: %u\n", *(mdl_u16_t*)_obj->p);
                off+= _obj->_type->size;
            }

            ffly_printf("types, off: %u\n", (mdl_u32_t)o._type);
        }

        if (_obj->to != NULL) {
            ffly_printf("--to-- %u\n", _obj->to->off);
            o.to = (void*)_obj->to->off;
        }
        if (_obj->from != NULL) {
            ffly_printf("--from-- %u\n", _obj->from->off);
            o.from = (void*)_obj->from->off;
        }
        if (_obj->val != NULL) {
            ffly_printf("--val-- %u\n", _obj->val->off);
            o.val = (void*)_obj->val->off;
        }
        if (_obj->l != NULL) {
            o.l = (void*)_obj->l->off;
        }
        if (_obj->r != NULL) {
            o.r = (void*)_obj->r->off;
        }
        if (_obj->opcode == _op_jump || _obj->opcode == _op_cond_jump) { 
            o.jmp = !*_obj->jmp?NULL:(void**)(*_obj->jmp)->off;
        }

        if (_obj->flags != NULL) {
            o.flags = (void*)_obj->flags->off;
        }

        if ((void*)_obj != __script->top) {
            void *off = (void*)_obj->off;
            ffly_fseek(f, prev, FF_SEEK_SET);
            ffly_fwrite(f, &off, sizeof(void*));
        }

        ffly_printf("obj, off: %u\n", _obj->off);
        ffly_fseek(f, _obj->off, FF_SEEK_SET); 
        ffly_fwrite(f, &o, sizeof(struct obj));
        prev = _obj->off+offsetof(struct obj, next);
        _obj = _obj->next;
    }

    ffly_fseek(f, 0, FF_SEEK_SET);
    ffly_fwrite(f, &top, sizeof(mdl_u32_t));
    ffly_fclose(f);
*/
    return FFLY_SUCCESS;
}

# include "system/bin_tree.h"
ffly_err_t ffly_script_ld_bin(struct ffly_script *__script, char *__file) {
/*
    ffly_err_t err;
    FF_FILE *f = ffly_fopen(__file, FF_O_RDONLY, 0, &err);
    struct ffly_bin_tree objs;

    ffly_bin_tree_init(&objs);

    mdl_u32_t off;
    ffly_fread(f, &off, sizeof(mdl_u32_t));
    struct obj *_obj, *prev = NULL;
    struct obj *top = NULL;

    _next:
    ffly_printf("\n");
    _obj = (struct obj*)__ffly_mem_alloc(sizeof(struct obj));
    if (!top)
        top = _obj;
    ffly_fseek(f, off, FF_SEEK_SET);
    ffly_fread(f, _obj, sizeof(struct obj));
    ffly_bin_tree_insert(&objs, off, _obj);
    if (_obj->_type != NULL) {
        ffly_fseek(f, (mdl_u32_t)_obj->_type, FF_SEEK_SET);
        _obj->_type = (struct obj*)__ffly_mem_alloc(sizeof(struct type));
        ffly_fread(f, _obj->_type, sizeof(struct type));
    
        ffly_printf("---> size: %u\n", _obj->_type->size);
        if (_obj->p != NULL) {
            ffly_fseek(f, (mdl_u32_t)_obj->p, FF_SEEK_SET);
            if (_obj->_type->size < 100) { // in case 
                _obj->p = __ffly_mem_alloc(_obj->_type->size);
                ffly_fread(f, _obj->p, _obj->_type->size);
            }
            ffly_printf("data: %u\n", *(mdl_u16_t*)_obj->p);
        }
    }

    if (_obj->to != NULL)
        ffly_bin_tree_find(&objs, (mdl_u32_t)_obj->to, (void**)&_obj->to);
    if (_obj->from != NULL)
        ffly_bin_tree_find(&objs, (mdl_u32_t)_obj->from, (void**)&_obj->from);
    if (_obj->val != NULL)
        ffly_bin_tree_find(&objs, (mdl_u32_t)_obj->val, (void**)&_obj->val); 
    if (_obj->l != NULL)
        ffly_bin_tree_find(&objs, (mdl_u32_t)_obj->l, (void**)&_obj->l);
    if (_obj->r != NULL)
        ffly_bin_tree_find(&objs, (mdl_u32_t)_obj->r, (void**)&_obj->r);
    
    if (_obj->opcode == _op_jump || _obj->opcode == _op_cond_jump) {
        struct obj **p = (struct obj**)__ffly_mem_alloc(sizeof(struct obj*));
        if (_obj->jmp != NULL) {
            ffly_bin_tree_find(&objs, (mdl_u32_t)_obj->jmp, (void**)p);
        } else
            *p = NULL;
        _obj->jmp = p;
    }

    if (_obj->flags != NULL)
        ffly_bin_tree_find(&objs, (mdl_u32_t)_obj->flags, (void**)&_obj->flags);

    pr_obj(_obj);
    if (prev != NULL)
        prev->next = _obj;
    prev = _obj;

    if (_obj->next != NULL) {
        off = (mdl_u32_t)_obj->next;
        goto _next;
    }
    _obj->next = NULL;

    __script->top = (void*)top;
    ffly_fclose(f);
    ffly_bin_tree_de_init(&objs);
*/
    return FFLY_SUCCESS;
}
//# define LOAD
# define DEBUG

# ifdef DEBUG
void* me(mdl_u8_t __id, void *__arg_p, void **__p) {
    printf("HI, id{%u}\n", __id);
/*
    while(*__p != NULL) {
        printf("...\n");
        __p++;
    }
*/
  //  printf("HI, id{%u}\n", *(mdl_u8_t*)*__p);

    ffscript_call((ffscriptp)__arg_p, *__p);
    return NULL;
}

void pr();
void pf();

int main() {
    ffly_io_init();
	struct ffly_script script;
    ffscript ff;
    ffscript_init(&ff, 1000);
	ffly_printf("loading script.\n");
	if (_err(ffly_script_prepare(&script))) {
        ffly_printf("failed to prepare.\n");
        return -1;
    }
# ifndef LOAD
	ffly_script_ld(&script, "../scripts/main.ff");
	ffly_printf("building script.\n");
	if (_err(ffly_script_build(&script, &ff.top, &ff.fresh))) {
        ffly_printf("failed to build.\n");
        return -1;
    }
    ffly_script_free(&script);
	ffly_printf("executing script.\n");
	ffscript_exec(&ff, &me, &ff, NULL, NULL);
    ffly_printf("stack used: %lu\n", ff.fresh-ff.stack);
# else
    ffly_script_ld_bin(&script, "test.bin");
    ffscript_exec(&ff, &me, NULL, NULL, NULL);
# endif
    ffscript_free(&ff);
//    pr();
//    pf();
	ffly_io_closeup();
}

# endif
