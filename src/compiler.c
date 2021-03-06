# define __ffly_compiler_internal
# include "compiler.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/io.h"
# include "system/file.h"
# include "dep/str_cmp.h"
# include "dep/mem_cpy.h"
# include "dep/mem_set.h"
# include "dep/str_len.h"
# include "dep/str_dup.h"
# include "system/errno.h"
# include "dep/bcopy.h"
# include "system/nanosleep.h"
# include "dep/str_cpy.h"
# include "system/realpath.h"
# include "linux/unistd.h"
# ifndef __ffc_no_script
ff_err_t ffc_script_final(struct ffly_compiler*, void**, ff_byte_t**, void(*)(void*, char const*, ff_u8_t));
ff_err_t ffc_script_build(struct ffly_compiler*, void**, ff_byte_t**, void(*)(void*, char const*, ff_u8_t));
ff_err_t ffly_script_parse(struct ffly_compiler*);
ff_err_t ffly_script_gen(struct ffly_compiler*, void**, ff_byte_t**, void(*)(void*, char const*, ff_u8_t));
# endif
# ifndef __ffc_no_ff
ff_err_t ffc_ff_final(struct ffly_compiler*);
ff_err_t ffc_ff_build(struct ffly_compiler*);
ff_err_t ffly_ff_parse(struct ffly_compiler*);
ff_err_t ffly_ff_gen(struct ffly_compiler*);
# endif
void *_ffc_final;
void *_ffc_build;
void *_ffly_parse;
void *_ffly_gen;
# ifndef __ffc_no_ff
void static
ld_ff(ffly_compilerp __compiler) {
	_ffc_final = ffc_ff_final;
	_ffc_build = ffc_ff_build;
	_ffly_parse = ffly_ff_parse;
	_ffly_gen = ffly_ff_gen;
}
# endif
# ifndef __ffc_no_script
void static
ld_script(ffly_compilerp __compiler) {
	_ffc_final = ffc_script_final;
	_ffc_build = ffc_script_build;
	_ffly_parse = ffly_script_parse;
	_ffly_gen = ffly_script_gen;
}
# endif
void
ffc_ldlang(ffly_compilerp __compiler, ff_u8_t __no) {
	switch(__no) {
# ifndef __ffc_no_ff
		case _ffc_ff:
			ld_ff(__compiler);
		break;
# endif
# ifndef __ffc_no_script
		case _ffc_script:
			ld_script(__compiler);
		break;
# endif
	}

	__compiler->lang = __no;
}

char static*
getdir(char *__buf, char const *__file) {
	char *buf = __buf;
	char *end = buf;
	while(*end != '\0') end++;

	char const *p = __file;
	if (*p == '.') {
	_again:
		while(*p != '/') {
			if (*p == '.') {
				if (*(++p) == '.') {
					while(*(--end) != '/');
				}
				p++;
			}
		}

		if (*(p+1) == '.') {
			p++;
			goto _again;
		}
		p++;
	}

	char *br;
_bk:
	*(br = end++) = '/';
	while(*p != '\0') {
		if (*p == '/') {
			p++;
			goto _bk;
		}
		*(end++) = *(p++);
	}

	*br ='\0';
	return br;
}

void static
getpath(char *__dir, char *__buf, char const *__file) {
	char *p = __buf;
	ffly_str_cpy(p, __dir);
	p=getdir(p, __file);

	char const *br = __file;
	while(*__file != '\0') {
		if (*__file == '/')
			br = __file+1;
		__file++;
	}

	*(p++) = '/';
	ffly_str_cpy(p, br);
}

void static
_get(ff_uint_t __from, ff_uint_t __size, void *__buf, void *__arg) {
	FF_FILE *f = (FF_FILE*)__arg;
	ffly_fpread(f, __buf, __size, __from);
}

ff_err_t
ffly_compiler_ld(struct ffly_compiler *__compiler, char const *__file) {
	ff_err_t err;
	char path[PATH_MAX];
	getpath(__compiler->dir, path, __file);

	ffly_printf("dir: %s\n", __compiler->dir);
	ffly_printf("path: %s\n", path);
	if (access(path, F_OK) == -1) {
		errmsg("file doesen't exist or access to it has been denied.\n");
		reterr;
	}

	FF_FILE *f = ffly_fopen(path, FF_O_RDONLY, 0, &err);
	getdir(__compiler->dir, __file);
	ffly_printf("dir: %s\n", __compiler->dir);
	__compiler->file->path = ffly_str_dup(path);

	struct ffly_stat st;
	ffly_fstat(path, &st);

	ffly_compiler_filep file = __compiler->file;
	ff_lexerp lexer = &__compiler->lexer;

	file->f = f;
	lexer->arg = (void*)f;
	lexer->off = 0;
	lexer->end = st.size;
	lexer->line = &file->line;
	lexer->lo = &file->lo;
	lexer->get = _get;
	return FFLY_SUCCESS;
}
/*
void set_call(struct ffly_compiler *__compiler, void *__p, ff_u8_t __no) {
	__compiler->call[__no] = __p;	
}

void __parser_func_call();
void __parser_decl_spec();

void static *jmp[] = {
	(void*)__parser_func_call,
	(void*)__parser_decl_spec
};

ff_u64_t _ringup(struct ffly_compiler *__compiler, ff_u8_t __no, ...) {
	__asm__("jmp *%0\n\t" : : ""(jmp[__no]));
	__asm__("__parser_func_call:\n\t");
	{
		struct ffly_compiler *p0;
		struct node **p1;
		struct node *p2;
		va_list args;
		va_start(args, __no);
		p0 = va_arg(args, struct ffly_compiler*);
		p1 = va_arg(args, struct node**);
		p2 = va_arg(args, struct node*);
		va_end(args);

	}
}
*/
// not the best way but works
char const* tokk_str(ff_u8_t __kind) {
	switch(__kind) {
		case _tok_ident: return "ident";
		case _tok_keywd: return "keyword";
		case _tok_no: return "number";
		case _tok_newline: return "newline";
	}
	return "unknown";
}

char const* tokid_str(ff_u8_t __id) {
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
		case _l_bracket: return "left bracket";
		case _r_bracket: return "right bracket";
	}
	return "unknown";
}

void pr_tok(struct token *__tok) {
	if (!__tok) return;
	ffly_printf("tok kind: %s\n", tokk_str(__tok->kind));
	ffly_printf("tok id: %s\n", tokid_str(__tok->id));
}

void static _vec(void *__arg) {
	ffly_vec_de_init((ffly_vecp)__arg);
}

void static _map(void *__arg) {
	ffly_map_de_init((ffly_mapp)__arg);
}

void cu_task(struct ffly_compiler *__compiler, void *__p, void(*__func)(void*)) {
	struct cleanup_task **task;
	ffly_vec_push_back(&__compiler->clean, (void**)&task);
	*(*task = (cleanup_taskp)__ffly_mem_alloc(sizeof(struct cleanup_task))) = (struct cleanup_task){
		.func = __func,
		.p = __p
	};
}

void vec_cleanup(struct ffly_compiler *__compiler, struct ffly_vec *__vec) {
	cu_task(__compiler, __vec, _vec);
}

void map_cleanup(struct ffly_compiler *__compiler, struct ffly_map *__map) {
	cu_task(__compiler, __map, _map);
}

void cleanup(struct ffly_compiler *__compiler, void *__p) {
	void **p;	
	ffly_vec_push_back(&__compiler->to_free, (void**)&p);
	*p = __p;
}

ff_bool_t is_keyword(struct token *__tok, ff_u8_t __id) {
	return (__tok->kind == _tok_keywd && __tok->id == __id);
}

ff_off_t toklo(struct token *__tok) {
	return __tok->lo;
}

ff_off_t curlo(struct ffly_compiler *__compiler) {
	return __compiler->file->lo;
}

ff_bool_t next_token_is(struct ffly_compiler *__compiler, ff_u8_t __kind, ff_u8_t __id) {
	struct token *tok = next_token(__compiler);
	if (!tok) return 0;
	if (tok->kind == __kind && tok->id == __id) {
		ff_token_free(tok);
		return 1;
	}
	ffly_ulex(&__compiler->lexer, tok);
	return 0;
}

ff_uint_t tokcol(struct token *__tok) {
	return __tok->off-toklo(__tok);
}

void print_line(struct ffly_compiler *__compiler, ff_uint_t __off) {
	ff_u32_t off = __off;
	char c;

	_get(off, 1, &c, __compiler->file->f); 
	while(c != '\n' && c != '\0') {
		if (c != '\t')
			ffly_printf("%c", c);
		_get(++off, 1, &c, __compiler->file->f);
	}
	ffly_printf("\n");
}

# define expectmsg(__s) \
	ffly_fprintf(ffly_out, "%s:%u;%u: %s, got %s.\n", __compiler->file->path, tokl(tok)+1, tokcol(tok), __s, tokid_str(tok->id))
ff_bool_t expect_token(struct ffly_compiler *__compiler, ff_u8_t __kind, ff_u8_t __id) {
	struct token *tok = next_token(__compiler);
	if (!tok) {
		errmsg("token is null.\n");
		return 0;
	}

	ff_u8_t ret;
	if (!(ret = (tok->kind == __kind && tok->id == __id))) {
		if (__kind == _tok_keywd) {
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
			print_line(__compiler, tok->lo+1);
			ff_uint_t pad = tok->off-toklo(tok);
			while(pad != 0) {
				ffly_printf(" ");
				pad--;
			}

			ffly_printf("^\n");
		}
	}
	ff_token_free(tok);
	return ret;
}

# include "system/util/hash.h"
void put_keyword(struct ffly_compiler *__compiler, char const *__key, ff_u8_t __id) {
	ff_u64_t val = ffly_hash((ff_u8_t const*)__key, ffly_str_len(__key));

	keywdp p = (keywdp)__ffly_mem_alloc(sizeof(struct keywd));
	p->id = __id;
	p->next = (keywdp)ffly_lat_get(&__compiler->keywd, val);
	ffly_lat_put(&__compiler->keywd, val, (void*)p);
	ffly_str_cpy(p->key, __key);
}

keywdp get_keyword(struct ffly_compiler *__compiler, char const *__key) {
	ff_u64_t val = ffly_hash(__key, ffly_str_len(__key));
	keywdp cur = (keywdp)ffly_lat_get(&__compiler->keywd, val);
	while(!null(cur)) {
		if (!ffly_str_cmp(cur->key, __key)) return cur;
		cur = cur->next;
	}
	return NULL;
}

ff_err_t ffly_compiler_free(struct ffly_compiler *__compiler) {
	FF_ERR err;
	void *cur;

	cur = ffly_lat_head(&__compiler->keywd);
	while(cur != NULL) {
		__ffly_mem_free(ffly_lat_getp(cur));	
		ffly_lat_fd(&cur);
	}

	ffly_lat_free(&__compiler->keywd);

	ffly_lexer_cleanup(&__compiler->lexer);
	ffly_parser_cleanup();

	cleanup_taskp *task;
	___ffly_vec_nonempty(&__compiler->clean) {
		task = (cleanup_taskp*)ffly_vec_begin(&__compiler->clean);
		while(!vec_deadstop(task, &__compiler->clean)) {
			cleanup_taskp t = *task;
			t->func(t->p);
			__ffly_mem_free(t);
			task++;
		}
	}

	if (_err(err = ffly_vec_de_init(&__compiler->clean))) {
		errmsg("failed to de-init vector.\n");
	}

	if (_err(err = ffly_map_de_init(&__compiler->env))) {
		errmsg("failed to de-init map.\n");
	}

	if (_err(err = ffly_vec_de_init(&__compiler->nodes))) {
		errmsg("failed to de-init vector.\n");
	}

	void **p;
	___ffly_vec_nonempty(&__compiler->to_free) {
		p = (void**)ffly_vec_begin(&__compiler->to_free);
		while(!vec_deadstop(p, &__compiler->to_free))
			__ffly_mem_free(*(p++));
	}

	if (_err(err = ffly_vec_de_init(&__compiler->to_free))) {
		errmsg("failed to de-init vector.\n");
	}

	if (__compiler->file->f != NULL) {
		ffly_fclose(__compiler->file->f);
	}

	if (_err(err = ffly_map_de_init(&__compiler->macros))) {
		errmsg("failed to de-init map.\n");
	}

	if (_err(err = ffly_map_de_init(&__compiler->typedefs))) {
		errmsg("failed to de-init map.\n");
	}

	if (__compiler->file->path != NULL)
		__ffly_mem_free(__compiler->file->path);
	__ffly_mem_free(__compiler->file);
	retok;
}

ff_tokenp
peek_token(ffly_compilerp __compiler) {
	ff_tokenp tok;
	if (!(tok = next_token(__compiler))) {
		// error
		return NULL;
	}
	ffly_ulex(&__compiler->lexer, tok);
	return tok;
}


/*
	is next token a new line
*/
ff_bool_t
next_tok_nl(ffly_compilerp __compiler) {
	ff_err_t err;
	ff_tokenp tok;
	tok = ffly_lex(&__compiler->lexer, &err);

	if (tok != NULL) {
		if (tok->kind == _tok_newline) {
			ff_token_free(tok);
			return 1;
		}
		ffly_ulex(&__compiler->lexer, tok);
	}
	return 0;
}

ff_err_t static
read_define(ffly_compilerp __compiler) {
	ff_tokenp name;	
	if (!(name = next_token(__compiler))) {
		// error
	}

	void *p = NULL;
	if (!next_tok_nl(__compiler)) {
		struct ffly_vec *toks;
		toks = (ffly_vecp)__ffly_mem_alloc(sizeof(struct ffly_vec));

		ffly_vec_set_flags(toks, VEC_AUTO_RESIZE);
		ffly_vec_init(toks, sizeof(ff_tokenp));
		p = toks;
		ff_tokenp *tok;
	_next:
		ffly_vec_push_back(toks, (void**)&tok);
		if (!(*tok = next_token(__compiler))) {
			// error
		}
 
		if (!next_tok_nl(__compiler))
			goto _next;
		vec_cleanup(__compiler, toks);
		cleanup(__compiler, toks);
	}/* else {

	}*/

	ffly_map_put(&__compiler->macros, name->p, ffly_str_len((char*)name->p), p);
	retok;
}

ff_bool_t
is_endif(ffly_compilerp __compiler, ff_tokenp __tok) {
	if (is_keyword(__tok, _percent)) {
		ff_err_t err;
		ff_tokenp tok;		
		if (!(tok = ffly_lex(&__compiler->lexer, &err))) {
			// error
		}

		ff_u8_t res;
		if (!(res = !ffly_str_cmp(tok->p, "endif")))
			ffly_ulex(&__compiler->lexer, tok);
		return res;
	}
	return 0;
}

ff_bool_t
is_else(ffly_compilerp __compiler, ff_tokenp __tok) {
	if (is_keyword(__tok, _percent)) {
		ff_err_t err;
		ff_tokenp tok;
		if (!(tok = ffly_lex(&__compiler->lexer, &err))) {
			// error
		}
		ff_u8_t res;
		if (!(res = !ffly_str_cmp(tok->p, "else")))
			ffly_ulex(&__compiler->lexer, tok);
		return res;
	}
	return 0;
}


/*
	shoud just pass function pointer to is_***
*/
void static
sk_to_else(struct ffly_compiler *__compiler) {
	ff_err_t err;
	struct token *tok = NULL;
	for(;;) {
		if (!(tok = ffly_lex(&__compiler->lexer, &err))) break;
		if (_err(err)) {
			errmsg("lexer failure.\n");
			break;
		}
		if (is_else(__compiler, tok)) break;
		if (tok->kind == _tok_newline)
			ff_token_free(tok);
	}
}

void static
sk_to_endif(struct ffly_compiler *__compiler) {
	ff_err_t err;
	ff_tokenp tok = NULL;
	for(;;) {
		if (!(tok = ffly_lex(&__compiler->lexer, &err))) break;
		if (_err(err)) {
			errmsg("lexer failure.\n");
			break;
		}
		if (is_endif(__compiler, tok)) break;
		if (tok->kind == _tok_newline)
			ff_token_free(tok);
	}
}

ff_err_t static
read_ifdef(ffly_compilerp __compiler) {
	ff_tokenp name;
	ff_err_t err;
_again:
	if (!(name = next_token(__compiler))) {
		// error
	}

	if (!is_tokident(name)) {
		// error
	}

	ffly_map_get(&__compiler->macros, name->p, ffly_str_len((char*)name->p), &err);
	if (_err(err)) {
		sk_to_else(__compiler);
		retok;
	}

	if (next_token_is(__compiler, _tok_keywd, _and))
		goto _again;
	retok;
}

ff_err_t static
read_ifndef(ffly_compilerp __compiler) {
	ff_tokenp name;
	ff_err_t err;
_again:
	if (!(name = next_token(__compiler))) {
		// error
	}

	if (!is_tokident(name)) {
		// error
	}

	ffly_map_get(&__compiler->macros, name->p, ffly_str_len((char*)name->p), &err);
	if (_ok(err)) {
		sk_to_else(__compiler);
		retok;
	}

	if (next_token_is(__compiler, _tok_keywd, _and))
		goto _again;
	retok;
}

ff_err_t static
read_include(ffly_compilerp __compiler) {
	ff_tokenp path;
	if (!(path = next_token(__compiler))) {
		// error
	}

	if (!is_tokstr(path)) {
		// error
	}

	ffly_printf("include: %s\n", (char*)path->p);

	ff_lexerp lexer;
	
	lexer = &__compiler->lexer;

	ff_u32_t off, end;
	off = lexer->off;
	end = lexer->end;

	ffly_compiler_filep file;
	file = ++__compiler->file;

	ffly_str_cpy(file->dir, (file-1)->dir);

	__compiler->dir = file->dir;
	file->line = 0;
	file->lo = 0;

	/*
		get rid loaded chunk if not it will read the wrong data
	*/
	ff_lexer_reset(lexer);
	if (_err(ffly_compiler_ld(__compiler, (char*)path->p))) {
		ffly_printf("failed to load.\n");
		goto _sk;
	}
	switch(__compiler->lang) {
# ifndef __ffc_no_ff
		case _ffc_ff:
			ffly_ff_parse(__compiler);
		break;
# endif
# ifndef __ffc_no_script
		case _ffc_script:
			ffly_script_parse(__compiler);
		break;
# endif
		default:
			// error
			break;
	}

	__ffly_mem_free(file->path);
	ffly_fclose(file->f);
_sk:
	file = --__compiler->file;
	__compiler->dir = file->dir;

	lexer->arg = file->f;
	lexer->off = off;
	lexer->end = end;
	lexer->line = &file->line;
	lexer->lo = &file->lo;
	retok;
}

void read_macro(struct ffly_compiler *__compiler) {
	ff_err_t err;
	struct token *tok = ffly_lex(&__compiler->lexer, &err);
	if (tok->kind != _tok_ident) {
		ffly_printf("what is this? need ident not this.\n");
		return;
	}

	if (!ffly_str_cmp(tok->p, "define")) {
		if (_err(err = read_define(__compiler))) {
			ffly_printf("failed to read 'define'.\n");
		}
	} else if (!ffly_str_cmp(tok->p, "ifdef")) {
		if (_err(err = read_ifdef(__compiler))) {
			ffly_printf("failed to read 'ifdef'.\n");
		}
	} else if (!ffly_str_cmp(tok->p, "ifndef")) {
	 	if (_err(err = read_ifndef(__compiler))) {
			ffly_printf("failed to read 'ifndef'.\n");
		}
	} else if (!ffly_str_cmp(tok->p, "include")) {
		if (_err(err = read_include(__compiler))) {
			ffly_printf("failed to read 'include'.\n");
		}
	} else if (!ffly_str_cmp(tok->p, "else"))
		sk_to_endif(__compiler);
	else {
		if (ffly_str_cmp(tok->p, "endif") == -1)
			ffly_printf("unknown macro: %s\n", tok->p);
	}
}

ff_tokenp
next_token(ffly_compilerp __compiler) {
	ff_err_t err;
	ff_tokenp tok;
_bk:
	if (!(tok = ffly_lex(&__compiler->lexer, &err))) {
		ffly_printf("got null token.\n");
		return NULL;
	}
	if (tok->kind == _tok_newline) {
		ff_token_free(tok);
		goto _bk;
	}

	if (is_keyword(tok, _percent)) {
		read_macro(__compiler);
		goto _bk;
	}

	if (tok->kind == _tok_ident) {
		struct ffly_vec *toks = (struct ffly_vec*)ffly_map_get(&__compiler->macros, tok->p, ffly_str_len((char*)tok->p), &err);
		if (toks != NULL && _ok(err)) {
			struct token **p = (struct token**)ffly_vec_end(toks);
			while(p >= (struct token**)ffly_vec_begin(toks))
				ffly_ulex(&__compiler->lexer, *(p--)); 
			tok = next_token(__compiler);
		}
	}

	maybe_keyword(__compiler, tok);
	if (!tok) {
		ffly_printf("null token being returned.\n");
	}
	return tok;
}

ff_uint_t tokl(struct token *__tok) {
	return __tok->line;
}

void to_keyword(struct token *__tok, ff_u8_t __id) {
	__tok->kind = _tok_keywd;
	__tok->id = __id;
}

ff_bool_t maybe_keyword(struct ffly_compiler *__compiler, struct token *__tok) {
	if (!__tok) return 0;
	if (__tok->kind != _tok_ident || __tok->p == NULL) return 0;
	keywdp p;	
	if (!(p = get_keyword(__compiler, __tok->p)))	
		return 0;
	to_keyword(__tok, p->id);
	return 1;
}

char const static *keywords[] = {
	"print",
	"if",
	"uint_t",
	"int_t",
	"u64_t",
	"i64_t",
	"u32_t",
	"i32_t",
	"u16_t",
	"i16_t",
	"u8_t",
	"i8_t",
	"fn",
	"extern",
	"struct",
	"void",
	"var",
	"exit",
	"while",
	"match",
	"else",
	"float",
	"typedef",
	"ret",
	"brk",
	"as",
	"out",
	"goto",
	"SYPUT",
	"va_args",
	"sizeof",
	"no_va",
	"no_params",
	"no_ret",
	NULL
};

ff_u8_t static keyword_ids[] = {
	_k_print,
	_k_if,
	_k_uint_t,
	_k_int_t,
	_k_u64_t,
	_k_i64_t,
	_k_u32_t,
	_k_i32_t,
	_k_u16_t,
	_k_i16_t,
	_k_u8_t,
	_k_i8_t,
	_k_fn,
	_k_extern,
	_k_struct,
	_k_void,
	_k_var,
	_k_exit,
	_k_while,
	_k_match,
	_k_else,
	_k_float,
	_k_typedef,
	_k_ret,
	_k_brk,
	_k_as,
	_k_out,
	_k_goto,
	_k_syput,
	_k_va_args,
	_k_sizeof,
	_k_no_va,
	_k_no_params,
	_k_no_ret,
	0
};

ff_u8_t ffly_compiler_kwno(char const *__keyword) {
	char const **s = keywords;
	ff_u8_t no = 0;
	while(*s != NULL) {
		if (!ffly_str_cmp(*s, __keyword))
			return no;
		no++;
		s++;
	}
	return no;
}

void ffly_compiler_ldkeywd(struct ffly_compiler *__compiler, ff_u8_t __no) {
	put_keyword(__compiler, keywords[__no], keyword_ids[__no]);
}

ff_err_t parser_init(struct ffly_compiler*);
ff_err_t ffly_compiler_prepare(struct ffly_compiler *__compiler) {
	FF_ERR err;
//	parser_init(__compiler);

	ffly_lat_prepare(&__compiler->keywd);

	ffly_lexer_init(&__compiler->lexer);

	if (!(__compiler->file = (struct ffly_compiler_file*)__ffly_mem_alloc(12*sizeof(struct ffly_compiler_file)))) {
		errmsg("memory allocation failure.\n");
		reterr;
	}

	ffly_vec_set_flags(&__compiler->to_free, VEC_AUTO_RESIZE);
	if (_err(err = ffly_vec_init(&__compiler->to_free, sizeof(void*)))) {
		errmsg("failed to initialize vector.\n");
		_ret;
	}

	ffly_vec_set_flags(&__compiler->clean, VEC_AUTO_RESIZE);
	if (_err(err = ffly_vec_init(&__compiler->clean, sizeof(cleanup_taskp)))) {
		errmsg("failed to initialize vector.\n");
		_ret;
	}

	ffly_vec_set_flags(&__compiler->nodes, VEC_AUTO_RESIZE);
	if (_err(err = ffly_vec_init(&__compiler->nodes, sizeof(struct node*)))) {
		errmsg("failed to initialize vector.\n");
		_ret;
	}

	if (_err(err = ffly_map_init(&__compiler->env, _ffly_map_127))) {
		errmsg("failed to initialize map.\n");
		_ret;
	}

	if (_err(err = ffly_map_init(&__compiler->typedefs, _ffly_map_127))) {
		errmsg("failed to initialize map.\n");
		_ret;
	}
	__compiler->brkp = __compiler->brk;
	__compiler->file->line = 0;
	__compiler->file->lo = 0;
	__compiler->local = NULL;
	__compiler->file->f = NULL;
	__compiler->file->path = NULL;
	if (_err(err = ffly_map_init(&__compiler->macros, _ffly_map_127))) {
		errmsg("failed to initialize map.\n");
		_ret;
	}
	__compiler->var_pond = NULL;
	__compiler->ret_type = NULL;
	getcwd(__compiler->file->dir, PATH_MAX);
	__compiler->dir = __compiler->file->dir;
	retok;
}

# ifndef __ffc_no_script
ff_err_t ffc_script_final(struct ffly_compiler *__compiler, void **__top, ff_byte_t **__stack, void(*__syput)(void*, char const*, ff_u8_t)) {
	ff_err_t err;
	if (_err(err = ffly_script_gen(__compiler, __top, __stack, __syput))) {
		errmsg("an error has occurred in the generative process.\n");
		_ret;
	}
	retok;
}

ff_err_t ffc_script_build(struct ffly_compiler *__compiler, void ** __top, ff_byte_t **__stack, void(*__syput)(void*, char const*, ff_u8_t)) {
	ff_err_t err;
	if (_err(err = ffly_script_parse(__compiler))) {
		errmsg("an error has occurred in the parsing process.\n");
		_ret;
	}

	if (_err(err = ffc_script_final(__compiler, __top, __stack, __syput))) {
		errmsg("failed to finalize.\n");
		_ret;
	}
	ffly_fprintf(ffly_out, "build successful.\n");
	retok;
}
# endif

# ifndef __ffc_no_ff
ff_err_t ffc_ff_final(struct ffly_compiler *__compiler) {
	ffly_ff_gen(__compiler);
}

ff_err_t ffc_ff_build(struct ffly_compiler *__compiler) {
	ffly_ff_parse(__compiler);
	ffc_ff_final(__compiler);
}
# endif

ff_uint_t curl(struct ffly_compiler *__compiler) {
	return __compiler->file->line;
}

void sktok(ffly_compilerp __compiler) {
	ff_token_free(next_token(__compiler));
}
