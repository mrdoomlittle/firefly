# include "exec.h"
void ffexec(void *__p, void *__end, ff_u8_t __format, void(*__prep)(void*, void*), void *__hdr, ff_u32_t __entry) {
	switch(__format) {
		case _ffexec_bc:
			ffres_exec(__p, __end, __prep, __hdr, __entry);
		break;
	}
}

/*
	not finished
*/

# include "linux/unistd.h"
# include "linux/stat.h"
# include "linux/fcntl.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "ffef.h"
# include "system/io.h"
# include "resin.h"
# include "rdm.h"
# include "system/nanosleep.h"
int static fd;
typedef struct segment {
	struct segment *next;
	struct ffef_seg_hdr hdr;
} *segmentp;

// stack segments
static segmentp ss = NULL;

void static
prep(void *__hdr, void *__ctx) {
	ffef_hdrp hdr = (ffef_hdrp)__hdr;
	ffly_resinp ctx = (ffly_resinp)__ctx;

	if (ss != NULL)
		ffly_printf("loading stack segment/s.\n");
	else
		ffly_printf("no stack segment/s to load.\n");
	segmentp cur = ss, bk;
	while(cur != NULL) {
		ffly_printf("...\n");
		ff_u8_t *seg;	
		if (!(seg = (ff_u8_t*)__ffly_mem_alloc(cur->hdr.sz))) {
			// err
		}

		lseek(fd, cur->hdr.offset, SEEK_SET);
		read(fd, seg, cur->hdr.sz);

		ff_resin_sst(ctx, seg, cur->hdr.adr, cur->hdr.sz);	
		__ffly_mem_free(seg);
		bk = cur;
		cur = cur->next;
		__ffly_mem_free(bk);
	}
}

void ffexecf(char const *__file) {
	if ((fd = open(__file, O_RDONLY, 0)) == -1) {
		ffly_fprintf(ffly_err, "failed to open file, %s\n", __file);
		return;
	}

	struct stat st;
	fstat(fd, &st);

	struct ffef_hdr hdr;
	read(fd, &hdr, ffef_hdr_size);

	if (*hdr.ident != FF_EF_MAG0) {
		ffly_printf("ffexec, mag0 corrupted\n");
		goto _corrupt;
	}

	if (hdr.ident[1] != FF_EF_MAG1) {
		ffly_printf("ffexec, mag1 corrupted\n");
		goto _corrupt;
	}

	if (hdr.ident[2] != FF_EF_MAG2) {
		ffly_printf("ffexec, mag2 corrupted\n");
		goto _corrupt;
	}

	if (hdr.ident[3] != FF_EF_MAG3) {
		ffly_printf("ffexec, mag3 corrupted\n");
		goto _corrupt;
	}

	// program segments
	segmentp ps = NULL;
	ff_uint_t size = 0;

	segmentp seg;
	if (hdr.sg != FF_EF_NULL) {
		ff_uint_t i = 0;
		ff_u64_t offset = hdr.sg;
	_again:		
		seg = (segmentp)__ffly_mem_alloc(sizeof(struct segment));
		lseek(fd, offset, SEEK_SET);
		read(fd, &seg->hdr, ffef_seg_hdrsz);
		if (seg->hdr.type == FF_SG_STACK) {
			seg->next = ss;	
			ss = seg;
			ffly_printf("stack segment.\n");
		} else if (seg->hdr.type == FF_SG_PROG) {
			seg->next = ps;
			ps = seg;
			size+=seg->hdr.sz;
			ffly_printf("program segment.\n");
		} else {
			ffly_printf("unknown segment.\n");
			__ffly_mem_free(seg);
		}

		if (++i != hdr.nsg) {
			offset-=ffef_seg_hdrsz;
			goto _again;
		}	
	}

	ff_u8_t *bin, *end;
	if (!(bin = (ff_u8_t*)__ffly_mem_alloc(size))) {
		// error
	}

	segmentp cur = ps, bk;
	while(cur != NULL) {
		lseek(fd, cur->hdr.offset, SEEK_SET);
		read(fd, bin+cur->hdr.adr, cur->hdr.sz);
		bk = cur;
		cur = cur->next;
		__ffly_mem_free(bk);
	}

	end = bin+size;
# ifdef __ffly_debug
	ffly_rdm(bin, end);
# endif
	ffly_printf("routine at: %u\n", hdr.routine);
	ffexec(bin, end, hdr.format, prep, &hdr, hdr.routine);
	__ffly_mem_free(bin);
_corrupt:
	close(fd);

}
