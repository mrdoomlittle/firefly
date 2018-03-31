# include "bond.h"
# include "../linux/unistd.h"
# include "../linux/fcntl.h"
# include "../linux/stat.h"
# include "../stdio.h"
# include "../ffly_def.h"
int static s;
int d;

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

regionp syt;

regionp curbin = NULL;
segmentp curseg = NULL;
hookp curhok = NULL;
symbolp cursy = NULL;
regionp curreg = NULL;
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

	symbolp p;
	if ((p = (symbolp)hash_get(&symbols, name, __sy->l-1)) != NULL) {
		printf("symbol already exists in table.\n");
		goto _sk;
	}

	p = (symbolp)malloc(sizeof(struct symbol));
	p->next = cursy;
	cursy = p;
	hash_put(&symbols, name, __sy->l-1, p);
	_sk:
	p->loc = __sy->loc;
}

void absorb_hook(ffef_hokp __hook) {
	hookp p = (hookp)malloc(sizeof(struct hook));
	p->next = curhok;
	curhok = p;

	p->offset = __hook->offset+curbin->offset;
	ffef_syp sy = (ffef_syp)(syt->beg+__hook->to);

	char name[128];
	memcpy(name, stte-sy->name, sy->l);
	p->to = hash_get(&symbols, name, sy->l-1);	
}

void absorb_segment(ffef_seg_hdrp __seg) {
	segmentp seg = (segmentp)malloc(sizeof(struct segment));	
	seg->next = curseg;
	curseg = seg;

	seg->p = (mdl_u8_t*)malloc(seg->size = __seg->sz);
	seg->addr = __seg->adr;
	lseek(d, __seg->offset, SEEK_SET);
	read(d, seg->p, __seg->sz);
}

void absorb_region(ffef_reg_hdrp __reg) {
	regionp reg = (regionp)malloc(sizeof(struct region));
	if (__reg->type == FF_RG_PROG) {
		reg->next = curbin;
		curbin = reg;
	} else {
		reg->next = curreg;
		curreg = reg;
	}

	mdl_uint_t size;
	reg->beg = (mdl_u8_t*)malloc(size = (__reg->end-__reg->beg));
	reg->end = reg->beg+size;
	reg->offset = __reg->beg;

	lseek(s, __reg->beg, SEEK_SET);
	read(s, reg->beg, size);

	if (__reg->type == FF_RG_SYT) {
		syt = reg;
		ffef_syp sy = (ffef_syp)reg->beg;
		while(sy != (ffef_syp)reg->end)
			absorb_symbol(sy++);
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
		mdl_uint_t i;
		mdl_u64_t offset = hdr.rg;
		for(i = 0;i != hdr.nrg;i++,offset-=ffef_reg_hdrsz+reg.l) {
			lseek(s, offset, SEEK_SET);
			read(s, &reg, ffef_reg_hdrsz);
			absorb_region(&reg);	
		}
	}
/*
	struct ffef_seg_hdr seg;
	if (hdr.sg != FF_EF_NULL) {
		mdl_uint_t i;
		mdl_u64_t offset = hdr.sg;
		for(i = 0;i != hdr.nsg;i++,offset-=ffef_seg_hdrsz) {
			lseek(s, offset, SEEK_SET);
			read(s, &seg, ffef_reg_hdrsz);
			absorb_segment(&seg);
		}
	}

	struct ffef_hok hok;
	if (hdr.hk != FF_EF_NULL) {
		mdl_uint_t i;
		mdl_u64_t offset = hdr.hk;
		for(i = 0;i != hdr.nhk;i++,offset-=ffef_hoksz) {
			lseek(s, offset, SEEK_SET);
			read(s, &hok, ffef_hoksz);
			absorb_hook(&hok);
		}
	}
*/
	free(stt);
	close(s);
}

void latch_hooks() {
	hookp cur = curhok;
	while(cur != NULL) {
			
		cur = cur->next;
	}
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
	dhdr.nhk = 0;
	dhdr.sg = FF_EF_NULL;
	dhdr.rg = FF_EF_NULL;
	dhdr.rl = FF_EF_NULL;
	dhdr.hk = FF_EF_NULL;

	dhdr.sttr = FF_EF_NULL;

	mdl_i8_t epdeg = -1;
	while(*p != '\0') {
		char const *file;
		if (!(file = extsrcfl(&p)))
			break;
		process_srcfl(file, &dhdr);
	}

	output(&dhdr);

	close(d);
	hash_destroy(&symbols);
}
