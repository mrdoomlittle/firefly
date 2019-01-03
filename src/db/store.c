# include "../oyster.h"
# include "block.h"
# include "../ffly_def.h"
# include "../system/io.h"
void static
out_record(ffdbp __db, ffdb_recordp __rec) {
	struct ffdb_record_hdr out;
	out.p = __rec->p;
	out.no = __rec->no;
	out.size = __rec->size;
	ffly_fwrite(__db->file, &out, sizeof(struct ffdb_record_hdr));
	ffly_printf("record out, size: %u, no: %u, p: %u\n", out.size, out.no, out.p);
}

void static
out_pile(ffdbp __db, ffdb_pilep __pile, ff_u32_t *__bk) {
	struct ffdb_pile_hdr out;
	out.nr = 0;
	out.no = __pile->no;
	out.next = *__bk;
	ffdb_recordp cur = __pile->top;
	while(cur != NULL) {
		out_record(__db, cur);
		cur = cur->next;
		out.nr++;
	}
	*__bk = ffly_fseek(__db->file, 0, FF_SEEK_CUR);
	ffly_fwrite(__db->file, &out, sizeof(struct ffdb_pile_hdr));
}

void ffdb_settle(ffdbp __db) {
	struct ffdb_hdr hdr;
	*hdr.ident = FFDBF_MAG0;
	hdr.ident[1] = FFDBF_MAG1;
	hdr.ident[2] = FFDBF_MAG2;
	hdr.ident[3] = FFDBF_MAG3;

	hdr.top = ffdb_btop;
	hdr.off = __db->off;

	hdr.bin = !ffdb_bbin?FFDB_NULL:ffdb_bbin->off;
	ffdb_blkdp cur = ffdb_bbin;
	while(cur != NULL) {
		struct ffdb_blkd blk;
		ffdb_blkd_read(__db, &blk, cur->off);
		blk.fd = (void*)!cur->fd?FFDB_NULL:cur->fd->off;
		blk.bk = (void*)!cur->bk?FFDB_NULL:cur->bk->off;
		blk.p = NULL;
		ffdb_blkd_write(__db, &blk, cur->off);
		cur = cur->fd;
	}

	/*
		set offset to end of file
	*/
	ffly_fseek(__db->file, __db->off, FF_SEEK_SET);

	ffdb_pilep pile = __db->top;
	ffdb_recordp rec;
	ff_u32_t bk = FFDB_NULL;
	struct ffdb_pile_hdr out;
	while(pile != NULL) {
		out_pile(__db, pile, &bk);
		pile = pile->next;
	}
	ffly_printf("pile at: {%u}-%s\n", bk, bk == FFDB_NULL?"null":"good");
	hdr.pile = bk;
	
	hdr.fresh = ffdb_nfresh;
	hdr.nd = ffdb_nnext-ffdb_ndiched;
	if (hdr.nd>0) {
		hdr.diched = ffly_fseek(__db->file, 0, FF_SEEK_CUR);
		ffly_fwrite(__db->file, ffdb_ndiched, hdr.nd*sizeof(ff_u16_t));
	}

	ffly_fpwrite(__db->file, &hdr, ffdb_hdrsize, 0);
}
