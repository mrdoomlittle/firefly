# include "block.h"
# include "../oyster.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../system/io.h"
/*
	TODO:

	multipools

	2 or more files for diffrent threads
	after disconnect will be murged with main file


	also 
		allow for slab allocation on top
*/
// not the best but for now its okay 
/*
	
	complete cesspool using lseek and direct r/w
*/
/*
	TODO:
		ffdb_bbin static

		add get functions to get ffdb_bbin
		also remove fseek and use pread pwrite

	TONOTE:
		this is an extremely shit and i know that

		for now this works and allows me to build everything else

	TODO:
		all large allocations should be linear

		for smaller ones use slabs
*/

// top block
ff_u32_t ffdb_btop = FFDB_NULL;

// block bin
ffdb_blkdp ffdb_bbin = NULL;

void ffdb_unlink(ffdb_blkdp);
/*
	allocate block
*/
ff_uint_t ffdb_balloc(ffdbp __db, ff_uint_t __bc) {
	if (!__bc) return FFDB_NULL;
	if (ffdb_bbin != NULL) {
		struct ffdb_blkd *p = ffdb_bbin;
		while(p != NULL) {
			ffly_printf("%u >= %u\n", p->size, __bc);
			if (p->size >= __bc) {
				ff_uint_t leftover = p->size-__bc;
				if (leftover>blkd_size) {
					if (leftover-blkd_size >= PAGE_SIZE) {

					}
				}
				ffly_printf("found free space.\n");
				ff_uint_t off = p->off;
				ffly_fseek(__db->file, off+offsetof(struct ffdb_blkd, inuse), FF_SEEK_SET);
				ff_u8_t inuse = 1;
				ffly_fwrite(__db->file, &inuse, 1);
				ffdb_unlink(p);
				return off+blkd_size; 
			}
			p = p->fd;
		}
	}

	ff_u32_t off = __db->off;
	struct ffdb_blkd blk = {
		.size = __bc,
		.off = off,
		.fd = NULL,
		.bk = NULL,
		.p = NULL,
		.prev = ffdb_btop,
		.next = FFDB_NULL, .inuse = 1
	};

	if (ffdb_btop != FFDB_NULL) {
		ffly_fseek(__db->file, ffdb_btop+offsetof(struct ffdb_blkd, next), FF_SEEK_SET);
		ffly_fwrite(__db->file, &off, sizeof(ff_u32_t));
	}

	ffdb_btop = off;
	__db->off+=__bc+blkd_size;
	blk.end =  __db->off;
	ffdb_blkd_write(__db, &blk, off);
	return off+blkd_size;
}


// static?????
/*
	not the best but now dont realy care
*/
void ffdb_reattach(ffdbp __db, struct ffdb_blkd *__blk) {
	struct ffdb_blkd prev, next;
	if (__blk->end == __db->off) {
		ffly_fseek(__db->file, ffdb_btop+offsetof(struct ffdb_blkd, next), FF_SEEK_SET);
		ffly_fwrite(__db->file, &__blk->off, sizeof(ff_u32_t));	
		ffdb_btop = __blk->off;
		return;
	}  

	if (__blk->prev != FFDB_NULL) {
		ffly_fseek(__db->file, __blk->prev+offsetof(struct ffdb_blkd, next), FF_SEEK_SET);
		ffly_fwrite(__db->file, &__blk->off, sizeof(ff_u32_t));
	}

	if (__blk->next != FFDB_NULL) {
		ffly_fseek(__db->file, __blk->next+offsetof(struct ffdb_blkd, prev), FF_SEEK_SET);
		ffly_fwrite(__db->file, &__blk->off, sizeof(ff_u32_t));
	}
}

void ffdb_detatch(ffdbp __db, struct ffdb_blkd *__blk) {
	struct ffdb_blkd prev, next;
	if (__blk->off == ffdb_btop) {
		if ((ffdb_btop = __blk->prev) != FFDB_NULL) {
			ffly_fseek(__db->file, ffdb_btop+offsetof(struct ffdb_blkd, next), FF_SEEK_SET);
			ff_u32_t buf = FFDB_NULL;
			ffly_fwrite(__db->file, &buf, sizeof(ff_u32_t));
		}
		return;
	}

	if (__blk->prev != FFDB_NULL) {
		ffly_fseek(__db->file, __blk->prev+offsetof(struct ffdb_blkd, next), FF_SEEK_SET);
		ffly_fwrite(__db->file, &__blk->next, sizeof(ff_u32_t));
	}

	if (__blk->next != FFDB_NULL) {
		ffly_fseek(__db->file, __blk->next+offsetof(struct ffdb_blkd, prev), FF_SEEK_SET);
		ffly_fwrite(__db->file, &__blk->prev, sizeof(ff_u32_t));
	}
}

void ffdb_unlink(ffdb_blkdp __blk) {
	if (__blk == ffdb_bbin) {
		if ((ffdb_bbin = __blk->fd) != NULL)
			ffdb_bbin->bk = NULL;
		goto _end;
	}

	if (__blk->fd != NULL)
		__blk->fd->bk = __blk->bk;
	if (__blk->bk != NULL)
		__blk->bk->fd = __blk->fd;
_end:
	__ffly_mem_free(__blk);
}

# include "linux/unistd.h"
void ffdb_bfree(ffdbp __db, ff_uint_t __p) {
	if (__p == FFDB_NULL) return;
	struct ffdb_blkd blk;
	ff_off_t off = __p-blkd_size;
	ffdb_blkd_read(__db, &blk, off);
	ffdb_detatch(__db, &blk);

	ffly_printf("blockd size: %u\n", blkd_size);
	ffly_printf("0; %ubytes.\n", blk.size);
	struct ffdb_blkd prev, next;
	if (blk.prev != FFDB_NULL) {
		ffdb_blkd_read(__db, &prev, blk.prev);
		if (!prev.inuse) {
			ffly_printf("prev not inuse, %ubytes.\n", blk.size);
			ffdb_detatch(__db, &prev);
			ffdb_unlink(prev.p);
			blk.size+=prev.size+blkd_size;
			blk.prev = prev.prev;
			blk.off = prev.off;
			off = blk.off;
		}
	}
	ffly_printf("1; %ubytes.\n", blk.size);
	if (blk.next != FFDB_NULL) {
		ffdb_blkd_read(__db, &next, blk.next);
		if (!next.inuse) {
			ffly_printf("next not inuse, %ubytes.\n", blk.size);
			ffdb_detatch(__db, &next);
			ffdb_unlink(next.p);
			blk.next = next.next;
			blk.size+=next.size+blkd_size;
			blk.end = next.end; 
		}
	}
	ffly_printf("2; %ubytes.\n", blk.size);
	
	if (blk.end == __db->off) {
		__db->off = off;
		ftruncate(ffly_fileno(__db->file), off);
		return;
	}
	ffdb_reattach(__db, &blk);
	blk.inuse = 0;
	blk.p = (ffdb_blkdp)__ffly_mem_alloc(sizeof(struct ffdb_blkd));
	*blk.p = blk;
	blk.p->bk = NULL;
	blk.p->fd = ffdb_bbin;
	if (ffdb_bbin != NULL)
		ffdb_bbin->bk = blk.p;
	ffdb_bbin = blk.p;
	ffly_printf("freed: %u\n", blk.size);
	ffdb_blkd_write(__db, &blk, off);	
}

void ffdb_blkd_write(ffdbp __db, ffdb_blkdp __p, ff_off_t __off) {
	ffly_fseek(__db->file, __off, FF_SEEK_SET);
	ffly_fwrite(__db->file, __p, blkd_size);
}

void ffdb_blkd_read(ffdbp __db, ffdb_blkdp __p, ff_off_t __off) {
	ffly_fseek(__db->file, __off, FF_SEEK_SET);
	ffly_fread(__db->file, __p, blkd_size);
}
