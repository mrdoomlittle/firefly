# define __ffly_compiler_internal
# include "compiler.h"
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
# include "system/realpath.h"
ffly_err_t ffly_compiler_ld(struct ffly_compiler *__compiler, char const *__file) {
	ffly_err_t err;
	FF_FILE *f = ffly_fopen(__file, FF_O_RDONLY, 0, &err);
	__compiler->file->path = ffly_realpath(__file);

	struct ffly_stat st;
	ffly_fstat(__file, &st);

	__compiler->file->p = (ffly_byte_t*)__ffly_mem_alloc(st.size);
	__compiler->file->end = __compiler->file->p+st.size;
	ffly_fread(f, __compiler->file->p, st.size);
	ffly_fclose(f);
	return FFLY_SUCCESS;
}

/*
void set_call(struct ffly_compiler *__compiler, void *__p, mdl_u8_t __no) {
	__compiler->call[__no] = __p;	
}

void __parser_func_call();
void __parser_decl_spec();

void static *jmp[] = {
	&__parser_func_call,
	&__parser_decl_spec
};

mdl_u64_t _ringup(struct ffly_compiler *__compiler, mdl_u8_t __no, ...) {
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

		__asm__("push %%rbp\n\t"
			"mov %%rsp, %%rbp\n\t"
			"sub $24, %%rsp\n\t"
			"mov %0, %%rax\n\t"
			"mov %%rax, -8(%%rbp)\n\t"
			"mov %1, %%rax\n\t"
			"mov %%rax, -16(%%rbp)\n\t"
			"mov %2, %%rax\n\t"
			"mov %%rax, -24(%%rbp)\n\t"
			"call *%3\n\t"
			"mov %%rbp, %%rsp\n\t"
			"pop %%rbp\n\t"
			"mov %%rbp, %%rsp\n\t"
			"pop %%rbp\n\t"
			"ret" : : "m"(p0), "m"(p1), "m"(p2), "r"(__compiler->call[_parser_func_call]));
	}

	__asm__("__parser_decl_spec:\n\t");
	{
		struct ffly_compiler *p0;
		struct token *p1;
		struct type **p2;	
		va_list args;
		va_start(args, __no);
		p0 = va_arg(args, struct ffly_compiler*);
		p1 = va_arg(args, struct token*);
		p2 = va_arg(args, struct type**);
		va_end(args);
		__asm__("push %%rbp\n\t"
			"mov %%rsp, %%rbp\n\t"
			"sub $24, %%rsp\n\t"
			"mov %0, %%rax\n\t"
			"mov %%rax, -8(%%rbp)\n\t"
			"mov %1, %%rax\n\t"
			"mov %%rax, -16(%%rbp)\n\t"
			"mov %2, %%rax\n\t"
			"mov %%rax, -24(%%rbp)\n\t"
			"call *%3\n\t"
			"mov %%rbp, %%rsp\n\t"
			"pop %%rbp\n\t"
			"mov %%rbp, %%rsp\n\t"
			"pop %%rbp\n\t"
			"ret": : "m"(p0), "m"(p1), "m"(p2), "r"(__compiler->call[_parser_decl_spec]));
	}

	__asm__("__parser_struct_decl:\n\t");
	{
		
	}
}
*/
// not the best way but works
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

void vec_cleanup(struct ffly_compiler *__compiler, struct ffly_vec *__vec) {
	struct ffly_vec *vec;
	ffly_vec_push_back(&__compiler->vecs, (void**)&vec);
	*vec = *__vec;
}

void map_cleanup(struct ffly_compiler *__compiler, struct ffly_map *__map) {
	struct ffly_map *map;
	ffly_vec_push_back(&__compiler->maps, (void**)&map);
	*map = *__map;
}

void cleanup(struct ffly_compiler *__compiler, void *__p) {
	void **p;	
	ffly_vec_push_back(&__compiler->to_free, (void**)&p);
	*p = __p;
}


ffly_bool_t is_keyword(struct token *__tok, mdl_u8_t __id) {
	return (__tok->kind == TOK_KEYWORD && __tok->id == __id);
}

ffly_off_t toklo(struct token *__tok) {
	return __tok->lo;
}

ffly_off_t curlo(struct ffly_compiler *__compiler) {
	return __compiler->file->lo;
}

ffly_bool_t next_token_is(struct ffly_compiler *__compiler, mdl_u8_t __kind, mdl_u8_t __id) {
	struct token *tok = next_token(__compiler);
	if (!tok) return 0;
	if (tok->kind == __kind && tok->id == __id)
		return 1;
	ffly_ulex(__compiler, tok);
	return 0;
}

mdl_uint_t tokcol(struct token *__tok) {
	return __tok->off-toklo(__tok);
}

void print_line(struct ffly_compiler *__compiler, mdl_uint_t __off) {
	char *p = (char*)__compiler->file->p+__off;
	while(*p != '\n' && *p != '\0') {
		ffly_printf("%c", *p);
		p++;
	}
	ffly_printf("\n");
}

# define expectmsg(__s) ffly_fprintf(ffly_out, "%s:%u;%u: %s, got %s.\n", __compiler->file->path, tokl(tok), tokcol(tok), __s, tokid_str(tok->id))
ffly_bool_t expect_token(struct ffly_compiler *__compiler, mdl_u8_t __kind, mdl_u8_t __id) {
	struct token *tok = next_token(__compiler);
	if (!tok) {
		errmsg("token is null.\n");
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
			print_line(__compiler, tok->lo);
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

# include "system/util/hash.h"
void put_keyword(struct ffly_compiler *__compiler, char const *__key, mdl_u8_t __id) {
	mdl_u64_t val = ffly_hash((mdl_u8_t const*)__key, ffly_str_len(__key));

	keywdp p = (keywdp)__ffly_mem_alloc(sizeof(struct keywd));
	p->id = __id;
	p->next = (keywdp)ffly_lat_get(&__compiler->keywd, val);
	ffly_lat_put(&__compiler->keywd, val, (void*)p);
	ffly_str_cpy(p->key, __key);
}

keywdp get_keyword(struct ffly_compiler *__compiler, char const *__key) {
	mdl_u64_t val = ffly_hash(__key, ffly_str_len(__key));
	keywdp cur = (keywdp)ffly_lat_get(&__compiler->keywd, val);
	while(!null(cur)) {
		if (!ffly_str_cmp(cur->key, __key)) return cur;
		cur = cur->next;
	}
	return NULL;
}

ffly_err_t ffly_compiler_free(struct ffly_compiler *__compiler) {
	FF_ERR err;
	void *cur = ffly_lat_head(&__compiler->keywd);
	while(cur != NULL) {
		__ffly_mem_free(ffly_lat_getp(cur));	
		ffly_lat_fd(&cur);
	}

	ffly_lat_free(&__compiler->keywd);
	ff_vec *vec;
	___ffly_vec_nonempty(&__compiler->vecs) {
		vec = (ffly_vecp)ffly_vec_begin(&__compiler->vecs);
		while(!vec_deadstop(vec, &__compiler->vecs))
			ffly_vec_de_init(vec++);
	}

	ff_map *map;
	___ffly_vec_nonempty(&__compiler->maps) {
		map = (ffly_mapp)ffly_vec_begin(&__compiler->maps);
		while(!vec_deadstop(map, &__compiler->maps))
			ffly_map_de_init(map++);
	}

	if (_err(err = ffly_map_de_init(&__compiler->env))) {
		errmsg("failed to de-init map.\n");
	}

	if (_err(err = ffly_buff_de_init(&__compiler->sbuf))) {
		errmsg("failed to de-init buffer.\n");
	}

	if (_err(err = ffly_vec_de_init(&__compiler->vecs))) {
		errmsg("failed to de-init vector.\n");
	}

	if (_err(err = ffly_vec_de_init(&__compiler->maps))) {
		errmsg("failed to de-init vector.\n");	
	}

	if (_err(err = ffly_vec_de_init(&__compiler->nodes))) {
		errmsg("failed to de-init vector.\n");
	}

	if (_err(err = ffly_buff_de_init(&__compiler->iject_buff))) {
		errmsg("failed to de-init buffer.\n");
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

	if (__compiler->file->p != NULL)
		__ffly_mem_free(__compiler->file->p);

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

struct token* peek_token(struct ffly_compiler *__compiler) {
	struct token *tok = next_token(__compiler);
	ffly_ulex(__compiler, tok);
	return tok;
}

ffly_bool_t next_tok_nl(struct ffly_compiler *__compiler) {
	ffly_err_t err;
	struct token *tok = ffly_lex(__compiler, &err);
	if (tok != NULL) {
		if (tok->kind == TOK_NEWLINE) {
			__ffly_mem_free(tok);
			return 1;
		}
		ffly_ulex(__compiler, tok);
	}
	return 0;
}

void read_define(struct ffly_compiler *__compiler) {
	struct token *name = next_token(__compiler);
	void *p = NULL;
	if (!next_tok_nl(__compiler)) {
		struct ffly_vec *toks = (ffly_vecp)__ffly_mem_alloc(sizeof(struct ffly_vec));
		ffly_vec_set_flags(toks, VEC_AUTO_RESIZE);
		ffly_vec_init(toks, sizeof(struct token*));
		p = toks;
		struct token **tok;
		_next:
		ffly_vec_push_back(toks, (void**)&tok);
		*tok = next_token(__compiler); 
 
		if (!next_tok_nl(__compiler))
			goto _next;
		vec_cleanup(__compiler, toks);
		cleanup(__compiler, toks);
	}
	ffly_map_put(&__compiler->macros, name->p, ffly_str_len((char*)name->p), p);
}

ffly_bool_t is_endif(struct ffly_compiler *__compiler, struct token *__tok) {
	if (is_keyword(__tok, _percent)) {
		ffly_err_t err;
		struct token *tok = ffly_lex(__compiler, &err);
		return !ffly_str_cmp(tok->p, "endif");
	}
	return 0;
}

void static
skip_until_endif(struct ffly_compiler *__compiler) {
	ffly_err_t err;
	struct token *tok = NULL;
	for(;;) {
		if (!(tok = ffly_lex(__compiler, &err))) break;
		if (_err(err)) break;
		if (is_endif(__compiler, tok)) break;
		if (tok->kind == TOK_NEWLINE)
			__ffly_mem_free(tok);
	}
}

void static
read_ifdef(struct ffly_compiler *__compiler) {
	struct token *name = next_token(__compiler);
	ffly_err_t err;
	ffly_map_get(&__compiler->macros, name->p, ffly_str_len((char*)name->p), &err);
	if (_err(err)) skip_until_endif(__compiler);
}

void static
read_ifndef(struct ffly_compiler *__compiler) {
	struct token *name = next_token(__compiler);
	ffly_err_t err;
	ffly_map_get(&__compiler->macros, name->p, ffly_str_len((char*)name->p), &err);
	if (_ok(err)) skip_until_endif(__compiler);
}

# include "linux/unistd.h"
void static
read_include(struct ffly_compiler *__compiler) {
	struct token *file = next_token(__compiler);
	ffly_printf("include: %s\n", (char*)file->p);
	if (access((char*)file->p, F_OK) == -1) {
		errmsg("file{%s} doesen't exist or access to it has been denied.\n", file->p);
		return;
	}

	__compiler->file++;

	__compiler->file->line = 0;
	__compiler->file->lo = 0;
	__compiler->file->off = 0;
	ffly_compiler_ld(__compiler, (char*)file->p);  
	ffly_parse(__compiler);
	__ffly_mem_free(__compiler->file->path);
	__ffly_mem_free(__compiler->file->p);
	__compiler->file--;
}

void read_macro(struct ffly_compiler *__compiler) {
	struct token *tok = next_token(__compiler);
	if (tok->kind != TOK_IDENT) return;
	if (!ffly_str_cmp(tok->p, "define"))
		read_define(__compiler);
	else if (!ffly_str_cmp(tok->p, "ifdef"))
		read_ifdef(__compiler);
	else if (!ffly_str_cmp(tok->p, "ifndef"))
		read_ifndef(__compiler);
	else if (!ffly_str_cmp(tok->p, "include"))
		read_include(__compiler);
}

struct token* next_token(struct ffly_compiler *__compiler) {
	ffly_err_t err;
	struct token *tok;
	_back:
	tok = ffly_lex(__compiler, &err);
	if (!tok) return NULL;

	if (tok->kind == TOK_NEWLINE) {
		__ffly_mem_free(tok);
		goto _back;
	}

	if (is_keyword(tok, _percent)) {
		read_macro(__compiler);
		goto _back;
	}

	if (tok->kind == TOK_IDENT) {
		struct ffly_vec *toks = (struct ffly_vec*)ffly_map_get(&__compiler->macros, tok->p, ffly_str_len((char*)tok->p), &err);
		if (toks != NULL && _ok(err)) {
			struct token **p = (struct token**)ffly_vec_end(toks);
			while(p >= (struct token**)ffly_vec_begin(toks))
				ffly_ulex(__compiler, *(p--)); 
			tok = next_token(__compiler);
		}
	}

	maybe_keyword(__compiler, tok);
	return tok;
}

mdl_uint_t tokl(struct token *__tok) {
	return __tok->line;
}

void to_keyword(struct token *__tok, mdl_u8_t __id) {
	__tok->kind = TOK_KEYWORD;
	__tok->id = __id;
}

ffly_bool_t maybe_keyword(struct ffly_compiler *__compiler, struct token *__tok) {
	if (!__tok) return 0;
	if (__tok->kind != TOK_IDENT || __tok->p == NULL) return 0;
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
	NULL
};

mdl_u8_t static keyword_ids[] = {
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
	0
};

mdl_u8_t ffly_compiler_kwno(char const *__keyword) {
	char const **s = keywords;
	mdl_u8_t no = 0;
	while(*s != NULL) {
		if (!ffly_str_cmp(*s, __keyword))
			return no;
		no++;
		s++;
	}
	return no;
}

void ffly_compiler_ldkeywd(struct ffly_compiler *__compiler, mdl_u8_t __no) {
	put_keyword(__compiler, keywords[__no], keyword_ids[__no]);
}

ffly_err_t parser_init(struct ffly_compiler*);
ffly_err_t ffly_compiler_prepare(struct ffly_compiler *__compiler) {
	FF_ERR err;
//	parser_init(__compiler);

	ffly_lat_prepare(&__compiler->keywd);

	if (!(__compiler->file = (struct ffly_compiler_file*)__ffly_mem_alloc(12*sizeof(struct ffly_compiler_file)))) {
		errmsg("memory allocation failure.\n");
		reterr;
	}

	if (_err(err = ffly_buff_init(&__compiler->sbuf, 100, 1))) {
		errmsg("failed to initialize buffer.\n");
		_ret;
	}

	ffly_vec_set_flags(&__compiler->to_free, VEC_AUTO_RESIZE);
	if (_err(err = ffly_vec_init(&__compiler->to_free, sizeof(void*)))) {
		errmsg("failed to initialize vector.\n");
		_ret;
	}

	ffly_vec_set_flags(&__compiler->vecs, VEC_AUTO_RESIZE);
	if (_err(err = ffly_vec_init(&__compiler->vecs, sizeof(struct ffly_vec)))) {
		errmsg("failed to initialize vector.\n");
		_ret;
	}

	ffly_vec_set_flags(&__compiler->vecs, VEC_AUTO_RESIZE);
	if (_err(err = ffly_vec_init(&__compiler->maps, sizeof(struct ffly_map)))) {
		errmsg("failed to initialize vector.\n");
		_ret;
	}

	ffly_vec_set_flags(&__compiler->nodes, VEC_AUTO_RESIZE);
	if (_err(err = ffly_vec_init(&__compiler->nodes, sizeof(struct node*)))) {
		errmsg("failed to initialize vector.\n");
		_ret;
	}

	if (_err(err = ffly_buff_init(&__compiler->iject_buff, 100, sizeof(struct token*)))) {
		errmsg("failed to initialize buffer.\n");
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
	__compiler->file->off = 0;
	__compiler->file->line = 0;
	__compiler->file->lo = 0;
	__compiler->local = NULL;
	__compiler->file->p = NULL;
	__compiler->file->path = NULL;
	if (_err(err = ffly_map_init(&__compiler->macros, _ffly_map_127))) {
		errmsg("failed to initialize map.\n");
		_ret;
	}
	__compiler->ret_type = NULL;
	retok;
}

ffly_err_t ffly_compiler_finalize(struct ffly_compiler *__compiler, void **__top, ffly_byte_t **__stack) {
	ffly_err_t err;
	if (_err(err = ffly_gen(__compiler, __top, __stack))) {
		errmsg("an error has occurred in the generative prossess.\n");
		_ret;
	}
	retok;
}

ffly_err_t ffly_compiler_build(struct ffly_compiler *__compiler, void ** __top, ffly_byte_t **__stack) {
	ffly_err_t err;
	if (_err(err = ffly_parse(__compiler))) {
		errmsg("an error has occurred in the parsing prossess.\n");
		_ret;
	}

	if (_err(err = ffly_compiler_finalize(__compiler, __top, __stack))) {
		errmsg("failed to finalize.\n");
		_ret;
	}
	ffly_fprintf(ffly_out, "build successful.\n");
	retok;
}

ffly_bool_t at_eof(struct ffly_compiler *__compiler) {
	return (__compiler->file->p+__compiler->file->off) >= __compiler->file->end;
}

mdl_uint_t curl(struct ffly_compiler *__compiler) {
	return __compiler->file->line;
}

void skip_token(struct ffly_compiler *__compiler) {
	next_token(__compiler);
}
