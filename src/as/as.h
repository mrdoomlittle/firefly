# ifndef __ffly__as__h
# define __ffly__as__h
# include "symbol.h"
int extern out;
int extern in;
# include "../resin.h"
# include "../ffint.h"
# define fakget(__off) \
	(*((fak_)->p+__off))
# define inssize struct(ins)
# define is_flag(__flags, __flag) \
	(((__flags)&(__flag))==(__flag))

# define HOOK 0x1
# define RELOCATE 0x2
# define SIGNED 0x1
# define S_ADR 0x2
# define _of_null 0xff
enum {
	_of_ffef,
	_of_elf
};

extern void(*ff_as_forge)(void);
extern void **op_tray;
extern void*(*ff_as_getreg)(char const*);
extern ff_u8_t(*ff_as_regsz)(void*);

ff_u8_t extern of;
ff_u64_t extern offset;

extern void(*post)(void);
# define LA_LOOSE 0x1
typedef struct region* regionp;

# define setinfo(__fak, __bits, __off) \
	(__fak)->info[__off] = __bits
# define getinfo(__fak, __off) \
	((__fak)->info[__off])

# define _o_reg (_o_reg8|_o_reg16|_o_reg32|_o_reg64)
# define _o_imm (_o_imm8|_o_imm16|_o_imm32|_o_imm64)
# define _o_dis (_o_dis8|_o_dis16|_o_dis32|_o_dis64)
# define _o_label 0x1
# define _o_reg8 0x2
# define _o_reg16 0x4
# define _o_reg32 0x8
# define _o_reg64 0x10
# define _o_imm8 0x20
# define _o_imm16 0x40
# define _o_imm32 0x80
# define _o_imm64 0x100
# define _o_dis8 0x200
# define _o_dis16 0x400
# define _o_dis32 0x800
# define _o_dis64 0x1000

typedef struct flask {
	ff_u16_t info[4];
	void *p[4];
	ff_u8_t n;
} *flaskp;

extern struct flask *fak_;

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

extern ff_i8_t _local;
extern void *__label;

// rename
extern labelp curlabel;

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
	ff_u64_t adr;
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

typedef struct st {
	struct st *prev, *next;
	char const *p;
	ff_u8_t l;
} *stp;

stp extern stt_head;
stp extern stt_tail;
ff_u64_t(*ff_as_stt_drop)(void);
labelp extern ff_as_entry;
// alloca.c
// cleanup
void ff_as_al_cu();
// allocate
void *ff_as_al(ff_uint_t);

// as.c
void ff_as(char*, char*);
void* ff_as_memdup(void*, ff_uint_t);

// exp.c
symbolp ff_as_eval(char*);
struct hash extern env;

void ff_as_oust(ff_u8_t*, ff_u8_t);
void ff_as_oustbyte(ff_u8_t);
void ff_as_oust_16l(ff_u16_t);
void ff_as_oust_32l(ff_u32_t);
void ff_as_oust_64l(ff_u64_t);

// stt.c
ff_uint_t ff_as_stt(char const*, ff_uint_t);
void ff_as_final(void);
void ff_as_init(void);
void ff_as_de_init(void);
symbolp ff_as_parse(char*);
ff_u64_t ff_as_read_no(char*, ff_uint_t*, ff_u8_t*);
void ff_as_hash_init(struct hash*);
void ff_as_hash_put(struct hash*, ff_u8_t const*, ff_uint_t, void*);
void* ff_as_hash_get(struct hash*, ff_u8_t const*, ff_uint_t);
ff_uint_t ff_as_stackadr();
void ff_as_isa(ff_uint_t);

void ff_as_resin(void);
void ff_as_amd64(void);
void ff_as_ffef(void);
void ff_as_elf(void);
//void ffef(symbolp);
# endif /*__ffly__as__h*/
