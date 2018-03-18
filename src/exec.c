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
	return; // ignore for now
	ffef_hdrp hdr = (ffef_hdrp)__hdr;
	ffly_bcip ctx = (ffly_bcip)__ctx;

	struct ffef_seg_hdr sgh;
	lseek(fd, hdr->sg, SEEK_SET);
	read(fd, &sgh, ffef_seg_hdrsz);

	mdl_u8_t *seg = (mdl_u8_t*)__ffly_mem_alloc(sgh.memsz);
	lseek(fd, sgh.offset, SEEK_SET);
	read(fd, seg, sgh.memsz);

	ffly_bci_sst(ctx, seg, sgh.adr, sgh.memsz);	

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

	mdl_u8_t *bin, *end;
	if (!(bin = (mdl_u8_t*)__ffly_mem_alloc(hdr.end-hdr.routine))) {
		// error
	}

	end = bin+(hdr.end-hdr.routine);

	lseek(fd, hdr.routine, SEEK_SET);
	read(fd, bin, hdr.end-hdr.routine);

	ffexec(bin, end, hdr.format, prep, &hdr);
	__ffly_mem_free(bin);
	_corrupt:
	close(fd);
}
