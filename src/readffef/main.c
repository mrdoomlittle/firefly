# include "../ffef.h"
# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <malloc.h>
# include <stdio.h>
int fd;

char const* regtype_s(mdl_u8_t __type) {
	switch(__type) {
		case FF_RG_NULL: return "null";
		case FF_RG_PROG: return "program";
	}


	return "unknown";
}

void prhdr(ffef_hdrp __hdr) {
	char *i = __hdr->ident;
	printf("ident: ");
	while(i != __hdr->ident+FF_EF_IL-1 && *(i+1) != '\0')
		printf("%c, ", *(i++));
	printf("%c\n", *i);	

	printf("entry, at: %u\n", __hdr->routine);
}

void prsy(ffef_sym_hdrp __sy) {
	char *name = (char*)malloc(__sy->l);
	lseek(fd, __sy->name, SEEK_SET);
	read(fd, name, __sy->l);

	printf("symbol, name: %s, loc: %u\n", name, __sy->loc);

	free(name);
}

void prseg(ffef_seg_hdrp __seg) {
	printf("segment, adr: %u, offset: %u, size: %u\n", __seg->adr, __seg->offset, __seg->sz);
}

void prreg(ffef_reg_hdrp __reg) {
	char *name = (char*)malloc(__reg->l);
	lseek(fd, __reg->name, SEEK_SET);
	read(fd, name, __reg->l);

	printf("region, name: %s, beg: %u, end: %u, type: %s\n", name, __reg->beg, __reg->end, regtype_s(__reg->type));
}

void prrel(ffef_relp __rel) {
	printf("relocate, offset: %u, length: %u\n", __rel->offset, __rel->l);
}

int main(int __argc, char const *__argv[]) {
	fd = open(__argv[1], O_RDONLY);

	struct ffef_hdr hdr;
	read(fd, &hdr, ffef_hdr_size);

	prhdr(&hdr);
	
	struct ffef_sym_hdr sy;
	if (hdr.sy != FF_EF_NULL) {
		mdl_uint_t i = 0;
		mdl_u64_t offset = hdr.sy;
		while(i != hdr.nsy) {
			lseek(fd, offset, SEEK_SET);
			read(fd, &sy, ffef_sym_hdrsz);
			prsy(&sy);				
			offset-=ffef_sym_hdrsz+sy.l;
			i++;
		}
	} else
		printf("no symbols present.\n");

	struct ffef_seg_hdr seg;
	if (hdr.sg != FF_EF_NULL) {
		mdl_uint_t i = 0;
		mdl_u64_t offset = hdr.sg;
		while(i != hdr.nsg) {
			lseek(fd, offset, SEEK_SET);
			read(fd, &seg, ffef_seg_hdrsz);
			prseg(&seg);
			offset-=ffef_seg_hdrsz;
			i++;
		}
	} else
		printf("no segments present.\n");

	struct ffef_reg_hdr reg;
	if (hdr.rg != FF_EF_NULL) {
		mdl_uint_t i = 0;
		mdl_u64_t offset = hdr.rg;
		while(i != hdr.nrg) {
			lseek(fd, offset, SEEK_SET);
			read(fd, &reg, ffef_reg_hdrsz);
			prreg(&reg);
			offset-=ffef_reg_hdrsz+reg.l;
			i++;
		}
	} else
		printf("no regions present.\n");

	struct ffef_rel rel;
	if (hdr.rl != FF_EF_NULL) {
		mdl_uint_t i = 0;
		mdl_u64_t offset = hdr.rl;
		while(i != hdr.nrl) {
			lseek(fd, offset, SEEK_SET);
			read(fd, &rel, ffef_relsz);
			prrel(&rel);
			offset-=ffef_relsz;
			i++;
		}
	} else
		printf("no rel present.\n");

	close(fd);

}
