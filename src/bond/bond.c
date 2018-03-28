# include "bond.h"
# include "../linux/unistd.h"
# include "../linux/fcntl.h"
# include "../linux/stat.h"
# include "../stdio.h"
# include "../ffly_def.h"
int static s;
int static d;

struct hash symbols;

char const *extsrcfl(char const **__p) {
	char static buf[128];
	char *bufp = buf;

	char const *p;
	if (*(p = *__p) == '\0')
		return NULL;

	while(*p == ' ') p++;

	while(*p != ' ' && *p != '\0')
		*(bufp++) = *(p++);
	*__p = p;
	*bufp = '\0';

	return (char const*)buf;
}

mdl_i8_t validate(ffef_hdrp __hdr) {
	if (*__hdr->ident != FF_EF_MAG0) {
		printf("mag0 corrupted\n");
		return -1;
	}

	if (__hdr->ident[1] != FF_EF_MAG1) {
		printf("mag1 corrupted\n");
		return -1;
	}

	if (__hdr->ident[2] != FF_EF_MAG2) {
		printf("mag2 corrupted\n");
		return -1;
	}

	if (__hdr->ident[3] != FF_EF_MAG3) {
		printf("mag3 corrupted\n");
		return -1;
	}
	return 0;
}

# include "../malloc.h"
# include "../dep/bzero.h"
mdl_uint_t offset = ffef_hdr_size;
mdl_uint_t get_offset() {
	return offset;
}

void incr_offset(mdl_uint_t __by) {
	offset+=__by;
}

# include "../string.h"
typedef struct segment {
	struct segment *next;
} *segmentp;

typedef struct region {
	struct region *next;
} *regionp;

typedef struct relocate {

} *relocatep;

typedef struct hook {
	struct hook *next;	
	mdl_uint_t offset;
} *hookp;

typedef struct symbol {
	struct symbol *next;
} *symbolp;

typedef struct bin {
	struct bin *next;
	mdl_u8_t *p, *end;
	mdl_uint_t offset;
} *binp;

binp curbin = NULL;

hookp curhok = NULL;
symbolp cursy = NULL;

void oust(void *__p, mdl_uint_t __size) {
	lseek(d, offset, SEEK_SET);
	write(d, __p, __size);
	offset+=__size;
}

char const *stt, *stte;

void absorb_symbol(ffef_syp __sy) {
	char name[128];
	memcpy(name, stte-__sy->name, __sy->l);
	printf("symbol: %s\n", name);

	symbolp p = (symbolp)malloc(sizeof(struct symbol));
	p->next = cursy;
	cursy = p;
	hash_put(&symbols, name, __sy->l-1, p);
}

void absorb_hook(ffef_hokp __hook) {
	hookp p = (hookp)malloc(sizeof(struct hook));
	p->next = curhok;
	curhok = p;

	p->offset = __hook->offset+curbin->offset;

}

void absorb_segment() {

}

void absorb_region(ffef_reg_hdrp __reg) {
	if (__reg->type == FF_RG_PROG) {
		binp p = (binp)malloc(sizeof(struct bin));
		p->next = curbin;

		mdl_uint_t size;
		p->p = (mdl_u8_t*)malloc(size = (__reg->end-__reg->beg));
		p->end = p->p+size;
		p->offset = offset;
		offset+=size;

		lseek(s, __reg->beg, SEEK_SET);
		read(s, p->p, size);
	} else if (__reg->type == FF_RG_SYT) {
		mdl_uint_t size;
		ffef_syp bed;
		ffef_syp sy = (ffef_syp)malloc(size = (__reg->end-__reg->beg));
		bed = sy;

		lseek(s, __reg->beg, SEEK_SET);
		read(s, bed, size);

		ffef_syp end = (ffef_syp)((mdl_u8_t*)sy+size);
		mdl_uint_t i = 0;
		while(sy != end)
			absorb_symbol(sy++);
		free(bed);
	}
}

void ldstt(ffef_hdrp __hdr) {
	struct ffef_reg_hdr reg;
	lseek(s, __hdr->sttr, SEEK_SET);
	read(s, &reg, ffef_reg_hdrsz);

	mdl_uint_t size;
	stt = (char const*)malloc(size = (reg.end-reg.beg));
	lseek(s, reg.beg, SEEK_SET);
	read(s, (void*)stt, size);
	stte = stt+size;
}

mdl_i8_t static epdeg = -1;

void process_srcfl(char const *__file, ffef_hdrp __dhdr) {
	if ((s = open(__file, O_RDONLY, 0)) == -1) {
		printf("failed to open source file.\n");
		return;
	}

	struct ffef_hdr hdr;
	read(s, &hdr, ffef_hdr_size);
	if (validate(&hdr) == -1) {
		printf("%s: validation failed ffef header not pressent or is corrupted, skipping.\n", __file);
		return;
	}

	if (hdr.routine != FF_EF_NULL) {
		if (!epdeg) {
			printf("entry point already designated, skipping.\n");
			return;
		}
		__dhdr->routine = (offset-ffef_hdr_size)+hdr.routine;
	}

	ldstt(&hdr);

	struct ffef_reg_hdr reg;
	if (hdr.rg != FF_EF_NULL) {
		mdl_uint_t i = 0;
		mdl_u64_t offset = hdr.rg;
		while(i != hdr.nrg) {
			lseek(s, offset, SEEK_SET);
			read(s, &reg, ffef_reg_hdrsz);
			absorb_region(&reg);	
			offset-=ffef_reg_hdrsz+reg.l;
			i++;
		}
	}

/*
	struct ffef_hok hok;
	if (hdr.hk != FF_EF_NULL) {
		mdl_uint_t i = 0;
		mdl_u64_t offset = hdr.hk;
		while(i != hdr.nhk) {
			lseek(s, offset, SEEK_SET);
			read(s, &hok, ffef_hoksz);
			absorb_hook(&hok);
			offset-=ffef_hoksz;
			i++;
		}
	}
*/
	free(stt);
	close(s);
}

void bond(char const *__s, char const *__dst) {
	hash_init(&symbols);
	char const *p = __s;

	if ((d = open(__dst, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU)) == -1) {

	}

	struct ffef_hdr dhdr;
	ffly_bzero(&dhdr, ffef_hdr_size);

	*dhdr.ident = FF_EF_MAG0;
	dhdr.ident[1] = FF_EF_MAG1;
	dhdr.ident[2] = FF_EF_MAG2;
	dhdr.ident[3] = FF_EF_MAG3;

	dhdr.nsg = 0;
	dhdr.nrg = 0;
	dhdr.nrl = 0;
	dhdr.sg = FF_EF_NULL;
	dhdr.rg = FF_EF_NULL;
	dhdr.rl = FF_EF_NULL;

	mdl_i8_t epdeg = -1;
	while(*p != '\0') {
		char const *file;
		if (!(file = extsrcfl(&p)))
			break;
		process_srcfl(file, &dhdr);
	}

	binp cur = curbin, bk;
	while(cur != NULL) {
		bk = cur;
		cur = cur->next;
		ffly_printf("%u\n", bk->offset);
		lseek(d, bk->offset, SEEK_SET);
		write(d, bk->p, (bk->end-bk->p));
		free(bk->p);
		free(bk);
	}

	close(d);
	hash_destroy(&symbols);
}
