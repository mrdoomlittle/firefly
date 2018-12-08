# include "bond.h"
# include "../linux/unistd.h"
# include "../linux/fcntl.h"
# include "../ffly_def.h"
# include "../malloc.h"
# include "../stdio.h"
# include "../string.h"
void static
out_seg(segmentp __seg) {
	struct remf_seg_hdr seg;
	seg.type = __seg->type;
	seg.offset = __seg->offset;
	seg.adr = __seg->addr;
	seg.sz = __seg->size;
	bond_oust(&seg, remf_seghdrsz);
}
# include "../hexdump.h"
# include "../rdm.h"
void bond_output(remf_hdrp __hdr) {
	regionp cur = curbin, bk;
	segmentp seg;
	struct frag *fs, *fe, *f;
	while(cur != NULL) {
		fs = bond_fbn(cur->fs);
		fe = bond_fbn(cur->fs+cur->nf-1);
		seg = (segmentp)malloc(sizeof(struct segment));
		seg->next = curseg;
		curseg = seg;

		seg->type = FF_SG_PROG;
		seg->size = ((fe->adr+fe->size)-fs->adr);
		seg->addr = fs->adr;
		seg->p = (ff_u8_t*)malloc(seg->size);
		f = fs;
		ff_u8_t *p;
		while(f != fe->next) {
			printf("copying frag-%u\n", f->f);
			p = ((ff_u8_t*)seg->p)+(f->adr-fs->adr);
			bond_read(f->adr-f->m, p, f->size);
			if (f->bs>0) {
				memcpy(p+f->size, f->data, f->bs);
			}
			f = f->next;
		}
		printf("segment size: %u, m: %u\n", seg->size, fe->m-fs->m);
		ffly_hexdump(seg->p, seg->size);
		ffly_rdmp(seg->p, seg->size);
		cur = cur->next;
	}

	ff_u32_t out;
	out = remf_hdrsz;
	lseek(d, out, SEEK_SET);
	{
		segmentp cur = curseg, bk;
		while(cur != NULL) {
			write(d, cur->p, cur->size);
			free(cur->p);
			cur->offset = out;
			out+=cur->size;
			cur = cur->next;
		}

		// set oust offval
		imos(out);
		cur = curseg;
		while(cur != NULL) {
			out_seg(cur);
			bk = cur;
			cur = cur->next;
			__hdr->nsg++;
		}
	}

	__hdr->sg = offset-remf_seghdrsz;
	pwrite(d, __hdr, remf_hdrsz, 0);
}
