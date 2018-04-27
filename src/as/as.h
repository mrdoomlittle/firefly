# ifndef __ffly__as__h
# define __ffly__as__h
# include "symbol.h"
int extern out;
int extern in;
# include "../bci.h"
# include "../ffint.h"
# define inssize struct(ins)
# define is_flag(__flags, __flag) \
	((__flags&__flag)==__flag)

# define HOOK 0x1
# define RELOCATE 0x2
# define SIGNED 0x1
# define S_ADR 0x2
# define _of_null 0xff
enum {
	_of_ffef
};

ff_u8_t extern of;
ff_u64_t extern offset;

typedef struct label *labelp;

typedef struct ins {
	char *name;
	struct ins *next;
	void(*post)(struct ins*);
	symbolp l, r;
	ff_u8_t op;
} *insp;

# define LA_LOOSE 0x1
typedef struct region* regionp;
typedef struct label {
	ff_uint_t offset, s_adr, adr;
	ff_u8_t flags;
	char const *s;
	symbolp sy;
	regionp reg;
} *labelp;

typedef struct local_label {
	ff_uint_t adr;
	ff_uint_t *p_adr;
} *local_labelp;

typedef struct segment {
	struct segment *next;
	ff_u64_t offset;
	ff_u64_t adr;
	ff_uint_t size;
	ff_u8_t buf[200];
	ff_u8_t *fresh;
} *segmentp;

typedef struct region {
	char const *name;
	struct region *next;
	ff_u64_t beg, end;
	ff_u16_t no;
} *regionp;

typedef struct relocate {
	struct relocate *next;
	ff_u64_t offset;
	ff_u8_t l;
	symbolp *sy;
	local_labelp ll;
} *relocatep;

typedef struct hook {
	struct hook *next;
	ff_u64_t offset;
	ff_u8_t l;
	symbolp *to;
} *hookp;

segmentp extern curseg;
regionp extern curreg;

typedef struct hash_entry {
	struct hash_entry *next;
	ff_u8_t const *key;
	void *p;
	ff_uint_t len;
} *hash_entryp;

struct hash {
	struct hash_entry **table;
};

void assemble(char*, char*);
void* _memdup(void*, ff_uint_t);
symbolp eval(char*);
void* _memdup(void*, ff_uint_t);
struct hash extern env;
void _cleanup();
void* _alloca(ff_uint_t);
void oust(ff_u8_t*, ff_u8_t);
void oustbyte(ff_u8_t);
void oust_16l(ff_u16_t);
void oust_32l(ff_u32_t);
void oust_64l(ff_u64_t);


// stt.c
ff_uint_t stt(char const*, ff_uint_t);
ff_u64_t stt_drop();

void reloc(ff_u64_t, ff_u8_t, symbolp*, local_labelp);
void hook(ff_u64_t, ff_u8_t, symbolp*, local_labelp);
void finalize(void);
void ffas_init(void);
void ffas_de_init(void);
symbolp parse(char*);
ff_u64_t read_no(char*, ff_uint_t*, ff_u8_t*);
void hash_init(struct hash*);
void hash_put(struct hash*, ff_u8_t const*, ff_uint_t, void*);
void* hash_get(struct hash*, ff_u8_t const*, ff_uint_t);
void load(insp*);
ff_uint_t stackadr();
void isa(ff_uint_t);

//void ffef(symbolp);
# endif /*__ffly__as__h*/
