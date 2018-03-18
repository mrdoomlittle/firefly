# ifndef __ffly__as__h
# define __ffly__as__h
int extern out;
int extern in;
# include "../bci.h"
# include <mdlint.h>
# define inssize struct(ins)
# define is_flag(__flags, __flag) \
	((__flags&__flag)==__flag)
# define SY_STR 0x1
# define SY_MAC 0x2
# define SY_CHR 0x3
# define SY_INT 0x4
# define SY_LABEL 0x5
# define SY_DIR 0x6
# define SY_REG 0x7
# define is_syreg(__st) \
	((__sy)->sort==SY_REG)
# define is_symac(__sy) \
	((__sy)->sort==SY_MAC)
# define is_systr(__sy) \
	((__sy)->sort==SY_STR)
# define is_sychr(__sy) \
	((__sy)->sort==SY_CHR)
# define is_syint(__sy) \
	((__sy)->sort==SY_INT)
# define is_sylabel(__sy) \
	((__sy)->sort==SY_LABEL)
# define is_sydir(__sy) \
	((__sy)->sort==SY_DIR)
# define SIGNED 0x1
# define _of_null 0xff
enum {
	_of_ffef
};

mdl_u8_t extern of;

typedef struct symbol {
	void *p;
	mdl_u8_t len:6; // max 64
	mdl_u8_t flags:4; // max 16
	mdl_u8_t sign:1; 
	mdl_u8_t sort:4; // max 16
	struct symbol *next;
} *symbolp;

typedef struct ins {
	char *name;
	struct ins *next;
	void(*post)(struct ins*);
	symbolp l, r;
	mdl_u8_t opcode[8];
} *insp;

typedef struct label {
	mdl_uint_t offset;	
} *labelp;

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
struct hash extern symbols;
struct hash extern env;
void _cleanup();
void* _alloca(mdl_uint_t);
void oust(mdl_u8_t*, mdl_u8_t);
void oustbyte(mdl_u8_t);
void oust_16l(mdl_u16_t);
void oust_32l(mdl_u32_t);
void oust_64l(mdl_u64_t);
void finalize(void);
void ffas_init(void);
void ffas_de_init(void);
symbolp parse(char*);
mdl_u64_t read_no(char*, mdl_uint_t*, mdl_u8_t*);
void hash_init(struct hash*);
void hash_put(struct hash*, mdl_u8_t const*, mdl_uint_t, void*);
void* hash_get(struct hash*, mdl_u8_t const*, mdl_uint_t);
void load(insp*);
//void ffef(symbolp);
# endif /*__ffly__as__h*/
