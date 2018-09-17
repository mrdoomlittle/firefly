# include "clay.h"
# include "clay/input.h"
# include "clay/lexer.h"
# include "clay/memalloc.h"
# include "ffly_def.h"
# include "linux/unistd.h"
# include "linux/fcntl.h"
# include "linux/stat.h"
# include "clay/parser.h"
void ffly_clay_init(ffly_clayp __clay) {
	__clay->p = NULL;
	__clay->off = 0;
	__clay->end = 0;
	clay_hash_init(&__clay->table);
}

void ffly_clay_de_init(ffly_clayp __clay) {
	clay_mem_cleanup();
}

void ffly_clay_load(ffly_clayp __clay, char const *__file) {
	int fd;

	fd = open(__file, O_RDONLY, 0);

	struct stat st;
	fstat(fd, &st);

	__clay->p = clay_mem_alloc(st.st_size);	
	read(fd, __clay->p, st.st_size);
	__clay->end = st.st_size;
	close (fd);
}

ffly_clayp static clay;
void static
get(ff_uint_t __size, ff_uint_t __offset, void *__buf) {
	*(ff_u8_t*)__buf = *(clay->p+clay->off);
	clay->off++;
}

ff_u8_t static at_eof(void) {
	return (clay->off>=clay->end);
}

ff_u8_t clay_nexttokis(ffly_clayp __clay, ff_u8_t __type, ff_u8_t __val) {
	ff_u8_t tok;
	tok = clay_nexttok(__clay);

	if (tok == __type && clay_tok_val == __val) {
		return 1;
	}

	clay_rtok(tok);
	return 0;
}

ff_u8_t clay_reckontok(ffly_clayp __clay, ff_u8_t __type, ff_u8_t __val) {
	ff_u8_t tok;
	tok = clay_nexttok(__clay);

	return (tok == __type && clay_tok_val == __val);
}

# include "dep/str_len.h"
void* ffly_clay_get(char const *__key, ffly_clayp __clay) {
	return clay_hash_get(&__clay->table, __key, ffly_str_len(__key));
}

void* ffly_clay_tget(char const *__key, void *__trove) {
	struct clay_trove *t;
	t = (struct clay_trove*)__trove;
	return clay_hash_get(&t->table, __key, ffly_str_len(__key));
}

void *ffly_clay_gettext(void *__val) {
	return ((struct clay_val*)__val)->text;
}
# include "system/io.h"

void clay_solidify(ffly_clayp, clay_nodep);
void ffly_clay_read(ffly_clayp __clay) {
	clay_get = get;
	clay_at_eof = at_eof;
	clay = __clay;

	ffly_printf("reading.\n");
	clay_nodep n;
	n = clay_parser(__clay);
	clay_solidify(__clay, n);
}

//# define DEBUG
# ifdef DEBUG
# include "types.h"
ff_err_t ffmain(int __argc, char const *__argv[]) {
	struct ffly_clay clay;
	ffly_clay_init(&clay);

	ffly_clay_load(&clay, "test.clay");
	ffly_clay_read(&clay);

	void *p, *width, *height;
	p = ffly_clay_get("screen", &clay);
	width = ffly_clay_tget("width", p);
	height = ffly_clay_tget("height", p);

	
	ffly_printf("screen, width: %u, height: %u\n", clay_16(width), clay_16(height));
	ffly_clay_de_init(&clay);
}
# endif
