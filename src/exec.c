# include "exec.h"
void ffexec(void *__p, void *__end, mdl_u8_t __format, void(*__prep)(void*, void*), void *__hdr) {
	switch(__format) {
		case _ffexec_bc:
			ffbci_exec(__p, __end, __prep, __hdr);	
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
# include "bci.h"
int static fd;
void static
prep(void *__hdr, void *__ctx) {
	ffef_hdrp hdr = (ffef_hdrp)__hdr;
	ffly_bcip ctx = (ffly_bcip)__ctx;

	if (hdr->sg == FF_EF_NULL) return;

	struct ffef_seg_hdr sgh;
	lseek(fd, hdr->sg, SEEK_SET);
	read(fd, &sgh, ffef_seg_hdrsz);

	mdl_u8_t *seg = (mdl_u8_t*)__ffly_mem_alloc(sgh.sz);
	lseek(fd, sgh.offset, SEEK_SET);
	read(fd, seg, sgh.sz);

	ffly_bci_sst(ctx, seg, sgh.adr, sgh.sz);	

	__ffly_mem_free(seg);
}

void ffexecf(char const *__file) {
	fd = open(__file, O_RDONLY, 0);

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

	struct ffef_reg_hdr reg;
	if (hdr.rg != FF_EF_NULL) {
		mdl_uint_t i = 0;
		mdl_u64_t offset = hdr.rg;
		_again:
		lseek(fd, offset, SEEK_SET);
		read(fd, &reg, ffef_reg_hdrsz);
		if (i >= hdr.nrg) {
			ffly_printf("missing .text region.\n");
			close(fd);
			return;
		}

		if (reg.type != FF_RG_PROG) {
			offset-=ffef_reg_hdrsz+reg.l;
			i++;
			goto _again;
		}
	}


	mdl_u8_t *bin, *end;
	if (!(bin = (mdl_u8_t*)__ffly_mem_alloc(reg.end-reg.beg))) {
		// error
	}

	end = bin+(reg.end-reg.beg);

	lseek(fd, hdr.routine, SEEK_SET);
	read(fd, bin, reg.end-reg.beg);

	ffexec(bin, end, hdr.format, prep, &hdr);
	__ffly_mem_free(bin);
	_corrupt:
	close(fd);

}
