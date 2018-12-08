# include "bond.h"
# include "../linux/unistd.h"
# include "../linux/fcntl.h"
# include "../linux/stat.h"
# include "../stdio.h"
# include "../ffly_def.h"
int static s;
int d;

ff_u32_t adr = 0;
void static
iadr(ff_uint_t __by) {
	adr+=__by;
}

ff_u32_t static curadr() {return adr;}
struct hash symbols;

char const static*
extsrcfl(char const **__p) {
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

ff_i8_t static
validate(remf_hdrp __hdr) {
	if (*__hdr->ident != FF_REMF_MAG0) {
		printf("mag0 corrupted\n"); //<- add somthing else other then corrupted 
		return -1;
	}

	if (__hdr->ident[1] != FF_REMF_MAG1) {
		printf("mag1 corrupted\n");
		return -1;
	}

	if (__hdr->ident[2] != FF_REMF_MAG2) {
		printf("mag2 corrupted\n");
		return -1;
	}

	if (__hdr->ident[3] != FF_REMF_MAG3) {
		printf("mag3 corrupted\n");
		return -1;
	}
	return 0;
}

# include "../malloc.h"
# include "../dep/bzero.h"
ff_uint_t offset = remf_hdrsz;
ff_uint_t static
get_offset() {
	return offset;
}

void static
incr_offset(ff_uint_t __by) {
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
void static **map = NULL;
ff_uint_t page_c = 0;

void static *tf[1024];
void static **fresh = tf;
void static to_free(void *__p) {
	if (fresh-tf >= 1024) {
		printf("error overflow.\n");
	}
	*(fresh++) = __p;
}

ff_u64_t bot = 0;
ff_u64_t rise = 0;
ff_uint_t fbt = 0;

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

		if (!map[page]) {
			printf("error null page.\n");
			return;
		}

		memcpy(map[page]+pg_off, p, shred);
		p+=shred;
	}
}

void bond_mapout(ff_u64_t __offset, ff_uint_t __size) {
	ff_uint_t page = __offset>>PAGE_SHIFT;
	ff_uint_t pg_off = __offset-(page*PAGE_SIZE);
	ff_u64_t end = __size+__offset;
	ff_uint_t pg_c = ((end>>PAGE_SHIFT)+((end-((end>>PAGE_SHIFT)*PAGE_SIZE))>0));

	if (!map) {
		map = (void**)malloc(pg_c*sizeof(void*));
		ff_uint_t cur = 0;
		while(cur != pg_c) {
			map[cur++] = NULL;
			printf("page: %u, %p\n", cur, map[cur-1]);
		}
		page_c = pg_c;
	} else {
		if (pg_c>page_c) {
			map = (void**)realloc(map, pg_c*sizeof(void*));
			ff_uint_t cur = page_c;
			while(cur != pg_c)
				map[cur++] = NULL;
			page_c = pg_c;
		}
	}

	printf("pg_c: %u\n", pg_c);
	while(page != pg_c) {
		printf("page: %u\n", page);
		if (!map[page]) {
			printf("alloc\n");
			map[page] = malloc(PAGE_SIZE);
		} else {
			printf("page not null.\n");
		}
		page++;
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
		if (!map[page]) {
			printf("error null page.\n");
			return;
		}
		memcpy(p, map[page]+pg_off, shred);
		p+=shred;
	}
}

void
bond_oust(void *__p, ff_uint_t __size) {
	lseek(d, offset, SEEK_SET);
	write(d, __p, __size);
	offset+=__size;
}

char const *stt, *stte;

void static
absorb_symbol(remf_syp __sy, symbolp *__stp) {
	char name[128];
	memcpy(name, stte-__sy->name, __sy->l);
	*(name+(__sy->l-1)) = '\0';
	printf("symbol: %s\n", name);

	symbolp p;
	if ((p = (symbolp)bond_hash_get(&symbols, name, __sy->l-1)) != NULL) {
		printf("symbol already exists.\n");
		if (__sy->type == FF_SY_IND)
			goto _sk1;
		goto _sk0;
	}

	p = (symbolp)malloc(sizeof(struct symbol));
	to_free(p);
	p->next = cursy;
	cursy = p;

	bond_hash_put(&symbols, name, __sy->l-1, p);
	p->name = strdup(name);
_sk0:
	p->loc = curadr()+__sy->loc;
	p->type = __sy->type;
	p->reg = rindx+__sy->reg;
	p->f = fbt+__sy->f;
_sk1:
	*__stp = p;
}

void static 
absorb_hook(remf_hokp __hook) {
	hookp p = (hookp)malloc(sizeof(struct hook));
	to_free(p);
	p->next = curhok;
	curhok = p;

	p->offset = bot+__hook->offset;
	p->to = *(syt-__hook->to);	
	p->l = __hook->l;
	p->adr = curadr()+__hook->adr;
	p->f = fbt+__hook->f;
}

void static
absorb_segment(remf_seg_hdrp __seg) {
	segmentp seg = (segmentp)malloc(sizeof(struct segment));	
	seg->next = curseg;
	curseg = seg;

	seg->p = (ff_u8_t*)malloc(seg->size = __seg->sz);
	seg->addr = __seg->adr;
	lseek(d, __seg->offset, SEEK_SET);
	read(d, seg->p, __seg->sz);
}

# include "../hexdump.h"
void static
absorb_region(remf_reg_hdrp __reg) {
	char name[128];
	lseek(s, __reg->name, SEEK_SET);
	read(s, name, __reg->l);
	printf("region, %s\n", name);

	ff_uint_t size;
	ff_u8_t *buf = (ff_u8_t*)malloc(size = __reg->size);
	printf("region size: %u\n", size);

	lseek(s, __reg->offset, SEEK_SET);
	read(s, buf, size);

	if (__reg->type == FF_RG_SYT) {
		ff_uint_t l;
		syt = (symbolp*)malloc((l = (size/sizeof(struct remf_sy)))*sizeof(symbolp));
		to_free(syt);
		symbolp *p = syt;
		syt+=(l-1);
		remf_syp sy = (remf_syp)buf;
		while(sy != (remf_syp)(buf+size))
			absorb_symbol(sy++, p++);
		free(buf);
		return;
	}

	regionp reg;
	
	reg = (regionp)malloc(sizeof(struct region));
	to_free(reg);
	reg->offset = bot+rise;
	reg->size = __reg->size;
	reg->fs = fbt+__reg->fs;
	reg->nf = __reg->nf;
	if (__reg->type == FF_RG_PROG) {
		reg->next = curbin;
		curbin = reg;
		rise+=__reg->size;
		printf("region placed at: %u\n", reg->offset);
		reg->adr = __reg->adr+curadr();
		ffly_hexdump(buf, size);
	} else {
		reg->next = curreg;
		curreg = reg;
	}

	if (__reg->type == FF_RG_PROG) {
		bond_mapout(reg->offset, size);
		bond_write(reg->offset, buf, size);
	}

	free(buf);
}

void static
absorb_relocate(remf_relp __rel) {
	relocatep rel = (relocatep)malloc(sizeof(struct relocate));
	to_free(rel);
	rel->next = currel;
	currel = rel;
	rel->offset = bot+__rel->offset;
	rel->l = __rel->l;
	rel->addto = __rel->addto;
	rel->adr = curadr()+__rel->adr;
	rel->sy = *(syt-__rel->sy);
	rel->f = fbt+__rel->f;
	printf("reloc: symbol, %s\n", rel->sy->name);
}

// load string table
void static
ldstt(remf_hdrp __hdr) {
	struct remf_reg_hdr reg;
	lseek(s, __hdr->sttr, SEEK_SET);
	read(s, &reg, remf_reghdrsz);

	ff_uint_t size;
	stt = (char const*)malloc(size = reg.size);
	lseek(s, reg.offset, SEEK_SET);
	read(s, (void*)stt, size);
	stte = stt+size;
}


void lif(ff_u32_t __ft) {
	struct remf_ft ft;
	pread(s, &ft, sizeof(struct remf_ft), __ft);

	struct remf_frag *fr, *f;
	fr = (struct remf_frag*)malloc(ft.n*sizeof(struct remf_frag));

	printf("number of fragments: %u\n", ft.n);
	pread(s, fr, ft.n*sizeof(struct remf_frag), ft.array-((ft.n-1)*sizeof(struct remf_frag)));
	ff_uint_t i;
	i = 0;
	while(i != ft.n) {
		f = fr+(i++);
		printf("fragment; id: %u, src: %u, sz: %u\n", f->id, f->src, f->size);
		bond_fnew(f->src, f->size, f->id);
	}

	fbt+=ft.n;
	free(fr);
}

ff_i8_t static epdeg = -1;
void static
process_srcfl(char const *__file, remf_hdrp __dhdr) {
	if ((s = open(__file, O_RDONLY, 0)) == -1) {
		printf("failed to open source file.\n");
		return;
	}

	struct remf_hdr hdr;
	read(s, &hdr, remf_hdrsz);
	if (validate(&hdr) == -1) {
		printf("%s: validation failed remf header not pressent or is corrupted, skipping file.\n", __file);
		return;
	}

	if (hdr.routine != FF_REMF_NULL) {
		if (!epdeg) {
			printf("entry point already designated, skipping.\n");
			return;
		}
		__dhdr->routine = curadr()+hdr.routine;
	}

	ldstt(&hdr);
	struct remf_reg_hdr reg;
	if (hdr.rg != FF_REMF_NULL) {
		ff_uint_t i;
		ff_u64_t offset = hdr.rg;
//		to_free(rindx = (regionp*)malloc(hdr.nrg*sizeof(regionp)));
		for(i = 0;i != hdr.nrg;i++,offset-=remf_reghdrsz+reg.l) {
			lseek(s, offset, SEEK_SET);
			read(s, &reg, remf_reghdrsz);
			absorb_region(&reg);
//			rindx[i] = curreg;
		}
	}

/*
	struct remf_seg_hdr seg;
	if (hdr.sg != FF_EF_NULL) {
		ff_uint_t i;
		ff_u64_t offset = hdr.sg;
		for(i = 0;i != hdr.nsg;i++,offset-=remf_seghdrsz) {
			lseek(s, offset, SEEK_SET);
			read(s, &seg, remf_reg_hdrsz); <- dont know why reghdrsz
			absorb_segment(&seg);
		}
	}
*/

	struct remf_rel rel;
	if (hdr.rl != FF_REMF_NULL) {
		ff_uint_t i = 0;
		ff_u64_t offset = hdr.rl;
		for(i = 0;i != hdr.nrl;i++,offset-=remf_relsz) {
			lseek(s, offset, SEEK_SET);
			read(s, &rel, remf_relsz);
			absorb_relocate(&rel);
		}
	}

	struct remf_hok hok;
	if (hdr.hk != FF_REMF_NULL) {
		ff_uint_t i;
		ff_u64_t offset = hdr.hk;
		for(i = 0;i != hdr.nhk;i++,offset-=remf_hoksz) {
			lseek(s, offset, SEEK_SET);
			read(s, &hok, remf_hoksz);
			absorb_hook(&hok);
		}
	}

	lif(hdr.ft);
	bot+=rise;
	rise=0;
	iadr(hdr.adr);
	free(stt);
	close(s);
}

struct fix {
	struct fix *next;
	void *arg;
	ff_u8_t type;
	ff_u8_t flags;
	struct frag *f;
	ff_uint_t addto;
};

enum {
	_fx_dis
};

struct fix *fx_head = NULL;
void fix(ff_uint_t __f, void *__arg, ff_u8_t __type, ff_u8_t __flags, ff_uint_t __addto) {
	struct fix *fx = (struct fix*)malloc(sizeof(struct fix));
	fx->f = bond_fbn(__f);
	fx->arg = __arg;
	fx->type = __type;
	fx->flags = __flags;
	fx->addto = __addto;
	fx->next = fx_head;
	fx_head = fx;
}

ff_uint_t static __n(long long __val) {
	ff_uint_t i;
	i = 0;
	while((__val&(0xffffffffffffffff<<i))>0)
		i++;
	return i;
}


ff_u8_t fix_flgs = 0x00;
void static
_fixins(struct fix *__fx) {
	printf("^^^^^^^^^^^^^^^fix.\n");
	if (__fx->type == _fx_dis) {
		symbolp sy = (symbolp)__fx->arg;
		struct frag *sf;
		sf = bond_fbn(sy->f);
		ff_int_t dis;
		printf("----> fix: %u, %u\n", sf->m, __fx->f->m);
		dis = ((sf->adr+sf->m)+(sy->loc-sf->adr)+__fx->addto)-(__fx->f->adr+__fx->f->m+__fx->f->size);
		printf("dis: %d\n", dis);
		ff_uint_t n;
		n = __n(dis<0?-dis:dis);
		ff_uint_t bs;
		ff_int_t *bsp;
		bsp = &__fx->f->bs;
		bs = ((n+1)+((1<<4)-1))>>3;

		*(ff_i64_t*)__fx->f->data = dis-bs;

		printf("before %u, after %u\n", *bsp, bs);
		if (*bsp != bs)
			fix_flgs |= 0x01;
		*bsp+=(bs-*bsp);
		return;
	}
	fix_flgs = 0x00;
}

void latch_hooks(void) {
	hookp cur = curhok;
	while(cur != NULL) {
		if (!cur->to)
			printf("cant hook onto a symbol that doesen't exist.\n");
		else {		
			if (cur->to->type == FF_SY_GBL) {
				fix(cur->f, cur->to, _fx_dis, 0x00, 0);
//				ff_i64_t loc = ((ff_i64_t)cur->to->loc)-(ff_i64_t)cur->adr;
//				bond_write(cur->offset, &loc, cur->l);	
				printf("hooking at: %u\n", cur->offset);
			} else
				printf("symbol hasen't been defined.\n");
		}
		cur = cur->next;
	}
}

void reloc(void) {
	relocatep cur = currel;
	while(cur != NULL) {
		fix(cur->f, cur->sy, _fx_dis, 0x00, cur->addto);
//		ff_i64_t loc = ((ff_i64_t)(cur->sy->loc+cur->addto))-(ff_i64_t)cur->adr;
//		bond_write(cur->offset, &loc, cur->l);	
		cur = cur->next;
	}
}

void static 
cleanup(void) {
	void **cur = tf;
	while(cur != fresh)
		free(*(cur++));
	if (map != NULL) {
		void **page = map;
		while(page != map+page_c) {
			if (*page != NULL)
				free(*page);
			page++;
		}
		free(map);
	}
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
	bond_hash_init(&symbols);
	char const *p = __s;

	if ((d = open(__dst, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU)) == -1) {
		// error
		return;
	}

	struct remf_hdr dhdr;
	ffly_bzero(&dhdr, remf_hdrsz);

	*dhdr.ident = FF_REMF_MAG0;
	dhdr.ident[1] = FF_REMF_MAG1;
	dhdr.ident[2] = FF_REMF_MAG2;
	dhdr.ident[3] = FF_REMF_MAG3;

	dhdr.nsg = 0;
	dhdr.nrg = 0;
	dhdr.nrl = 0;
	dhdr.nhk = 0;
	dhdr.sg = FF_REMF_NULL;
	dhdr.rg = FF_REMF_NULL;
	dhdr.rl = FF_REMF_NULL;
	dhdr.hk = FF_REMF_NULL;
	dhdr.ft = FF_REMF_NULL;
	dhdr.sttr = FF_REMF_NULL;

	ff_i8_t epdeg = -1;
	while(*p != '\0') {
		char const *file;
		if (!(file = extsrcfl(&p)))
			break;
		process_srcfl(file, &dhdr);
	}

	latch_hooks();
	reloc();

	ff_uint_t bs;
	struct frag *f;
	struct fix *fx;

_again:
	bs = 0;
	fix_flgs = 0x00;
	fx = fx_head;
	while(fx != NULL) {
		_fixins(fx);
		printf("fix: BS: %u, M: %u\n", fx->f->bs, fx->f->m);
		fx = fx->next;
	}

	f = fr_head;
	while(f != NULL) {
		f->m = bs;
		bs+=f->bs;
		printf("frag-%u: BS: %u, M:%u\n", f->f, f->bs, f->m);

		f = f->next;
	}

	if (fix_flgs&0x01) {
		printf("#################change.\n");
		goto _again;
	}

	f = fr_head;
	while(f != NULL) {
		f->adr+=f->m;
		f = f->next;
	}

	offset+=bot;
	bond_output(&dhdr);
	cleanup();
	close(d);
	bond_hash_destroy(&symbols);
}
