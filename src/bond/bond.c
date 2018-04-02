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


# define PAGE_SHIFT 2
# define PAGE_SIZE (1<<PAGE_SHIFT)
void **map = NULL;

void *tf[100];
void **fresh = tf;
void to_free(void *__p) {
	if (fresh-tf >= 100) {
		printf("error overflow.\n");
	}
	*(fresh++) = __p;
}

mdl_u64_t bot = 0;

void bond_write(mdl_u64_t __offset, void *__buf, mdl_uint_t __size) {
	mdl_u8_t *p = (mdl_u8_t*)__buf;
	mdl_u8_t *end = p+__size;
	mdl_u64_t offset;
	while(p != end) {
		offset = __offset+(p-(mdl_u8_t*)__buf);

		mdl_uint_t page = offset>>PAGE_SHIFT;
		mdl_uint_t pg_off = offset-(page*PAGE_SIZE);

		mdl_uint_t lh = PAGE_SIZE-pg_off;
		if (lh>__size)
			lh = __size;

		mdl_int_t rh = (mdl_int_t)__size-(mdl_int_t)lh;
		if (lh>0) {
			memcpy(map[page]+pg_off, p, lh);
			p+=lh;
		}
		if (rh>0) {
			memcpy(map[page+1], p, rh);
			p+=rh;
		}
	}
}

void bond_mapout(mdl_u64_t __offset, mdl_uint_t __size) {
	mdl_uint_t page = __offset>>PAGE_SHIFT;
	mdl_uint_t pg_off = __offset-(page*PAGE_SIZE);
	mdl_uint_t pg_c = page+1;

	mdl_uint_t static crest = 0;
	if (!map) {
		map = (void**)malloc(pg_c*sizeof(void*));
		crest = pg_c;
	} else {
		if (pg_c>crest) {
			map = (void**)realloc(map, pg_c*sizeof(void*));
			crest = pg_c;
		}
	}

	mdl_u64_t end_off = __size+__offset;
	mdl_uint_t end = ((end_off>>PAGE_SHIFT)+((end_off-((end_off>>PAGE_SHIFT)*PAGE_SIZE))>0));
	while(page != end) {
		printf("page, alloc, %u\n", page);
		to_free(map[page++] = malloc(PAGE_SIZE));
	}
}

void bond_read(mdl_u64_t __offset, void *__buf, mdl_uint_t __size) {
	mdl_u8_t *p = (mdl_u8_t*)__buf;
	mdl_u8_t *end = p+__size;

	mdl_u64_t offset;
	while(p != end) {
		offset = __offset+(p-(mdl_u8_t*)__buf);

		mdl_uint_t page = __offset>>PAGE_SHIFT;
		mdl_uint_t pg_off = __offset-(page*PAGE_SIZE);

		mdl_uint_t lh = PAGE_SIZE-pg_off;
		if (lh>__size)
			lh = __size;

		mdl_int_t rh = (mdl_int_t)__size-(mdl_int_t)lh;
		if (lh>0) {
			memcpy(p, map[page]+pg_off, lh);
			p+=lh;
		}
		if (rh>0) {
			memcpy(p, map[page+1], rh);
			p+=rh;
		}	
	}
}

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
		printf("symbol already exists.\n");
		goto _sk;
	}

	p = (symbolp)malloc(sizeof(struct symbol));
	p->next = cursy;
	cursy = p;
	hash_put(&symbols, name, __sy->l-1, p);
	p->name = strdup(name);
_sk:
	p->loc = __sy->loc;
}

void absorb_hook(ffef_hokp __hook) {
	hookp p = (hookp)malloc(sizeof(struct hook));
	p->next = curhok;
	curhok = p;

	p->offset = bot-(bot-__hook->offset);
	ffef_syp sy = (ffef_syp)(syt->beg+__hook->to);

	char name[128];
	memcpy(name, stte-sy->name, sy->l);
	p->to = hash_get(&symbols, name, sy->l-1);	
	p->l = __hook->l;
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
	char name[128];
	lseek(s, __reg->name, SEEK_SET);
	read(s, name, __reg->l);
	printf("region, %s\n", name);


	regionp reg = (regionp)malloc(sizeof(struct region));
	if (__reg->type == FF_RG_PROG) {
		reg->next = curbin;
		curbin = reg;
		bot+=__reg->end-__reg->beg;
	} else {
		reg->next = curreg;
		curreg = reg;
	}

	mdl_uint_t size;
	reg->beg = __reg->beg;
	reg->end = __reg->end;
	mdl_u8_t *buf = (mdl_u8_t*)malloc(size = (__reg->end-__reg->beg));

	lseek(s, __reg->beg, SEEK_SET);
	read(s, buf, size);

	bond_mapout(__reg->beg, size);
	bond_write(__reg->beg, buf, size);

	if (__reg->type == FF_RG_SYT) {
		syt = reg;
		ffef_syp sy = (ffef_syp)buf;
		while(sy != (ffef_syp)(buf+size))
			absorb_symbol(sy++);
	}

	free(buf);
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
		printf("%s: validation failed ffef header not pressent or is corrupted, skipping file.\n", __file);
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

	free(stt);
	close(s);
}

void latch_hooks() {
	mdl_uint_t i = 0;
	hookp cur = curhok;
	while(cur != NULL) {
		// dont include header
		bond_write(cur->offset-ffef_hdr_size, &cur->to->loc, cur->l);	
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
	mdl_uint_t l = strlen(text)+1;
	bond_mapout(PAGE_SIZE, l);

	bond_write(PAGE_SIZE, text, l);

	char buf[100];
	bond_read(PAGE_SIZE, buf, l);
	printf("out: %s\n", buf);
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

	mdl_i8_t epdeg = -1;
	while(*p != '\0') {
		char const *file;
		if (!(file = extsrcfl(&p)))
			break;
		process_srcfl(file, &dhdr);
	}

	latch_hooks();

	output(&dhdr);

	close(d);
	hash_destroy(&symbols);
}
