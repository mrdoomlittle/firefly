# include "../ffef.h"
# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <malloc.h>
# include <stdio.h>
int fd;

char const* regtype_s(ff_u8_t __type) {
	switch(__type) {
		case FF_RG_NULL:	return "null";
		case FF_RG_PROG:	return "program";
		case FF_RG_STT:		return "string table";
		case FF_RG_SYT:		return "symbol table";
	}

	return "unknown";
}

char const* segtype_s(ff_u8_t __type) {
	switch(__type) {
		case FF_SG_NULL: return "null";
		case FF_SG_STACK:	return "stack segment";
		case FF_SG_PROG:	return "program segment";
	}
	return "unknown";
}

char const* sytype_s(ff_u8_t __type) {
	switch(__type) {
		case FF_SY_NULL: return "null";
		case FF_SY_IND: return "ind";
		case FF_SY_GBL: return "global";
		case FF_SY_LCA: return "local";
	}
	return "unknown";
}

void prhdr(ffef_hdrp __hdr) {
	char *i = __hdr->ident;
	printf("ident: ");
	while(i != __hdr->ident+FF_EF_IL-1 && *(i+1) != '\0')
		printf("%c, ", *(i++));
	printf("%c\n", *i);	

	printf("adr: %u\n", __hdr->adr);

	if (__hdr->routine == FF_EF_NULL)
		printf("no entry point.\n");
	else
		printf("entry, at: %u\n", __hdr->routine);
}

void prsy(ffef_reg_hdrp __sttr, ffef_syp __sy, ff_uint_t __no) {
	char *name = (char*)malloc(__sy->l);
	lseek(fd, __sttr->end-__sy->name, SEEK_SET);
	read(fd, name, __sy->l);

	printf("%u: symbol,\ttype: %s,\tname: %s\t: %u,\tloc: %u,\treg: %u, len: %u\n", __no, sytype_s(__sy->type),
		name, __sy->name, __sy->loc, __sy->reg, __sy->l);
	free(name);
}

void prseg(ffef_seg_hdrp __seg, ff_uint_t __no) {
	printf("%u: segment,\ttype: %s,\tadr: %u,\toffset: %u,\tsize: %u\n", __no, segtype_s(__seg->type),
		__seg->adr, __seg->offset, __seg->sz);
}

void prreg(ffef_reg_hdrp __reg, ff_uint_t __no) {
	char *name = (char*)malloc(__reg->l);
	lseek(fd, __reg->name, SEEK_SET);
	read(fd, name, __reg->l);

	printf("%u: region,\tname: %s,\tbeg: %u,\tend: %u,\tsize: %u\ttype: %s,\t%s,\tadr: %u\n", __no, name, __reg->beg,
		__reg->end, __reg->end-__reg->beg, regtype_s(__reg->type), __reg->beg==__reg->end?"empty":"not empty", __reg->adr);
}

void prhok(ffef_hokp __hok, ff_uint_t __no) {
	printf("%u: hook,\toffset: %u\tlength: %u,\tto: %u,\tadr: %u\n", __no, __hok->offset, __hok->l, __hok->to, __hok->adr);
}

void prrel(ffef_relp __rel, ff_uint_t __no) {
	printf("%u: relocate,\toffset: %u,\tlength: %u,\tsymbol: %u,\taddto: %u,\tadr: %u\n", __no, __rel->offset, __rel->l, __rel->sy, __rel->addto, __rel->adr);
}

int main(int __argc, char const *__argv[]) {
	if (__argc<2) {
		printf("please provide file.\n");
		return -1;
	}

	if (access(__argv[1], F_OK) == -1) {
		printf("file doesen't exist.\n");
		return -1;
	}

	fd = open(__argv[1], O_RDONLY);

	struct ffef_reg_hdr sttr;

	struct ffef_hdr hdr;
	read(fd, &hdr, ffef_hdr_size);

	if (hdr.sttr == FF_EF_NULL) {
		printf("no string table pressent\n");
	} else {
		lseek(fd, hdr.sttr, SEEK_SET);
		read(fd, &sttr, ffef_reg_hdrsz);
		prreg(&sttr, 0);
	}
	prhdr(&hdr);

	struct ffef_hok hok;
	if (hdr.hk != FF_EF_NULL) {
		printf("hook, entries: %u\n", hdr.nhk);
		ff_uint_t i;
		ff_u64_t offset = hdr.hk;
		for(i = 0;i != hdr.nhk;i++,offset-=ffef_hoksz) {
			lseek(fd, offset, SEEK_SET);
			read(fd, &hok, ffef_hoksz);
			prhok(&hok, i);
		}
	} else
		printf("no hook/s present.\n");

	struct ffef_seg_hdr seg;
	if (hdr.sg != FF_EF_NULL) {
		printf("segment, entries: %u\n", hdr.nsg);
		ff_uint_t i;
		ff_u64_t offset = hdr.sg;
		for(i = 0;i != hdr.nsg;i++,offset-=ffef_seg_hdrsz) {
			lseek(fd, offset, SEEK_SET);
			read(fd, &seg, ffef_seg_hdrsz);
			prseg(&seg, i);
		}
	} else
		printf("no segment/s present.\n");

	struct ffef_reg_hdr reg;
	if (hdr.rg != FF_EF_NULL) {
		printf("region, entries: %u\n", hdr.nrg);
		ff_uint_t i;
		ff_u64_t offset = hdr.rg;
		for(i = 0;i != hdr.nrg;i++,offset-=ffef_reg_hdrsz+reg.l) {
			lseek(fd, offset, SEEK_SET);
			read(fd, &reg, ffef_reg_hdrsz);
			prreg(&reg, i);
			if (reg.type == FF_RG_SYT) {
				ff_uint_t size;
				ffef_syp bed;
				ffef_syp sy = (ffef_syp)malloc(size = (reg.end-reg.beg));
				bed = sy;

				lseek(fd, reg.beg, SEEK_SET);
				read(fd, sy, size);

				ffef_syp end = (ffef_syp)((ff_u8_t*)sy+size);
				ff_uint_t i = 0;
				while(sy != end) {
					prsy(&sttr, sy, i++);
					sy++;
				}
				free(bed);
			}
		}
	} else
		printf("no region/s present.\n");

	struct ffef_rel rel;
	if (hdr.rl != FF_EF_NULL) {
		ff_uint_t i;
		ff_u64_t offset = hdr.rl;
		for(i = 0;i != hdr.nrl;i++,offset-=ffef_relsz) {
			lseek(fd, offset, SEEK_SET);
			read(fd, &rel, ffef_relsz);
			prrel(&rel, i);
		}
	} else
		printf("no rel present.\n");

	close(fd);

}
