# ifndef __ffly__db__block__h
# define __ffly__db__block__h
# include "../ffint.h"
# include "../types.h"
# include "../db.h"
# define blkd_size sizeof(struct ffdb_blkd)
# define PAGE_SHIFT 6
# define PAGE_SIZE (1<<PAGE_SHIFT)
// block
struct ffdb_blkd {
	ff_uint_t size;
	ff_u32_t end, off;
	struct ffdb_blkd *fd, *bk, *p;
	ff_u32_t prev, next;
	ff_u8_t inuse;
};


// later blocks should be alligned to page size
/*
	balloc returns an offset in the db file,
	later reads and writes will load that block into memory
	if not used for an x period of time then the block will be 
	loaded backinto the file and memory will be freed <- larger
	blocks will be loaded page by page
*/

typedef struct ffdb_blkd* ffdb_blkdp;

ff_u32_t extern ffdb_btop;
ffdb_blkdp extern ffdb_bbin;
// block-alloc
ff_uint_t ffdb_balloc(ffdbp, ff_uint_t);

// block-free
void ffdb_bfree(ffdbp, ff_uint_t);
void ffdb_blkd_write(ffdbp, ffdb_blkdp, ff_off_t);
void ffdb_blkd_read(ffdbp, ffdb_blkdp, ff_off_t);
# endif /*__ffly__db__block__h*/
