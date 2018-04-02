# include "bond.h"
# include "../linux/unistd.h"
# include "../linux/fcntl.h"
# include "../ffly_def.h"
# include "../malloc.h"

void out_seg(segmentp __seg) {
	struct ffef_seg_hdr seg;
	seg.type = __seg->type;
	seg.offset = __seg->offset;
	seg.adr = __seg->addr;
	seg.sz = __seg->size;
	oust(&seg, ffef_seg_hdrsz);
}

void output(ffef_hdrp __hdr) {
	regionp cur = curbin, bk;
	segmentp seg;
	while(cur != NULL) {
		seg = (segmentp)malloc(sizeof(struct segment));
		seg->next = curseg;
		curseg = seg;

		seg->type = FF_SG_PROG;
		seg->addr = 0;
		seg->size = (cur->end-cur->beg);
		seg->p = (mdl_u8_t*)malloc(seg->size);
		bond_read(cur->beg, seg->p, seg->size);
		cur = cur->next;
	}

	{
		segmentp cur = curseg, bk;
		while(cur != NULL) {
			cur->offset = offset;
			oust(cur->p, cur->size);
			free(cur->p);
			cur = cur->next;
		}

		__hdr->sg = offset;
		cur = curseg;
		while(cur != NULL) {
			out_seg(cur);
			bk = cur;
			cur = cur->next;
			__hdr->nsg++;
		}
	}


	lseek(d, 0, SEEK_SET);
	write(d, __hdr, ffef_hdr_size);
}
