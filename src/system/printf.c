# include "file.h"
# include "../dep/str_len.h"
# include "printf.h"
# include "io.h"
# include "../dep/str_cpy.h"
# include "../dep/mem_cpy.h"
# include "../dep/mem_set.h"
# include "string.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
# include "../linux/unistd.h"
# include "mutex.h"

// multithread support

# define CHUNK_SHIFT 4
# define CHUNK_SIZE (1<<CHUNK_SHIFT)
ff_mlock_t static lock = FFLY_MUTEX_INIT;
static void **chunks;
ff_u16_t static cut;
ff_u8_t static off;

ff_uint_t static chunk_c;
static FF_FILE *file;
void static
drain(ff_u8_t *__p, ff_u16_t __size, ff_u16_t __cut, ff_u8_t __off) {
	ff_u8_t *p = __p;
	ff_u8_t *end = p+__size;

	ff_uint_t end_off = (__cut*CUT_SIZE)+__off+__size;
	ff_uint_t cc = (end_off>>CHUNK_SHIFT)+((end_off&((~(ff_u64_t)0)>>(64-CHUNK_SHIFT)))>0);

	if (cc>chunk_c) {
		if (!chunks)
			chunks = (void**)__ffly_mem_alloc(cc*sizeof(void*));
		else
			chunks = (void**)__ffly_mem_realloc(chunks, cc*sizeof(void*));
		void **c = chunks+chunk_c;
		void **end = chunks+cc;
		while(c != end)
			*(c++) = NULL;
		chunk_c = cc;
	}

	while(p != end) {
		ff_uint_t off = ((__cut*CUT_SIZE)+__off)+(p-__p);
		ff_uint_t chunk = off>>CHUNK_SHIFT;

		ff_uint_t chunk_off = off-(chunk*CHUNK_SIZE);
		ff_uint_t left = CHUNK_SIZE-chunk_off;
		void **c = chunks+chunk;
		if (!*c) {
			*c = __ffly_mem_alloc(CHUNK_SIZE);
			ffly_mem_set(*c, '@', CHUNK_SIZE);
		}
		ff_uint_t size = __size-(p-__p);

		if (left<size)
			size = left;

		ffly_mem_cpy(((ff_u8_t*)*c)+chunk_off, p, size);
		p+=size;
	}
}

ff_err_t ffly_printf(char const *__format, ...) {
	va_list args;
	va_start(args, __format);
	ffly_vfprintf(ffly_out, __format, args); 
	va_end(args);
}

ff_err_t ffly_fprintf(FF_FILE *__file, char const *__format, ...) {
	va_list args;
	va_start(args, __format);
	ffly_vfprintf(__file, __format, args);
	va_end(args);
}

ff_err_t ffly_vfprintf(FF_FILE *__file, char const *__format, va_list __args) {
	ff_uint_t l = ffly_str_len(__format);
	ffly_vsfprintf(__file, l, __format, __args);
}

ff_uint_t static
gen(ff_size_t __n, char const *__format, va_list __args) {
	char const *p = __format;
	while(p != __format+__n) {
		if (*(p++) == '%') {
			ff_u8_t is_long;
			if (is_long = (*p == 'l')) p++; 
			if (*p == 'd') {
				ff_i64_t v = is_long? va_arg(__args, ff_i64_t):va_arg(__args, ff_i32_t);
				ff_u8_t neg;
				if ((neg = (v < 0))) {
					char c = '-';
					drain(&c, 1, cut, off++);
					v = -v;
				} 
				_ffly_nots(v, &cut, &off, drain);
			} else if (*p == 'u') {
				ff_u64_t v = is_long?va_arg(__args, ff_u64_t):va_arg(__args, ff_u32_t);
				_ffly_nots(v, &cut, &off, drain);
			} else if (*p == 's') {
				char *s = va_arg(__args, char*);
				ff_uint_t l = ffly_str_len(s);
				drain(s, l, cut, off);
				ff_u16_t cc = l>>CUT_SHIFT;
				cut+=cc;
				off+=l-(cc*CUT_SIZE);
			} else if (*p == 'c') {
				char c = va_arg(__args, int);
				drain(&c, 1, cut, off++);
			} else if (*p == 'f') {
	//			double v = va_arg(__args, double);
	//			bufp+=ffly_floatts(v, bufp);
			} else if (*p == 'p') {
				void *v = va_arg(__args, void*);
				_ffly_noths((ff_u64_t)v, &cut, &off, drain);
			} else if (*p == 'x') {
				ff_u64_t v = is_long? va_arg(__args, ff_u64_t):va_arg(__args, ff_u32_t);
				_ffly_noths(v, &cut, &off, drain);
			}
			p++;
		} else // buffer this
			drain(p-1, 1, cut, off++);

		if (off>=CUT_SIZE) {
			cut++;
			off-=CUT_SIZE;
		}
	}
	char c = '\0';
	drain(&c, 1, cut, off);
	return (cut*CUT_SIZE)+off;
}

void static
prep(void) {
	chunks = NULL;
	chunk_c = 0;
	cut = 0;
	off = 0;
}

void static
cleanup(void) {
	void **chunk = chunks;
	void **end = chunk+chunk_c;
	while(chunk != end) {
		__ffly_mem_free(*chunk);
		chunk++;
	}
	__ffly_mem_free(chunks);
}

ff_uint_t ffly_vsprintf(char *__buf, char const *__format, va_list __args) {
	ffly_mutex_lock(&lock);
	ff_uint_t l;
	prep();
	l = gen(ffly_str_len(__format), __format, __args)+1;

	void **chunk = chunks;
	void **end = chunks+(l>>CHUNK_SHIFT);
	char *p = __buf;
	while(chunk != end) {
		ffly_mem_cpy(p, *chunk, CHUNK_SIZE);
		p+=CHUNK_SIZE;
		chunk++;
	}

	ff_uint_t left = l-((l>>CHUNK_SHIFT)*CHUNK_SIZE);
	if (left>0)
		ffly_mem_cpy(p, *chunk, left);
	cleanup();
	ffly_mutex_unlock(&lock);
	return l;
}

ff_uint_t ffly_sprintf(char *__buf, char const *__format, ...) {
	va_list args;
	ff_uint_t l;
	va_start(args, __format);
	l = ffly_vsprintf(__buf, __format, args);
	va_end(args);
	return l;
}

ff_uint_t static out(FF_FILE*, ff_size_t, char const*, va_list, ff_err_t(*)(struct ffly_file*, void*, ff_uint_t));
ff_uint_t ffly_fprintfs(FF_FILE *__file, char const *__format, ...) {
	va_list args;
	ff_uint_t l;
	va_start(args, __format);
	out(__file, ffly_str_len(__format), __format, args, ffly_fwrites);
	va_end(args);
	return l;
}

ff_err_t ffly_vsfprintf(FF_FILE *__file, ff_size_t __n, char const *__format, va_list __args) {
	out(__file, __n, __format, __args, ffly_fwrite);
}

ff_uint_t
out(FF_FILE *__file, ff_size_t __n, char const *__format, va_list __args, ff_err_t(*__write)(struct ffly_file*, void*, ff_uint_t)) {
	ffly_mutex_lock(&lock);
	prep();
	ff_uint_t l;
	l = gen(__n, __format, __args);

	void **chunk = chunks;
	void **end = chunks+(l>>CHUNK_SHIFT);

	while(chunk != end) {
		__write(__file, *chunk, CHUNK_SIZE);
		chunk++;
	}

	ff_uint_t left = l-((l>>CHUNK_SHIFT)*CHUNK_SIZE);
	if (left>0)
		__write(__file, *chunk, left);

	cleanup();
	ffly_mutex_unlock(&lock);
}


/*
int main() {
	ffly_io_init();
	ff_u8_t x;
	ffly_fprintf(ffly_out, "test: %p\n", &x);
	ffly_io_closeup();
}*/

