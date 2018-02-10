# ifndef __ffly__as__h
# define __ffly__as__h
int extern out;
int extern in;
# include <mdlint.h>
# define inssize struct(ins)
# define is_flag(__flags, __flag) \
	((__flags&__flag)==__flag)
# define SY_STR 0x1
# define SY_MAC 0x2

typedef struct symbol {
	void *p;
	mdl_u8_t len:6;
	mdl_u8_t flags:4;
	mdl_u8_t sign:1;
	struct symbol *next;
} *symbolp;

typedef struct ins {
	char *name;
	struct ins *next;
	void(*post)(struct ins*);
	mdl_u8_t opcode[8];
} *insp;

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
void* memdup(void*, mdl_uint_t);
struct hash extern symbols;
struct hash extern globl;
void _cleanup();
void* _alloca(mdl_uint_t);
void oustbyte(mdl_u8_t);
symbolp parse(char*);
mdl_uint_t read_no(char*, mdl_uint_t*);
void hash_init(struct hash*);
void hash_put(struct hash*, mdl_u8_t const*, mdl_uint_t, void*);
void* hash_get(struct hash*, mdl_u8_t const*, mdl_uint_t);
void load(insp*);
# endif /*__ffly__as__h*/
