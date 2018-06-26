# include "../db.h"
# include "../ffly_def.h"
# include "block.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
void ld_record(ffdbp __db, ffdb_pilep __pile, struct ffdb_record_hdr *__rec) {
	ffdb_recordp p = ffdb_record_creat(__db, __pile, __rec->size);
	p->p = __rec->p;
	ffdb_rivet(__rec->no, p);
	ffdb_bind(p, __rec->no);
}

void ld_pile(ffdbp __db, ff_u32_t *__pile) {
	struct ffdb_pile_hdr pile;
	ffly_fseek(__db->file, *__pile, FF_SEEK_SET);
	ffly_fread(__db->file, &pile, sizeof(struct ffdb_pile_hdr));
	ffly_printf("pile: %u\n", *__pile);

	ffdb_pilep p = ffdb_pile_creat(__db);
	ffdb_rivet(pile.no, p);
	ffdb_bind(p, pile.no);
	*__pile = pile.next;

	if (pile.nr>0) {
		ffly_printf("records: %u\n", pile.nr);
		if (pile.nr > 100) {
			ffly_printf("error.\n");
			return;
		}
		struct ffdb_record_hdr *rec = (struct ffdb_record_hdr*)__ffly_mem_alloc(pile.nr*sizeof(struct ffdb_record_hdr));
		ffly_fseek(__db->file, pile.rec, FF_SEEK_SET);
		ffly_fread(__db->file, rec, pile.nr*sizeof(struct ffdb_record_hdr));

		struct ffdb_record_hdr *cur = rec;
		struct ffdb_record_hdr *end = rec+pile.nr;
		while(cur != end) {
			ld_record(__db, p, cur);
			cur++;
		}
		__ffly_mem_free(rec);
	}
}

void ffdb_load(ffdbp __db) {
	struct ffdb_hdr hdr;
	ffly_fseek(__db->file, 0, FF_SEEK_SET);
	ffly_fread(__db->file, &hdr, ffdb_hdrsize);
	if (*hdr.ident != FFDBF_MAG0) {
		ffly_printf("mag0 corrupted\n");
		return;
	}

	if (hdr.ident[1] != FFDBF_MAG1) {
		ffly_printf("mag1 corrupted\n");	
		return;
	}

	if (hdr.ident[2] != FFDBF_MAG2) {
		ffly_printf("mag2 corrupted\n");
		return;
	}

	if (hdr.ident[3] != FFDBF_MAG3) {
		ffly_printf("mag3 corrupted\n");
		return;
	}

	__db->off = hdr.off;
	ffdb_btop = hdr.top;

	ff_u32_t cur = hdr.bin;
	ffdb_blkdp bk = NULL;
	while(cur != FFDB_NULL) {
		ffdb_blkdp blk = (ffdb_blkdp)__ffly_mem_alloc(sizeof(struct ffdb_blkd));
		if (!ffdb_bbin)
			ffdb_bbin = blk;
		ffdb_blkd_read(__db, blk, cur);
		blk->p = blk;
		
		if ((ff_u64_t)blk->bk != FFDB_NULL) {
			ffly_fseek(__db->file, (ff_u64_t)blk->bk+offsetof(struct ffdb_blkd, p), FF_SEEK_SET);
			ffly_fwrite(__db->file, &blk, sizeof(ffdb_blkdp));
		}

		if (bk != NULL)
			bk->fd = blk;
		bk = blk;
		blk->bk = bk;
		cur = (ff_u64_t)blk->fd;
	}
	if (bk != NULL)
		bk->fd = NULL;

	ff_u32_t pile = hdr.pile;
	while(pile != FFDB_NULL) {
		ld_pile(__db, &pile);
	}

	ffdb_nfresh = hdr.fresh;
	ffdb_nnext+=hdr.nd;

	ffly_fseek(__db->file, hdr.diched, FF_SEEK_SET);
	ffly_fread(__db->file, ffdb_ndiched, hdr.nd*sizeof(ff_u16_t));
	ftruncate(ffly_fileno(__db->file), __db->off);
}
