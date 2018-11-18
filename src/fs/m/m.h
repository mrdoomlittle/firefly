# ifndef __ffly__fs__m__h
# define __ffly__fs__m__h
# include "../../ffint.h"
// mimicry file system
#define MFS_SLAB_INUSE 0x01
#define MFS_SLAB_SHIFT 4
#define MFS_SLAB_SIZE (1<<MFS_SLAB_SHIFT)

#define MFS_FREG 0
#define MFS_FDIR 1
#define MFS_NNA 0x01
#define MFS_NDNA 0x02
#define MFS_CREAT 0x01

struct mfs_slab {
	ff_u32_t in; // slab ident
	ff_u32_t off;
	struct mfs_slab **bk, *fd;
	struct mfs_slab *next, *prev;
	ff_u8_t flags;
};

struct mfs_node;
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

#define MFS_BALE_NULL 0xffffffff
#define MFS_BALE_SHIFT 4
#define MFS_BALE_SIZE (1<<MFS_BALE_SHIFT)
struct mfs_bale {
	ff_u32_t slabs[MFS_BALE_SIZE];
	ff_u32_t next;
};

struct mfs_tract {
	ff_u32_t cr;
	ff_uint_t crnc;
	ff_u32_t root;
};

struct mfs_engrave {
	ff_u32_t h;
	ff_u8_t mode;
	ff_u8_t flags;
	ff_uint_t n;
	ff_u32_t start;
	ff_uint_t bale_c;
	char name[86];
};

struct mfs_node {
	char name[86];
	struct mfs_scope *s;
	ff_u32_t ca;
	ff_u8_t flags;
	ff_uint_t size;
	ff_u32_t h;
	ff_u8_t mode;
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

extern struct ffly_mfs *mfs;
struct mfs_scope* mfs_alloc(ff_uint_t);
void mfs_free(struct mfs_scope*);
void ffly_mfs_init(void);
void ffly_mfs_write(ff_u32_t, void*, ff_uint_t, ff_u64_t);
void ffly_mfs_read(ff_u32_t, void*, ff_uint_t, ff_u64_t);
void mfs_resize(struct mfs_scope*, ff_uint_t);

ff_u32_t mfs_hash_new(void);
void mfs_hash_put(ff_u32_t, ff_u8_t const*, ff_uint_t, void*);
void* mfs_hash_get(ff_u32_t, ff_u8_t const*, ff_uint_t);
struct mfs_file* mfs_file_new(void);
struct mfs_file* mfs_file_get(ff_u32_t);
void mfs_file_destroy(struct mfs_file*);
void mfs_tree(struct mfs_node*);
ff_u32_t mfs_open(char const*, ff_u8_t);
void ffly_mfs(void);
void ffly_mfs_de_init(void);
# endif /*__ffly__mfs__h*/
