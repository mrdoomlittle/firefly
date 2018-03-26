# include "config.h"
# include "dep/str_len.h"
# include "dep/str_dup.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/io.h"
# include "system/file.h"
# include "system/string.h"
enum {
	_tok_ident,
	_tok_keyword,
	_tok_str,
	_tok_no,
	_tok_chr
};

enum {
	_tilde,
	_colon,
	_comma,
	_l_bracket,
	_r_bracket
};

ffly_bool_t static is_space(char __c) {
	return (__c == ' ' || __c == '\n' || __c == '\t');
}

char const* ffly_conf_str(void const *__p) {
	return (char*)((struct ffly_conf_val*)__p)->p;
}
char ffly_conf_chr(void const *__p) {
	return *(char*)((struct ffly_conf_val*)__p)->p;
}

mdl_u64_t ffly_conf_64l_u(void const *__p) {
	return *(mdl_u64_t*)((struct ffly_conf_val*)__p)->p;}
mdl_u32_t ffly_conf_32l_u(void const *__p) {
	return *(mdl_u32_t*)((struct ffly_conf_val*)__p)->p;}
mdl_u16_t ffly_conf_16l_u(void const *__p) {
	return *(mdl_u16_t*)((struct ffly_conf_val*)__p)->p;}
mdl_u8_t ffly_conf_8l_u(void const *__p) {
	return *(mdl_u8_t*)((struct ffly_conf_val*)__p)->p;}
mdl_i64_t ffly_conf_64l_s(void const *__p) {
	return *(mdl_i64_t*)((struct ffly_conf_val*)__p)->p;}
mdl_i32_t ffly_conf_32l_s(void const *__p) {
	return *(mdl_i32_t*)((struct ffly_conf_val*)__p)->p;}
mdl_i16_t ffly_conf_16l_s(void const *__p) {
	return *(mdl_i16_t*)((struct ffly_conf_val*)__p)->p;}
mdl_i8_t ffly_conf_8l_s(void const *__p) {
	return *(mdl_i8_t*)((struct ffly_conf_val*)__p)->p;}

/*
	deposit all into ffconf
*/
void ffly_conf_depos(struct ffly_conf *__conf, ffconfp __d) {
	__d->arrs = __conf->arrs;
	__d->free = __conf->free;
	__d->env = __conf->env;
}

void const* ffly_conf_get(ffconfp __conf, char const *__name) {
	ffly_err_t err;
	return ffly_map_get(&__conf->env, (mdl_u8_t const*)__name, ffly_str_len(__name), &err);
}

void const* ffly_conf_arr_elem(void const *__p, mdl_uint_t __no) {
	return *(void**)ffly_vec_at(&((struct ffly_conf_arr*)__p)->data, __no);
}

mdl_uint_t ffly_conf_arr_len(void const *__p) {
	return ((struct ffly_conf_arr*)__p)->l;
}

struct token {
	mdl_u8_t kind, id;
	void *p, *end;
};

struct token static* read_token(struct ffly_conf*, ffly_err_t*);
struct token static* peek_token(struct ffly_conf*, ffly_err_t*);
ffly_bool_t static
expect_token(struct ffly_conf *__conf, mdl_u8_t __kind, mdl_u8_t __id, ffly_err_t *__err) {
	struct token *tok = read_token(__conf, __err);
	if (_err(*__err) || !tok) return 0;
	mdl_u8_t ret_val;
	if (!(ret_val = (tok->kind == __kind && tok->id == __id))) {
		if (tok->kind == __kind) {
			switch(__id) {
				case _colon:
					ffly_fprintf(ffly_out, "expected colon.\n");
				break;
				case _comma:
					ffly_fprintf(ffly_out, "expected comma.\n");
				break;
				case _l_bracket:
					ffly_fprintf(ffly_out, "expected left side bracket.\n");
				break;
				case _r_bracket:
					ffly_fprintf(ffly_out, "expected right side bracket.\n");
				break;
			}
		}
	}
	*__err = FFLY_SUCCESS;
	return ret_val;
}

ffly_err_t static push_free(struct ffly_conf *__conf, void *__p) {
	void **p;
	ffly_vec_push_back(&__conf->free, (void**)&p);
	*p = __p;
}

ffly_err_t static push_arr(struct ffly_conf *__conf, struct ffly_conf_arr *__arr) {
	struct ffly_conf_arr **p;
	ffly_vec_push_back(&__conf->arrs, (void**)&p);
	*p = __arr;
}

ffly_bool_t static is_eof(struct ffly_conf *__conf) {
	return (__conf->p+__conf->off) >= __conf->end;
}

void static mk_ident(struct token *__tok, char *__s) {
	*__tok = (struct token){.kind=_tok_ident, .p=(void*)__s};
}
void static mk_keyword(struct token *__tok, mdl_u8_t __id) {
	*__tok = (struct token){.kind=_tok_keyword, .id=__id, .p=NULL};
}
void static mk_str(struct token *__tok, char *__s) {
	*__tok = (struct token){.kind=_tok_str, .p=(void*)__s};
}
void static mk_no(struct token *__tok, char *__s) {
	*__tok = (struct token){.kind=_tok_no, .p=(void*)__s};
}
void static mk_chr(struct token *__tok, char *__s) {
	*__tok = (struct token){.kind=_tok_chr, .p=(void*)__s};
}

char static*
read_ident(struct ffly_conf *__conf, char **__end) {
	char *p = (char*)(__conf->p+__conf->off);
	char c = *p;
	while((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || (c >= '0' && c <= '9')) {
		ffly_buff_put(&__conf->sbuf, p++);
		ffly_buff_incr(&__conf->sbuf);
		c = *p;
	}

	char end = '\0';
	ffly_buff_put(&__conf->sbuf, &end);
	char *s = (char*)ffly_str_dupe((char*)ffly_buff_begin(&__conf->sbuf));
	mdl_uint_t l = ffly_buff_off(&__conf->sbuf);
	*__end = s+l;
	__conf->off+= l;
	ffly_buff_off_reset(&__conf->sbuf);
	return s;
}

char static*
read_no(struct ffly_conf *__conf, char **__end) {
	char *p = (char*)(__conf->p+__conf->off);
	char c = *p;
	while((c >= '0' && c <= '9') || c == '.' || c == '-') {
		ffly_buff_put(&__conf->sbuf, p++);
		ffly_buff_incr(&__conf->sbuf);
		c = *p;
	}

	char end = '\0';
	ffly_buff_put(&__conf->sbuf, &end);
	char *s = (char*)ffly_str_dupe((char*)ffly_buff_begin(&__conf->sbuf));
	mdl_uint_t l = ffly_buff_off(&__conf->sbuf);
	*__end = s+l;
	__conf->off+= l;
	ffly_buff_off_reset(&__conf->sbuf);
	return s;
}

char static*
read_str(struct ffly_conf *__conf, char **__end) {
	char *p = (char*)(__conf->p+__conf->off);
	while(*p != '"') {
		ffly_buff_put(&__conf->sbuf, p++);
		ffly_buff_incr(&__conf->sbuf);
	}

	char end = '\0';
	ffly_buff_put(&__conf->sbuf, &end);
	char *s = (char*)ffly_str_dupe((char*)ffly_buff_begin(&__conf->sbuf));
	mdl_uint_t l = ffly_buff_off(&__conf->sbuf);
	*__end = s+l; 
	__conf->off+= l;
	ffly_buff_off_reset(&__conf->sbuf);
	return s;
}

char static*
read_chr(struct ffly_conf *__conf) {
	__conf->off++;
	return (char*)(__conf->p+(__conf->off-1));
}

ffly_err_t static
uread_token(struct ffly_conf *__conf, struct token *__tok) {
	ffly_err_t err;
	ffly_buff_put(&__conf->iject_buff, (void*)&__tok);
	ffly_buff_incr(&__conf->iject_buff);
	return FFLY_SUCCESS;
}	

char static
fetchc(struct ffly_conf *__conf) {
	return *(__conf->p+__conf->off);
}

struct token static*
read_token(struct ffly_conf *__conf, ffly_err_t *__err) {
	ffly_err_t err;
	if (ffly_buff_off(&__conf->iject_buff)>0){
		struct token *tok;
		ffly_buff_decr(&__conf->iject_buff);
		ffly_buff_get(&__conf->iject_buff, (void*)&tok);
		*__err = FFLY_SUCCESS;
		return tok;
	}

	_again:
	while(is_space(fetchc(__conf)) && !is_eof(__conf)) __conf->off++;
	if (fetchc(__conf) == '#' && !is_eof(__conf)) {
		while(fetchc(__conf) != '\n') {
			if (is_eof(__conf)) goto _eof;
			__conf->off++;
		}
		goto _again;
	}

	if (is_eof(__conf)) {
		_eof:
		*__err = FFLY_SUCCESS;
		return NULL;
	}

	struct token *tok;
	if (!(tok = (struct token*)__ffly_mem_alloc(sizeof(struct token)))) {
		//err
	}

	char *end;
	switch(fetchc(__conf)) {
		case '~':
			mk_keyword(tok, _tilde);
			__conf->off++;
		break;
		case '"':
			__conf->off++;
			mk_str(tok, read_str(__conf, &end));
			__conf->off++;
		break;
		case '\x27':
			__conf->off++;
			mk_chr(tok, read_chr(__conf));
			__conf->off++;
		break;
		case ':':
			mk_keyword(tok, _colon);
			__conf->off++;
		break;
		case ',':
			mk_keyword(tok, _comma);
			__conf->off++;
		break;
		case '[':
			mk_keyword(tok, _l_bracket);
			__conf->off++;
		break;
		case ']':
			mk_keyword(tok, _r_bracket);
			__conf->off++;
		break;
		default:
			if ((fetchc(__conf) >= 'a' && fetchc(__conf) <= 'z') || fetchc(__conf) == '_')
				mk_ident(tok, read_ident(__conf, &end));
			else if ((fetchc(__conf) >= '0' && fetchc(__conf) <= '9') || fetchc(__conf) == '-')
				mk_no(tok, read_no(__conf, &end));
			else {
				__ffly_mem_free(tok);
				*__err = FFLY_FAILURE;
				return NULL;
			}
	}

	tok->end = end;
	struct token **p;
	ffly_vec_push_back(&__conf->toks, (void**)&p);
	*p = tok;
	*__err = FFLY_SUCCESS;
	return tok;
}

ffly_bool_t static
next_token_is(struct ffly_conf *__conf, mdl_u8_t __kind, mdl_u8_t __id, ffly_err_t *__err) {
	struct token *tok = read_token(__conf, __err);
	*__err = FFLY_SUCCESS;
	if (tok->kind == __kind && tok->id == __id) return 1;
	*__err = uread_token(__conf, tok);
	return 0;
}

ffly_byte_t static*
read_literal(struct ffly_conf *__conf, mdl_u8_t *__kind, ffly_err_t *__err) {
	struct token *tok = read_token(__conf, __err);
	if (_err(*__err)) {

	}

	*__err = FFLY_SUCCESS;
	if (tok->kind == _tok_no) {
		mdl_u8_t neg = (*(char*)tok->p == '-');
		void *p;
		mdl_u64_t no = ffly_stno(((char*)tok->p)+neg);
		if (neg)
			no = -no; 
		if (next_token_is(__conf, _tok_keyword, _tilde, __err)) {
			struct token *type = read_token(__conf, __err);
			mdl_u8_t sign = *(((char*)type->end)-1) == 's';
			char *p = ((char*)type->p)+1;
			mdl_uint_t size = (char*)type->p-(char*)type->end;
			if (size != 6 && size != 5) {
				// err
			}
			if (*p	== '6' && *(p+1) == '4') goto _64l;
			else if (*p  == '3' && *(p+1) == '2') goto _32l;
			else if (*p  == '1' && *(p+1) == '6') goto _16l;
			else if (*p  == '8') goto _8l;
			else {
				// err
			}
		}

		if (no >= 0 && no <= (mdl_u8_t)~0) {
			_8l:
			*(mdl_u8_t*)(p = __ffly_mem_alloc(sizeof(mdl_u8_t))) = no;
			*__kind = neg?_ffly_conf_8l_s:_ffly_conf_8l_u;
			return (ffly_byte_t*)p;
		} else if (no > (mdl_u8_t)~0 && no <= (mdl_u16_t)~0) {
			_16l:
			*(mdl_u16_t*)(p = __ffly_mem_alloc(sizeof(mdl_u16_t))) = no;
			*__kind = neg?_ffly_conf_16l_s:_ffly_conf_16l_u;
			return (ffly_byte_t*)p;
		} else if (no > (mdl_u16_t)~0 && no <= (mdl_u32_t)~0) {
			_32l:
			*(mdl_u32_t*)(p = __ffly_mem_alloc(sizeof(mdl_u32_t))) = no;
			*__kind = neg?_ffly_conf_32l_s:_ffly_conf_32l_u;
			return (ffly_byte_t*)p;
		} else if (no > (mdl_u32_t)~0 && no <= ~(mdl_u64_t)0) {
			_64l:
			*(mdl_u64_t*)(p = __ffly_mem_alloc(sizeof(mdl_u64_t))) = no;
			*__kind = neg?_ffly_conf_64l_s:_ffly_conf_64l_u;
			return (ffly_byte_t*)p;
		}
		return NULL;
	}

	if (tok->kind == _tok_str)
		*__kind = _ffly_conf_str;
	else if (tok->kind == _tok_chr)
		*__kind = _ffly_conf_chr;
	return (ffly_byte_t*)ffly_str_dupe((char*)tok->p); 
}

ffly_err_t static
read_val(struct ffly_conf *__conf, struct ffly_conf_val *__val) {
	ffly_err_t err = FFLY_SUCCESS;
	__val->p = read_literal(__conf, &__val->kind, &err);
	if (!__val->p) {
		ffly_fprintf(ffly_err, "got null.\n");
		return FFLY_FAILURE;
	}

	push_free(__conf, __val->p);
	if (_err(err)) return err;
	return FFLY_SUCCESS;
}

ffly_err_t static
read_arr(struct ffly_conf *__conf, struct ffly_vec *__data, mdl_uint_t *__l) {
	ffly_err_t err;
	if (next_token_is(__conf, _tok_keyword, _r_bracket, &err)) {
		return FFLY_SUCCESS;
	}

	ffly_vec_set_flags(__data, VEC_AUTO_RESIZE);
	ffly_vec_init(__data, sizeof(void*));
	_again:
	(*__l)++;
	if (next_token_is(__conf, _tok_keyword, _l_bracket, &err)) {
		struct ffly_conf_arr **arr;
		ffly_vec_push_back(__data, (void**)&arr);

		*arr = (struct ffly_conf_arr*)__ffly_mem_alloc(sizeof(struct ffly_conf_arr));
		if (_err(err = read_arr(__conf, &(*arr)->data, &(*arr)->l))) {
			ffly_errmsg("failed to process sub array.\n");
			return err;
		}
		push_arr(__conf, *arr);
		goto _again;
	}

	{
		struct token *tok = peek_token(__conf, &err);
		struct ffly_conf_val **val;
		ffly_vec_push_back(__data, (void**)&val);
		*val = (struct ffly_conf_val*)__ffly_mem_alloc(sizeof(struct ffly_conf_val));
		push_free(__conf, *val);
		if (_err(err = read_val(__conf, *val))) {
			ffly_fprintf(ffly_out, "failed to read value.\n");
			return err;
		}
	}

	if (next_token_is(__conf, _tok_keyword, _comma, &err)) {	
		goto _again;
	}

	if (!next_token_is(__conf, _tok_keyword, _r_bracket, &err)) {
		ffly_errmsg("missing right hand bracket on array.\n");
		return FFLY_FAILURE;
	}
/*
	if (!expect_token(__conf, _tok_keyword, _comma, &err)) {
		ffly_errmsg("expect error.\n");
		return FFLY_FAILURE;
	}
*/
	if (_err(err)) {
		ffly_errmsg("error has occurred while processing array.\n");
		return err;
	}
	return FFLY_SUCCESS;
}

ffly_err_t static
read_decl(struct ffly_conf *__conf) {
	ffly_err_t err;
	struct token *name = read_token(__conf, &err);
	if (_err(err)) return FFLY_FAILURE;
	if (!expect_token(__conf, _tok_keyword, _colon, &err)) {
		if (_err(err))
			return err;
		ffly_errmsg("missing colon separator.\n");
		return FFLY_FAILURE;
	}

	void *p;	
	if (next_token_is(__conf, _tok_keyword, _l_bracket, &err)) {
		struct ffly_conf_arr *arr = (struct ffly_conf_arr*)__ffly_mem_alloc(sizeof(struct ffly_conf_arr));
		if (_err(err = read_arr(__conf, &arr->data, &arr->l))) {
			ffly_fprintf(ffly_err, "failed to read array.\n");
			__ffly_mem_free(arr);
			return err;
		}

		arr->name = (char*)name->p;
		p = (void*)arr;
		push_arr(__conf, arr);
	} else {
		struct ffly_conf_val val;
		if (_err(err = read_val(__conf, &val))) {
			ffly_fprintf(ffly_err, "failed to read value.\n");
			return err;
		}

		struct ffly_conf_var *var = (struct ffly_conf_var*)__ffly_mem_alloc(sizeof(struct ffly_conf_var));
		*var = (struct ffly_conf_var) {
			.val = val,
			.name = (char*)name->p
		};

		p = (void*)var;
		push_free(__conf, p);
	}

	ffly_map_put(&__conf->env, (mdl_u8_t*)name->p, ffly_str_len((char*)name->p), p);
	if (!expect_token(__conf, _tok_keyword, _comma, &err)) {
		ffly_errmsg("expect error.\n");
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}

ffly_err_t
ffly_conf_init(struct ffly_conf *__conf) {
	ffly_err_t err;
	if (_err(err = ffly_buff_init(&__conf->sbuf, 56, 1))) {
		ffly_fprintf(ffly_err, "failed to init buff.\n");
		return err;
	}

	ffly_vec_set_flags(&__conf->toks, VEC_AUTO_RESIZE);
	if (_err(err = ffly_vec_init(&__conf->toks, sizeof(struct token*)))) {
		ffly_fprintf(ffly_err, "failed to init vec.\n");
		return err;
	}

	if (_err(err = ffly_buff_init(&__conf->iject_buff, 8, sizeof(struct token*)))) {
		ffly_fprintf(ffly_err, "failed to init buff.\n");
		return err;
	}

	ffly_vec_set_flags(&__conf->arrs, VEC_AUTO_RESIZE);
	if (_err(err = ffly_vec_init(&__conf->arrs, sizeof(struct ffly_conf_arr*)))) {
		ffly_fprintf(ffly_err, "failed to init vec.\n");
		return err;
	}

	ffly_vec_set_flags(&__conf->free, VEC_AUTO_RESIZE);
	if (_err(err = ffly_vec_init(&__conf->free, sizeof(void*)))) {
		ffly_fprintf(ffly_err, "failed to init vec.\n");
		return err;
	}

	if (_err(err = ffly_map_init(&__conf->env, _ffly_map_127))) {
		ffly_fprintf(ffly_err, "failed to init map.\n");
		return err;
	}
	return FFLY_SUCCESS;
}

struct token static*
peek_token(struct ffly_conf *__conf, ffly_err_t *__err) {
	struct token *tok = read_token(__conf, __err);
	if (_err(*__err)) return NULL;
	*__err = uread_token(__conf, tok);
	return tok;
}


// rename to ffly_conf_prossess
ffly_err_t
ffly_conf_read(struct ffly_conf *__conf) {
	ffly_err_t err;
	struct token *tok;
	while(!is_eof(__conf)) {
		tok = peek_token(__conf, &err);
		if (!tok) break;

		if (_err(err)) {
			ffly_fprintf(ffly_err, "failed to peek token.\n");
			return FFLY_FAILURE;
		}
		if (tok->kind == _tok_ident) {
			if (_err(err = read_decl(__conf))) {
				ffly_fprintf(ffly_err, "failed to read decl.\n");
			}
		} else
			break;
		if (_err(err))
			return err;
	}
	return FFLY_SUCCESS;
}

# include "dep/mem_set.h"
ffly_err_t
ffly_conf_ld(struct ffly_conf *__conf, char const *__file) {
	ffly_err_t err;
	FF_FILE *f = ffly_fopen(__file, FF_O_RDONLY, 0, &err);
	struct ffly_stat st;
	ffly_fstat(__file, &st);

	__conf->p = (ffly_byte_t*)__ffly_mem_alloc(st.size);
	ffly_fread(f, __conf->p, st.size);
	__conf->end = __conf->p+st.size;
	ffly_fclose(f);
	__conf->off = 0;
	return FFLY_SUCCESS; 
}

ffly_err_t
ffconf_free(ffconfp __conf) {
	ffly_err_t err;
	if (ffly_vec_size(&__conf->arrs)>0) {
		struct ffly_conf_arr **itr = (struct ffly_conf_arr**)ffly_vec_begin(&__conf->arrs);
		while(itr <= (struct ffly_conf_arr**)ffly_vec_end(&__conf->arrs)) {
			ffly_vec_de_init(&(*itr)->data);
			__ffly_mem_free(*itr);
			itr++;
		}
	}

	if (_err(err = ffly_vec_de_init(&__conf->arrs))) {
		ffly_fprintf(ffly_err, "failed to de-init vec.\n");
		return err;
	}

	if (ffly_vec_size(&__conf->free)>0) {
		void **itr = (void**)ffly_vec_begin(&__conf->free);
		while(itr <= (void**)ffly_vec_end(&__conf->free))
			__ffly_mem_free(*(itr++));
	}

	if (_err(err = ffly_vec_de_init(&__conf->free))) {
		ffly_fprintf(ffly_err, "failed to de-init vec.\n");
		return err;
	}

	if (_err(err = ffly_map_de_init(&__conf->env))) {
		ffly_fprintf(ffly_err, "failed to de-init map.\n");
		return err;
	}
	return FFLY_SUCCESS;
}

ffly_err_t
ffly_conf_free(struct ffly_conf *__conf) {
	ffly_err_t err;
	if (_err(err = ffly_buff_de_init(&__conf->sbuf))) {
		ffly_fprintf(ffly_err, "failed to de-init buff.\n");
		return err;
	}

	if (ffly_vec_size(&__conf->toks)>0) {
		struct token *tok;
		struct token **itr = (struct token**)ffly_vec_begin(&__conf->toks);
		while(itr <= (struct token**)ffly_vec_end(&__conf->toks)) {
			tok = *(itr++);
			if (tok->p != NULL) {
				if ((mdl_u8_t*)tok->p > __conf->end || (mdl_u8_t*)tok->p < __conf->p)
					__ffly_mem_free(tok->p);
			}
			__ffly_mem_free(tok);
		}
	}

	if (_err(err = ffly_vec_de_init(&__conf->toks))) {
		ffly_fprintf(ffly_err, "failed to de-init vec.\n");
		return err;
	}

	if (_err(err = ffly_buff_de_init(&__conf->iject_buff))) {
		ffly_fprintf(ffly_err, "failed to de-init buff.\n");
		return err;
	}

	if (__conf->p != NULL)
		__ffly_mem_free(__conf->p);
	return FFLY_SUCCESS;
}

mdl_u64_t
ffly_conf_int_u(void const *__val) {
	struct ffly_conf_val *val = (struct ffly_conf_val*)__val;
	switch(val->kind) {
		case _ffly_conf_64l_u:
			return *(mdl_u64_t*)val->p;
		case _ffly_conf_32l_u:
			return *(mdl_u32_t*)val->p;
		case _ffly_conf_16l_u:
			return *(mdl_u16_t*)val->p;
		case _ffly_conf_8l_u: 
			return *(mdl_u8_t*)val->p;
	}
	return 0;
}

mdl_i64_t
ffly_conf_int_s(void const *__val) {
	struct ffly_conf_val *val = (struct ffly_conf_val*)__val;
	switch(val->kind) {
		case _ffly_conf_64l_s:
			return *(mdl_i64_t*)val->p;
		case _ffly_conf_32l_s:
			return *(mdl_i32_t*)val->p;
		case _ffly_conf_16l_s:
			return *(mdl_i16_t*)val->p;
		case _ffly_conf_8l_s:
			return *(mdl_i8_t*)val->p;
	}
	return 0;
}

void static print_val(struct ffly_conf_val *__val) {
	if (__val->kind == _ffly_conf_str)
		ffly_printf("%s\n", (char*)__val->p);
	else if (__val->kind == _ffly_conf_chr)
		ffly_printf("%c\n", *(char*)__val->p);
	else if (__val->kind == _ffly_conf_64l_u)
		ffly_printf("%lu\n", *(mdl_u64_t*)__val->p);
	else if (__val->kind == _ffly_conf_32l_u)
		ffly_printf("%u\n", *(mdl_u32_t*)__val->p);
	else if (__val->kind == _ffly_conf_16l_u)
		ffly_printf("%u\n", *(mdl_u16_t*)__val->p);
	else if (__val->kind == _ffly_conf_8l_u)
		ffly_printf("%u\n", *(mdl_u8_t*)__val->p);
	else if (__val->kind == _ffly_conf_64l_s)
		ffly_printf("%ld\n", *(mdl_i64_t*)__val->p);
	else if (__val->kind == _ffly_conf_32l_s)
		ffly_printf("%d\n", *(mdl_i32_t*)__val->p);
	else if (__val->kind == _ffly_conf_16l_s)
		ffly_printf("%d\n", *(mdl_i16_t*)__val->p);
	else if (__val->kind == _ffly_conf_8l_s)
		ffly_printf("%d\n", *(mdl_i8_t*)__val->p);
}

//# define DEBUG
# ifdef DEBUG
int main() {
	ffly_io_init();
	struct ffly_conf conf;
	ffly_conf_init(&conf);
	ffly_conf_ld(&conf, "test.conf");
	ffly_conf_read(&conf);
	ffconf cf;
	ffly_conf_depos(&conf, &cf);

	void const *info = ffly_conf_get(&cf, "info");
	if (!info) {
		ffly_fprintf(ffly_out, "(null)\n");
		return -1;
	}

	void const *no = ffly_conf_get(&cf, "no");

	printf("val: %u\n", ffly_conf_int_u(no));

	print_val((struct ffly_conf_val*)info);
//	  print_val((struct ffly_conf_val*)no);
//	  void *in = ffly_conf_get_arr_elem(&conf, (void*)arr, 0);
//	  print_val((struct ffly_conf_val*)ffly_conf_get_arr_elem(&conf, in, 0));
 //   print_val((struct ffly_conf_val*)ffly_conf_get_arr_elem(&conf, arr, 1));
  //  print_val((struct ffly_conf_val*)ffly_conf_get_arr_elem(&conf, arr, 2));
   // print_val((struct ffly_conf_val*)ffly_conf_get_arr_elem(&conf, arr, 3));

	ffly_conf_free(&conf);
	ffconf_free(&cf);	 
	ffly_io_closeup();
}
# endif
