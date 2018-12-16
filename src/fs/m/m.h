# ifndef __ffly__fs__m__h
# define __ffly__fs__m__h
# include "../../ffint.h"
// mimicry file system

#define cr_write(__buf, __size, __dst)\
	mfs_swrite(mfs_cr.s, __buf, __size, __dst)
#define cr_read(__buf, __size, __src)\
	mfs_sread(mfs_cr.s, __buf, __size, __src)

#define MFS_SLAB_INUSE 0x01
#define MFS_SLAB_SHIFT 4
#define MFS_SLAB_SIZE (1<<MFS_SLAB_SHIFT)

#define MFS_PC_NULL 0xffffffff
#define MFS_FREG 0
#define MFS_FDIR 1
#define MFS_NNA 0x01
#define MFS_NDNA 0x02
#define MFS_PCP 0x04
#define MFS_NNL 0x08
#define MFS_HHT 0x01

#define MFS_CREAT 0x01
struct mfs_slab {
	ff_u32_t in; // slab ident
	ff_u32_t off;
	struct mfs_slab **bk, *fd;
	struct mfs_slab *next, *prev;
	ff_u8_t flags;
};

struct mfs_node;

struct mfs_slab_struc {
	ff_u32_t in;
	ff_u8_t flags;
	ff_u32_t off;
	ff_u32_t pv, nx;
};

struct ffly_mfs {
	void(*write)(void*, ff_uint_t, ff_u64_t);
	void(*read)(void*, ff_uint_t, ff_u64_t);
	ff_u32_t off;
	struct mfs_slab *top, *bin;
	struct mfs_node *root;
};

struct mfs_scope {
	struct mfs_slab **slabs;
	ff_uint_t slab_c;
};

struct mfs_cauldron {
	struct mfs_scope *s;
	ff_uint_t n;
};

struct mfs_dent {
	char const *name;
	ff_uint_t nlen;
	ff_u8_t id;
	struct mfs_node *n;
};

#define MFS_BALE_NULL	0xffffffff
#define MFS_BALE_SHIFT	4
#define MFS_BALE_SIZE	(1<<MFS_BALE_SHIFT)
struct mfs_bale {
	ff_u32_t slabs[MFS_BALE_SIZE];
	ff_u32_t next;
};

struct mfs_tract {
	// cauldron start location for slab ident
	ff_u32_t cr;
	// node count
	ff_uint_t crnc;
	// root node - cauldron address
	ff_u32_t root;
	// slab count
	ff_uint_t crsc;
	ff_uint_t sc;
	ff_u32_t slabs;
	ff_u32_t off;
};

struct mfs_cb_struc {
	ff_u16_t off;
	ff_u16_t size;
	ff_u32_t n;
};

struct mfs_engrave {
	ff_u32_t h;
	ff_u32_t pc;
	ff_uint_t pcs;
	ff_u8_t mode;
	ff_u8_t flags;
	ff_u32_t nn;
	ff_uint_t n;
	ff_u32_t start;
	ff_uint_t bale_c;
	ff_u32_t cbs;
	ff_uint_t cbc;
	char name[86];
};

struct mfs_cb {
	void *p;
	ff_u16_t off;
	ff_uint_t size;
};

struct mfs_node {
	char name[86];
	// prep code
	struct mfs_cb **cbs;
	ff_u32_t _cbs;
	ff_uint_t cbc;
	ff_uint_t pcs;
	ff_u32_t pc;
	struct mfs_scope *s;
	// cauldron address
	ff_u32_t ca;
	// node number
	ff_u32_t nn;
	ff_u8_t flags;
	// non tacked flags
	/*
		tobe murged with flags 
	*/
	ff_u8_t ntf;

	// why the fuck is this here?
	ff_uint_t size;
	ff_u32_t h;
	ff_u8_t mode;

	//size
	ff_uint_t n;
};

struct mfs_fo;
struct mfs_file {
	struct mfs_node *n;
	ff_u32_t i;
	struct mfs_fo *op;
};

struct mfs_fo {
	void(*write)(struct mfs_file*, void*, ff_uint_t, ff_u64_t);
	void(*read)(struct mfs_file*, void*, ff_uint_t, ff_u64_t);
};

struct mfs_dir {
};

void mfs_swrite(struct mfs_scope*, void*, ff_uint_t, ff_u64_t);
void mfs_sread(struct mfs_scope*, void*, ff_uint_t, ff_u64_t);

extern struct mfs_cauldron mfs_cr;
extern struct ffly_mfs *mfs;
struct mfs_scope* mfs_alloc(ff_uint_t);
void mfs_free(struct mfs_scope*);
void ffly_mfs_init(void);
void ffly_mfs_write(ff_u32_t, void*, ff_uint_t, ff_u64_t);
void ffly_mfs_read(ff_u32_t, void*, ff_uint_t, ff_u64_t);
void mfs_resize(struct mfs_scope*, ff_uint_t);

ff_u32_t mfs_hash_new(void);
void mfs_hash_put(ff_u32_t, ff_u8_t const*, ff_uint_t, long long);
long long mfs_hash_get(ff_u32_t, ff_u8_t const*, ff_uint_t, ff_i8_t*);
struct mfs_file* mfs_file_new(void);
struct mfs_file* mfs_file_get(ff_u32_t);
void mfs_file_destroy(struct mfs_file*);
void mfs_tree(struct mfs_node*);
ff_u32_t mfs_open(char const*, ff_u8_t);
void ffly_mfs(void);
void ffly_mfs_de_init(void);
ff_u32_t mfs_valloc(ff_uint_t);
void mfs_exec(ff_u8_t*, ff_uint_t);
# endif /*__ffly__mfs__h*/
