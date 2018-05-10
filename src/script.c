# define __ffly_script
# include "script.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "ffly_def.h"
# include "system/io.h"
# include "dep/str_len.h"
/* enabled keywords
*/
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
	"void",
	"exit",
	"while",
	"else",
	"typedef",
	"ret",
	"brk",
	"SYPUT",
	NULL
};

// load keywords
void static
ldkeywds(struct ffly_script *__script) {
	char const **keywd = keywords;
	while(*keywd != NULL) {
		ffly_compiler_ldkeywd(&__script->c_ctx, ffly_compiler_kwno(*keywd));
		ffly_printf("loaded keyword {'%s'}\n", *keywd);
		keywd++;
	}
}

ff_err_t ffly_script_ld(struct ffly_script *__script, char const *__file) {
	ffly_compiler_ld(&__script->c_ctx, __file);
}

ff_err_t ffly_script_prepare(struct ffly_script *__script) {
	ff_err_t ret = ffly_compiler_prepare(&__script->c_ctx);
	ldkeywds(__script);
	return ret;
}

void* ffscript_call(ffscriptp __script, char const *__name) {
	ff_err_t err;
	symbolp sy = (symbolp)ffly_map_get(&__script->symbols, __name, ffly_str_len(__name), &err);
	if (!sy || _err(err))
		return NULL;
	ffscript_exec(__script, NULL, NULL, *sy->start, *sy->end);
	retnull;
}

symbolp static sy_top = NULL;

ff_err_t ffscript_free(ffscriptp __script) {
	struct obj *_obj = (struct obj*)__script->top, *prev = NULL;
	while(!null(_obj)) {
		if (!null(prev))
			__ffly_mem_free(prev);
		prev = _obj;
		_obj = _obj->next;
	}

	if (!null(prev))
		__ffly_mem_free(prev);
	__ffly_mem_free(__script->stack);
	ffly_map_de_init(&__script->symbols);

	symbolp sy = sy_top, bk;
	while(sy != NULL) {
		bk = sy;
		sy = sy->next;
		__ffly_mem_free(bk);
	}
	retok;
}

ffly_mapp static symbols;

ff_err_t ffscript_init(ffscriptp __script, ff_uint_t __stack_size) {
	__script->stack = (ff_byte_t*)__ffly_mem_alloc(__stack_size);
	__script->fresh = __script->stack;
	ffly_map_init(&__script->symbols, _ffly_map_127);
	symbols = &__script->symbols;
	retok;
}

void static syput(void *__p, char const *__name, ff_u8_t __type) {
	ffly_printf("symbol: %p, %s\n", __p, __name);
	symbolp sy = (symbolp)__ffly_mem_alloc(sizeof(struct symbol));
	sy->next = sy_top;
	sy_top = sy;

	struct exec_reg *reg = (struct exec_reg*)__p;
	sy->start = reg->start;
	sy->end = reg->end;
	sy->type = __type;
	ffly_map_put(symbols, __name, ffly_str_len(__name), sy);
}

ff_err_t ffly_script_build(struct ffly_script *__script, void **__top, ff_byte_t **__stack) {
	ffc_build(&__script->c_ctx, __top, __stack, syput);
}

ff_err_t ffly_script_free(struct ffly_script *__script) {
	ffly_compiler_free(&__script->c_ctx);
}

// ignore this, only going to be used for reference for later
/*
ff_err_t ffly_script_save_bin(struct ffly_compiler *__compiler, char *__file) {

	ff_err_t err;
	FF_FILE *f = ffly_fopen(__file, FF_O_WRONLY|FF_O_TRUNC|FF_O_CREAT, FF_S_IRUSR|FF_S_IWUSR, &err);
	ffly_fseek(f, sizeof(ff_u32_t), FF_SEEK_SET);
	ff_u32_t top = ffly_fseek(f, 0, FF_SEEK_CUR);
	ff_u32_t off = top;

	ff_u32_t prev;
	struct obj *_obj = (struct obj*)__compiler->top;
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
				ffly_printf("data: %u\n", *(ff_u16_t*)_obj->p);
				off+= _obj->_type->size;
			}

			ffly_printf("types, off: %u\n", (ff_u32_t)o._type);
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

		if ((void*)_obj != __compiler->top) {
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
	ffly_fwrite(f, &top, sizeof(ff_u32_t));
	ffly_fclose(f);

	return FFLY_SUCCESS;
}

# include "system/bin_tree.h"
ff_err_t ffly_script_ld_bin(struct ffly_compiler *__compiler, char *__file) {

	ff_err_t err;
	FF_FILE *f = ffly_fopen(__file, FF_O_RDONLY, 0, &err);
	struct ffly_bin_tree objs;

	ffly_bin_tree_init(&objs);

	ff_u32_t off;
	ffly_fread(f, &off, sizeof(ff_u32_t));
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
		ffly_fseek(f, (ff_u32_t)_obj->_type, FF_SEEK_SET);
		_obj->_type = (struct obj*)__ffly_mem_alloc(sizeof(struct type));
		ffly_fread(f, _obj->_type, sizeof(struct type));
	
		ffly_printf("---> size: %u\n", _obj->_type->size);
		if (_obj->p != NULL) {
			ffly_fseek(f, (ff_u32_t)_obj->p, FF_SEEK_SET);
			if (_obj->_type->size < 100) { // in case 
				_obj->p = __ffly_mem_alloc(_obj->_type->size);
				ffly_fread(f, _obj->p, _obj->_type->size);
			}
			ffly_printf("data: %u\n", *(ff_u16_t*)_obj->p);
		}
	}

	if (_obj->to != NULL)
		ffly_bin_tree_find(&objs, (ff_u32_t)_obj->to, (void**)&_obj->to);
	if (_obj->from != NULL)
		ffly_bin_tree_find(&objs, (ff_u32_t)_obj->from, (void**)&_obj->from);
	if (_obj->val != NULL)
		ffly_bin_tree_find(&objs, (ff_u32_t)_obj->val, (void**)&_obj->val); 
	if (_obj->l != NULL)
		ffly_bin_tree_find(&objs, (ff_u32_t)_obj->l, (void**)&_obj->l);
	if (_obj->r != NULL)
		ffly_bin_tree_find(&objs, (ff_u32_t)_obj->r, (void**)&_obj->r);
	
	if (_obj->opcode == _op_jump || _obj->opcode == _op_cond_jump) {
		struct obj **p = (struct obj**)__ffly_mem_alloc(sizeof(struct obj*));
		if (_obj->jmp != NULL) {
			ffly_bin_tree_find(&objs, (ff_u32_t)_obj->jmp, (void**)p);
		} else
			*p = NULL;
		_obj->jmp = p;
	}

	if (_obj->flags != NULL)
		ffly_bin_tree_find(&objs, (ff_u32_t)_obj->flags, (void**)&_obj->flags);

	pr_obj(_obj);
	if (prev != NULL)
		prev->next = _obj;
	prev = _obj;

	if (_obj->next != NULL) {
		off = (ff_u32_t)_obj->next;
		goto _next;
	}
	_obj->next = NULL;

	__compiler->top = (void*)top;
	ffly_fclose(f);
	ffly_bin_tree_de_init(&objs);

	return FFLY_SUCCESS;
}
//# define LOAD
*/

ff_err_t ffmain(int __argc, char const **__argv) {
	char const *file;
	if (__argc < 2) {
		ffly_printf("you can provide a file you know?\n");
		file = "../scripts/main.ff";
	} else
		file = __argv[1];

	struct ffly_script script;
	ffscript ff;
	ffscript_init(&ff, 1000);
	ffly_printf("loading script.\n");
	if (_err(ffly_script_prepare(&script))) {
		ffly_printf("failed to prepare.\n");
		return -1;
	}

	ffc_ldlang(&script.c_ctx, _ffc_script);
	ffly_script_ld(&script, file);
	ffly_printf("building script.\n");

	if (_err(ffly_script_build(&script, &ff.top, &ff.fresh))) {
		ffly_printf("failed to build.\n");
		return -1;
	}

	ffly_script_free(&script);

	ffscript_call(&ff, "test");
	ffly_printf("executing script.\n");
	ffscript_exec(&ff, NULL, NULL, NULL, NULL);
	ffly_printf("stack used: %lu\n", ff.fresh-ff.stack);

	ffscript_free(&ff);
}

