# ifndef __ffly__as__h
# define __ffly__as__h
# include "symbol.h"
int extern out;
int extern in;
# include "../bci.h"
# include <mdlint.h>
# define inssize struct(ins)
# define is_flag(__flags, __flag) \
	((__flags&__flag)==__flag)

# define HOOK 0x1
# define RELOCATE 0x2
# define SIGNED 0x1
# define _of_null 0xff
enum {
	_of_ffef
};

mdl_u8_t extern of;
mdl_u64_t extern offset;

typedef struct label *labelp;

typedef struct ins {
	char *name;
	struct ins *next;
	void(*post)(struct ins*);
	symbolp l, r;
	mdl_u8_t opcode[8];
} *insp;

# define LA_LOOSE 0x1
typedef struct region* regionp;
typedef struct label {
	mdl_uint_t offset, s_adr, adr;
	mdl_u8_t flags;
	char const *s;
	regionp reg;
} *labelp;

typedef struct segment {
	char const *name;
	struct segment *next;
	mdl_u64_t offset;
	mdl_u64_t addr;
	mdl_uint_t size;
	mdl_u8_t buf[200];
	mdl_u8_t *fresh;
} *segmentp;

typedef struct region {
	char const *name;
	struct region *next;
	mdl_u64_t beg, end;
	mdl_u16_t no;
} *regionp;

typedef struct relocate {
	struct relocate *next;
	mdl_u64_t offset;
	mdl_u8_t l;
	labelp la;
} *relocatep;

typedef struct hook {
	struct hook *next;
	mdl_u64_t offset;
	mdl_u8_t l;
	labelp to;
} *hookp;

segmentp extern curseg;
regionp extern curreg;

typedef struct hash_entry {
	struct hash_entry *next;
	mdl_u8_t const *key;
	void *p;
	mdl_uint_t len;
} *hash_entryp;

struct hash {
	struct hash_entry **table;
};

void assemble(char*, char*);
char* read_str(char*, mdl_uint_t*);
symbolp eval(char*);
void* _memdup(void*, mdl_uint_t);
struct hash extern env;
void _cleanup();
void* _alloca(mdl_uint_t);
void oust(mdl_u8_t*, mdl_u8_t);
void oustbyte(mdl_u8_t);
void oust_16l(mdl_u16_t);
void oust_32l(mdl_u32_t);
void oust_64l(mdl_u64_t);


// stt.c
mdl_uint_t stt(char const*, mdl_uint_t);
mdl_u64_t stt_drop();

void reloc(mdl_u64_t, mdl_u8_t, labelp);
void hook(mdl_u64_t, mdl_u8_t, labelp);
void finalize(void);
void ffas_init(void);
void ffas_de_init(void);
symbolp parse(char*);
mdl_u64_t read_no(char*, mdl_uint_t*, mdl_u8_t*);
void hash_init(struct hash*);
void hash_put(struct hash*, mdl_u8_t const*, mdl_uint_t, void*);
void* hash_get(struct hash*, mdl_u8_t const*, mdl_uint_t);
void load(insp*);
mdl_uint_t stackadr();
void isa(mdl_uint_t);

//void ffef(symbolp);
# endif /*__ffly__as__h*/
