# include "as.h"
# include "../dep/str_len.h"
# include "../ffly_def.h"
# include "../system/string.h"
# include "../elf.h"
# include "../stdio.h"
# include "../string.h"
# include "../malloc.h"
# include "../dep/str_cmp.h"
struct hash symbols;
struct hash defines;

char **globl;

segmentp curseg = NULL;
regionp curreg = NULL;
relocatep rel = NULL;
struct hash env;
mdl_u64_t offset = 0;

mdl_u8_t of = _of_null;

# define OBSIZE 60

struct {
	mdl_uint_t off, dst;
	mdl_u8_t p[OBSIZE], *end;
} outbuf;

void ffas_ldsrc(void) {

}

void ffas_init(void) {
	hash_init(&symbols);
	hash_init(&env);
	outbuf.end = outbuf.p+OBSIZE;
	outbuf.off = 0;
	outbuf.dst = offset;
	globl = (char**)malloc(20*sizeof(char*));
	*(globl++) = NULL;
}

void ffas_de_init(void) {
	while(*(--globl) != NULL);
	free(globl);
}

void* _memdup(void *__p, mdl_uint_t __bc) {
	void *ret = _alloca(__bc);
	mdl_u8_t *p = (mdl_u8_t*)ret;
	mdl_u8_t *end = p+__bc;
	while(p != end) {
		*p = *((mdl_u8_t*)__p+(p-(mdl_u8_t*)ret));
		p++;
	}
	return ret;
}

char* read_str(char *__p, mdl_uint_t *__len) {
	char *p = __p;
	char buf[128];
	char *bufp = buf;
	while((*p >= 'a' && *p <= 'z') | *p == '_') {
		*(bufp++) = *(p++);
	}
	*bufp = '\0';
	*__len = bufp-buf;
	return _memdup(buf, (bufp-buf)+1);
}

mdl_u64_t read_no(char *__p, mdl_uint_t *__len, mdl_u8_t *__sign) {
	char *p = __p;
	char buf[128];
	char *bufp = buf;
	if ((*__sign = (*p == '-')))
		p++;
	mdl_u8_t hex = 0;
	while(*p >= '0' && *p <= '9') {
		if (*p == 'x') {
			bufp = buf;
			p++;
			hex = 1;
		}
		*(bufp++) = *(p++);
	}

	*bufp = '\0';
	*__len = bufp-buf;

	if (hex) 
		return ffly_htint(buf);
	if (*__sign)
		return -ffly_stno(buf);
	return ffly_stno(buf);
}

# include "../linux/unistd.h"
char*
copyln(char *__dst, char *__src, char *__end, mdl_uint_t *__len) {
	char *p = __src;
	while(*p != '\n' && *p != '\0' && p < __end) {
		*(__dst+(p-__src)) = *p;
		p++;
	}
	*__len = p-__src;
	return p;
}

ffly_addr_t rgadr(char const*);
// resolve register address
void adaptreg(symbolp __sy) {
	if (!__sy) return;
	if (is_syreg(__sy)) {
		ffly_addr_t *ra = (ffly_addr_t*)_alloca(sizeof(ffly_addr_t));
		*ra = rgadr((char const*)__sy->p);
		__sy->p = ra;
	}
}

mdl_i8_t static epdeg = -1;
char const static *ep = NULL;
void
assemble(char *__p, char *__end) {
	/*
		we parse line by line
	*/
	char buf[256]; // line buffer
	char *p = __p;
	mdl_uint_t len;
	while(p < __end) {
		while ((*p == ' ' | *p == '\t' | *p == '\n') && p < __end) p++;
		if (*p == '\0') break;

		p = copyln(buf, p, __end, &len)+1;
		*(buf+len) = '\0';

		symbolp sy;
		if ((sy = parse(buf)) != NULL) {
			if (is_symac(sy)) {
				if (!ffly_str_cmp(sy->p, "define")) {
					printf("got: macro\n");
					
				}
			} else if (is_sylabel(sy)) {
				labelp la = (labelp)_alloca(sizeof(struct label));
				la->offset = offset;
				la->adr = stackadr();
				hash_put(&env, sy->p, sy->len, la);
				printf("label\n");
			} else if (is_sydir(sy)) {
				printf("directive, %s\n", sy->p);
				if (*(char*)sy->p == 'b') {
					if (curseg->offset != offset) {
						// err
					}
					oustbyte(sy->next->p);
					isa(1);
					curseg->size++;
				} else if (!strcmp(sy->p, "entry") && epdeg<0) {
					epdeg = 0;
					ep = sy->next->p;
				} else if (!strcmp(sy->p, "globl"))
					*(globl++) = sy->next->p;
				else if (!strcmp(sy->p, "segment")) {
					segmentp sg = (segmentp)_alloca(sizeof(struct segment));
					sg->name = sy->next->p;
					sg->next = curseg;
					sg->size = 0;
					sg->addr = stackadr();
					sg->offset = offset;
					curseg = sg;
				} else if (!strcmp(sy->p, "region")) {
					regionp rg = (regionp)_alloca(sizeof(struct region));
					rg->name = sy->next->p;
					rg->next = curreg;
					rg->beg = offset;
					curreg = rg;
				} else if (!strcmp(sy->p, "axe")) {
					curreg->end = offset;
				}
			} else {
				insp ins;
				if ((ins = (insp)hash_get(&env, sy->p, sy->len))) {
					if (is_sylabel(sy->next)) {
						sy->next->sort = SY_INT;
						labelp la = (labelp)hash_get(&env, sy->next->p, ffly_str_len(sy->next->p));
						*(mdl_u64_t*)(sy->next->p = _alloca(sizeof(mdl_u64_t))) = la->adr;
					} else
						adaptreg(sy->next);	
					ins->l = sy->next;
					if (sy->next != NULL) {
						adaptreg(sy->next->next);
						ins->r = sy->next->next;
					}
					ins->post(ins);
					printf("got: %s\n", ins->name);
				} else
					printf("unknown.\n");
			}
		}
	}
}

void reloc(mdl_u64_t __offset, mdl_u8_t __l) {
	relocatep rl = (relocatep)_alloca(sizeof(struct relocate));
	rl->next = rel;
	rl->l = __l;
	rl->offset = __offset;
	rel = rl;
}

# include "../ffef.h"
# include "../exec.h"
void finalize(void) {
	if (!ep)
		ep = "_start";
	printf("entry point: %s\n", ep);
	labelp entry;
	if (!(entry = (labelp)hash_get(&env, ep, ffly_str_len(ep))))
		printf("entry point not found.\n");
	if (of == _of_ffef && entry != NULL) {
		struct ffef_hdr hdr;
		*hdr.ident = FF_EF_MAG0;
		hdr.ident[1] = FF_EF_MAG1;
		hdr.ident[2] = FF_EF_MAG2;
		hdr.ident[3] = FF_EF_MAG3;
		hdr.ident[4] = '\0';
		hdr.routine = entry->offset;
		hdr.format = _ffexec_bc;
		hdr.nsg = 0;
		hdr.nrg = 0;
		hdr.nsy = 0;
		hdr.nrl = 0;
		hdr.sg = FF_EF_NULL;
		hdr.rg = FF_EF_NULL;
		hdr.sy = FF_EF_NULL;
		hdr.rl = FF_EF_NULL;
		char **cur = globl;
		struct ffef_sym_hdr sy;
		while(*(--cur) != NULL) {
			mdl_uint_t l;
			sy.name = offset;
			labelp la = (labelp)hash_get(&env, *cur, l = ffly_str_len(*cur));
			sy.l = l+1;
			sy.loc = offset;
			oust(*cur, l+1);
			oust((mdl_u8_t*)&sy, ffef_sym_hdrsz);
			hdr.nsy++;
		}
	
		if (*(globl-1) != NULL)
			hdr.sy = offset-ffef_sym_hdrsz;				
		
		segmentp sg = curseg;
		while(sg != NULL) {
			struct ffef_seg_hdr seg;
			seg.adr = sg->addr;
			seg.offset = sg->offset;
			seg.sz = sg->size;
			oust((mdl_u8_t*)&seg, ffef_seg_hdrsz);
			hdr.nsg++;
			sg = sg->next;
		}

		if (curseg != NULL)
			hdr.sg = offset-ffef_seg_hdrsz;

		regionp rg = curreg;
		while(rg != NULL) {
			struct ffef_reg_hdr reg;
			reg.l = ffly_str_len(rg->name)+1;
			reg.name = offset;
			oust(rg->name, reg.l);
			reg.beg = rg->beg;
			reg.end = rg->end;

			if (!strcmp(rg->name, "text"))
				reg.type = FF_RG_PROG;
			else
				reg.type = FF_RG_NULL;

			oust((mdl_u8_t*)&reg, ffef_reg_hdrsz);
			hdr.nrg++;
			rg = rg->next;
		}

		if (curreg != NULL)
			hdr.rg = offset-ffef_reg_hdrsz;

		relocatep rl = rel;
		while(rl != NULL) {
			struct ffef_rel rel;
			rel.offset = rl->offset;
			rel.l = rl->l;
			oust(&rel, ffef_relsz);
			hdr.nrl++;
			rl = rl->next;
		}

		if (rel != NULL)
			hdr.rl = offset-ffef_relsz;

		lseek(out, 0, SEEK_SET);
		write(out, &hdr, ffef_hdr_size);
	}

	if (outbuf.off>0) {
		lseek(out, outbuf.dst, SEEK_SET);
		write(out, outbuf.p, outbuf.off);
	}
}

void oust(mdl_u8_t *__p, mdl_u8_t __n) {
	if (__n > OBSIZE) {
		printf("error: cant oust anything larger then %u\n", OBSIZE);
		return;
	}

	mdl_int_t overflow;
	if ((overflow = (mdl_int_t)(outbuf.off+__n)-(mdl_int_t)((outbuf.end-outbuf.p)-1))>0)
		__n-=overflow;

	ffly_mem_cpy(outbuf.p+outbuf.off, __p, __n);
	offset+=__n;
	outbuf.off+=__n;

	if (outbuf.p+outbuf.off == outbuf.end-1) {
		lseek(out, outbuf.dst, SEEK_SET);
		write(out, outbuf.p, outbuf.off);
		outbuf.off = 0;
		outbuf.dst = offset;
	}

	if (overflow>0) {
		mdl_u8_t *p = __p+__n;
		mdl_u8_t *end = p+overflow;
		while(p != end)
			oust(p++, 1);
	}
}

void oustbyte(mdl_u8_t __byte) {
	oust(&__byte, 1);
}

void oust_16l(mdl_u16_t __data) {
	oust((mdl_u8_t*)&__data, 2);
}

void oust_32l(mdl_u32_t __data) {
	oust((mdl_u8_t*)&__data, 4);
}

void oust_64l(mdl_u64_t __data) {
	oust((mdl_u8_t*)&__data, 8);
}

void load(insp* __list) {
	insp* p = __list;
	char *name;
	while(*p != NULL) {
		name = (*p)->name;
		hash_put(&env, name, ffly_str_len(name), *p);
		p++;
	}
}
