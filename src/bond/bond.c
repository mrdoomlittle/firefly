# include "bond.h"
# include "../linux/unistd.h"
# include "../linux/fcntl.h"
# include "../linux/stat.h"
# include "../stdio.h"
# include "../ffly_def.h"
int static s;
int d;

ff_u32_t adr = 0;
void iadr(ff_uint_t __by) {
	adr+=__by;
}

ff_u32_t curadr() {return adr;}
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

ff_i8_t validate(ffef_hdrp __hdr) {
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
ff_uint_t offset = ffef_hdr_size;
ff_uint_t get_offset() {
	return offset;
}

void incr_offset(ff_uint_t __by) {
	offset+=__by;
}

# include "../string.h"

symbolp *syt;

regionp curbin = NULL;
segmentp curseg = NULL;
hookp curhok = NULL;
symbolp cursy = NULL;
regionp curreg = NULL;

relocatep currel = NULL;

regionp *rindx;
# define PAGE_SHIFT 2
# define PAGE_SIZE (1<<PAGE_SHIFT)
void **map = NULL;

void static *tf[100];
void static **fresh = tf;
void static to_free(void *__p) {
	if (fresh-tf >= 100) {
		printf("error overflow.\n");
	}
	*(fresh++) = __p;
}

ff_u64_t bot = 0;
ff_u64_t rise = 0;

void bond_write(ff_u64_t __offset, void *__buf, ff_uint_t __size) {
	ff_u8_t *p = (ff_u8_t*)__buf;
	ff_u8_t *end = p+__size;
	ff_u64_t offset;
	while(p != end) {
		offset = __offset+(p-(ff_u8_t*)__buf);

		ff_uint_t page = offset>>PAGE_SHIFT;
		ff_uint_t pg_off = offset-(page*PAGE_SIZE);
		ff_uint_t left = end-p;

		ff_uint_t shred = PAGE_SIZE-pg_off;
		if (shred>left)
			shred = left;

		memcpy(map[page]+pg_off, p, shred);
		p+=shred;
	}
}

void bond_mapout(ff_u64_t __offset, ff_uint_t __size) {
	ff_uint_t page = __offset>>PAGE_SHIFT;
	ff_uint_t pg_off = __offset-(page*PAGE_SIZE);
	ff_u64_t end_off = __size+__offset;
	ff_uint_t pg_c = ((end_off>>PAGE_SHIFT)+((end_off-((end_off>>PAGE_SHIFT)*PAGE_SIZE))>0));

	ff_uint_t static crest = 0;
	if (!map) {
		map = (void**)malloc(pg_c*sizeof(void*));
		crest = pg_c;
	} else {
		if (pg_c>crest) {
			map = (void**)realloc(map, pg_c*sizeof(void*));
			crest = pg_c;
		}
	}

	printf("pg_c: %u\n", pg_c);
	while(page != pg_c) {
		printf("page, alloc, %u\n", page);
		to_free(map[page++] = malloc(PAGE_SIZE));
	}
}

void bond_read(ff_u64_t __offset, void *__buf, ff_uint_t __size) {
	ff_u8_t *p = (ff_u8_t*)__buf;
	ff_u8_t *end = p+__size;

	ff_u64_t offset;
	while(p != end) {
		offset = __offset+(p-(ff_u8_t*)__buf);

		ff_uint_t page = offset>>PAGE_SHIFT;
		ff_uint_t pg_off = offset-(page*PAGE_SIZE);
		ff_uint_t left = end-p;

		ff_uint_t shred = PAGE_SIZE-pg_off;
		if (shred>left)
			shred = left;
		memcpy(p, map[page]+pg_off, shred);
		p+=shred;
	}
}

void oust(void *__p, ff_uint_t __size) {
	lseek(d, offset, SEEK_SET);
	write(d, __p, __size);
	offset+=__size;
}

char const *stt, *stte;

void absorb_symbol(ffef_syp __sy, symbolp *__stp) {
	char name[128];
	memcpy(name, stte-__sy->name, __sy->l);
	printf("symbol: %s\n", name);

	symbolp p;
	if ((p = (symbolp)hash_get(&symbols, name, __sy->l-1)) != NULL) {
		printf("symbol already exists.\n");
		goto _sk;
	}

	p = (symbolp)malloc(sizeof(struct symbol));
	p->next = cursy;
	cursy = p;
	hash_put(&symbols, name, __sy->l-1, p);
	p->name = strdup(name);
_sk:
	p->loc = curadr()+__sy->loc;
	p->type = __sy->type;
	p->reg = rindx+__sy->reg;
	*__stp = p;
}

void absorb_hook(ffef_hokp __hook) {
	hookp p = (hookp)malloc(sizeof(struct hook));
	p->next = curhok;
	curhok = p;

	p->offset = bot+__hook->offset;
	p->to = *(syt-__hook->to);	
	p->l = __hook->l;
}

void absorb_segment(ffef_seg_hdrp __seg) {
	segmentp seg = (segmentp)malloc(sizeof(struct segment));	
	seg->next = curseg;
	curseg = seg;

	seg->p = (ff_u8_t*)malloc(seg->size = __seg->sz);
	seg->addr = __seg->adr;
	lseek(d, __seg->offset, SEEK_SET);
	read(d, seg->p, __seg->sz);
}

void absorb_region(ffef_reg_hdrp __reg) {
	char name[128];
	lseek(s, __reg->name, SEEK_SET);
	read(s, name, __reg->l);
	printf("region, %s\n", name);

	ff_uint_t size;
	ff_u8_t *buf = (ff_u8_t*)malloc(size = (__reg->end-__reg->beg));

	lseek(s, __reg->beg, SEEK_SET);
	read(s, buf, size);

	if (__reg->type == FF_RG_SYT) {
		ff_uint_t l;
		syt = (symbolp*)malloc((l = (size/sizeof(struct ffef_sy)))*sizeof(symbolp));
		to_free(syt);
		symbolp *p = syt;
		syt+=(l-1);
		ffef_syp sy = (ffef_syp)buf;
		while(sy != (ffef_syp)(buf+size))
			absorb_symbol(sy++, p++);
		free(buf);
		return;
	}

	regionp reg = (regionp)malloc(sizeof(struct region));
	reg->beg = __reg->beg;
	reg->end = __reg->end;
	if (__reg->type == FF_RG_PROG) {
		reg->next = curbin;
		curbin = reg;
		reg->beg+=bot;
		reg->end+=bot;
		rise+=__reg->end-__reg->beg;
		printf("region placed at: %u\n", reg->beg);
		reg->adr = curadr();
	} else {
		reg->next = curreg;
		curreg = reg;
	}

	if (__reg->type == FF_RG_PROG) {
		bond_mapout(reg->beg, size);
		bond_write(reg->beg, buf, size);
	}

	free(buf);
}

void absorb_relocate(ffef_relp __rel) {
	relocatep rel = (relocatep)malloc(sizeof(struct relocate));
	rel->next = currel;
	currel = rel;
	rel->offset = __rel->offset;
	rel->l = __rel->l;
	rel->sy = *(syt-__rel->sy);
	printf("reloc: symbol, %s\n", rel->sy->name);
}

void ldstt(ffef_hdrp __hdr) {
	struct ffef_reg_hdr reg;
	lseek(s, __hdr->sttr, SEEK_SET);
	read(s, &reg, ffef_reg_hdrsz);

	ff_uint_t size;
	stt = (char const*)malloc(size = (reg.end-reg.beg));
	lseek(s, reg.beg, SEEK_SET);
	read(s, (void*)stt, size);
	stte = stt+size;
}

ff_i8_t static epdeg = -1;
void process_srcfl(char const *__file, ffef_hdrp __dhdr) {
	if ((s = open(__file, O_RDONLY, 0)) == -1) {
		printf("failed to open source file.\n");
		return;
	}

	struct ffef_hdr hdr;
	read(s, &hdr, ffef_hdr_size);
	if (validate(&hdr) == -1) {
		printf("%s: validation failed ffef header not pressent or is corrupted, skipping file.\n", __file);
		return;
	}

	if (hdr.routine != FF_EF_NULL) {
		if (!epdeg) {
			printf("entry point already designated, skipping.\n");
			return;
		}
		__dhdr->routine = curadr()+hdr.routine;
	}

	ldstt(&hdr);
	struct ffef_reg_hdr reg;
	if (hdr.rg != FF_EF_NULL) {
		ff_uint_t i;
		ff_u64_t offset = hdr.rg;
//		to_free(rindx = (regionp*)malloc(hdr.nrg*sizeof(regionp)));
		for(i = 0;i != hdr.nrg;i++,offset-=ffef_reg_hdrsz+reg.l) {
			lseek(s, offset, SEEK_SET);
			read(s, &reg, ffef_reg_hdrsz);
			absorb_region(&reg);
//			rindx[i] = curreg;
		}
	}

/*
	struct ffef_seg_hdr seg;
	if (hdr.sg != FF_EF_NULL) {
		ff_uint_t i;
		ff_u64_t offset = hdr.sg;
		for(i = 0;i != hdr.nsg;i++,offset-=ffef_seg_hdrsz) {
			lseek(s, offset, SEEK_SET);
			read(s, &seg, ffef_reg_hdrsz);
			absorb_segment(&seg);
		}
	}
*/

	struct ffef_rel rel;
	if (hdr.rl != FF_EF_NULL) {
		ff_uint_t i = 0;
		ff_u64_t offset = hdr.rl;
		for(i = 0;i != hdr.nrl;i++,offset-=ffef_relsz) {
			lseek(s, offset, SEEK_SET);
			read(s, &rel, ffef_relsz);
			absorb_relocate(&rel);
		}
	}

	struct ffef_hok hok;
	if (hdr.hk != FF_EF_NULL) {
		ff_uint_t i;
		ff_u64_t offset = hdr.hk;
		for(i = 0;i != hdr.nhk;i++,offset-=ffef_hoksz) {
			lseek(s, offset, SEEK_SET);
			read(s, &hok, ffef_hoksz);
			absorb_hook(&hok);
		}
	}

	bot+=rise;
	rise=0;
	iadr(hdr.adr);
	free(stt);
	close(s);
}

void latch_hooks() {
	hookp cur = curhok;
	while(cur != NULL) {
		if (!cur->to)
			printf("cant hook onto a symbol that doesen't exist.\n");
		else {		
			if (cur->to->type == FF_SY_GBL) {
				bond_write(cur->offset, &cur->to->loc, cur->l);	
				printf("hooking at: %u\n", cur->offset);
			} else
				printf("symbol hasen't been defined.\n");
		}
		cur = cur->next;
	}
}

void reloc() {
	relocatep cur = currel;
	while(cur != NULL) {
		bond_write(cur->offset, &cur->sy->loc, cur->l);	
		cur = cur->next;
	}
}


void static 
cleanup() {
	void **cur = tf;
	while(cur != fresh)
		free(*(cur++));
	if (map != NULL)
		free(map);
}

void bond(char const *__s, char const *__dst) {
/*
	printf("pagesize: %u\n", PAGE_SIZE);
	char const *text = "1/2/3/4/5/6/7/8/9/10/11/12/13/14/15/16/17/18/19/20";
	ff_uint_t l = strlen(text)+1;
	bond_mapout(0, l);

	bond_write(0, text, l);

	char buf[100];
	bond_read(0, buf, l);
	printf("out: %s\n", buf);
	cleanup();
return;
*/
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

	ff_i8_t epdeg = -1;
	while(*p != '\0') {
		char const *file;
		if (!(file = extsrcfl(&p)))
			break;
		process_srcfl(file, &dhdr);
	}

	latch_hooks();
	reloc();
	offset+=bot;
	output(&dhdr);
	cleanup();
	close(d);
	hash_destroy(&symbols);
}
