# include "bond.h"
# include "../linux/unistd.h"
# include "../linux/fcntl.h"
# include "../ffly_def.h"
# include "../malloc.h"
# include "../stdio.h"
void static
out_seg(segmentp __seg) {
	struct ffef_seg_hdr seg;
	seg.type = __seg->type;
	seg.offset = __seg->offset;
	seg.adr = __seg->addr;
	seg.sz = __seg->size;
	ff_bond_oust(&seg, ffef_seg_hdrsz);
}

# include "../rdm.h"
void ff_bond_output(ffef_hdrp __hdr) {
	regionp cur = curbin, bk;
	segmentp seg;
	while(cur != NULL) {
		seg = (segmentp)malloc(sizeof(struct segment));
		seg->next = curseg;
		curseg = seg;
		seg->offset = cur->beg;
		seg->type = FF_SG_PROG;
		seg->size = (cur->end-cur->beg);
		seg->addr = cur->adr;
		seg->p = (ff_u8_t*)malloc(seg->size);
		ff_bond_read(cur->beg, seg->p, seg->size);
		ffly_rdm(seg->p, seg->p+seg->size);
		cur = cur->next;
	}

	{
		segmentp cur = curseg, bk;
		while(cur != NULL) {
			lseek(d, cur->offset, SEEK_SET);
			write(d, cur->p, cur->size);
			free(cur->p);
			cur = cur->next;
		}

		cur = curseg;
		while(cur != NULL) {
			out_seg(cur);
			bk = cur;
			cur = cur->next;
			__hdr->nsg++;
		}
	}

	__hdr->sg = offset-ffef_seg_hdrsz;

	lseek(d, 0, SEEK_SET);
	write(d, __hdr, ffef_hdr_size);
}
