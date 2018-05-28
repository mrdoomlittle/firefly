# include "as.h"
# include "../dep/str_len.h"
# include "../ffly_def.h"
# include "../system/string.h"
# include "../elf.h"
# include "../stdio.h"
# include "../string.h"
# include "../malloc.h"
# include "../dep/str_cmp.h"
# include "../ffef.h"
/*
	cleanup needed
*/

struct ff_as_op const *op;
void(*post)(void(*)(void));
struct hash symbols;
struct hash defines;
struct hash env;

char const **globl;
char const **extrn;

segmentp curseg = NULL;
regionp curreg = NULL;
relocatep rel = NULL;
hookp hok = NULL;

labelp curlabel = NULL;
ff_u64_t offset = 0;
ff_u32_t adr = 0;

ff_u8_t of = _of_null;

# define OBSIZE 60

struct {
	ff_uint_t off, dst;
	ff_u8_t p[OBSIZE], *end;
} outbuf;

void ffas_ldsrc(void) {

}

void iadr(ff_uint_t __by) {
	adr+=__by;
}

ff_u32_t curadr() {return adr;}

void ff_as_init(void) {
	ff_as_hash_init(&symbols);
	ff_as_hash_init(&env);
	outbuf.end = outbuf.p+OBSIZE;
	outbuf.off = 0;
	outbuf.dst = offset;
	globl = (char const**)malloc(20*sizeof(char const*));
	*(globl++) = NULL;
	extrn = (char const**)malloc(20*sizeof(char const*));
	*(extrn++) = NULL;
}

void ff_as_de_init(void) {
	while(*(--globl) != NULL);
	free(globl);

	while(*(--extrn) != NULL);
	free(extrn);
}

void* ff_as_memdup(void *__p, ff_uint_t __bc) {
	void *ret = ff_as_al(__bc);
	ff_u8_t *p = (ff_u8_t*)ret;
	ff_u8_t *end = p+__bc;
	while(p != end) {
		*p = *((ff_u8_t*)__p+(p-(ff_u8_t*)ret));
		p++;
	}
	return ret;
}

ff_u64_t
ff_as_read_no(char *__p, ff_uint_t *__len, ff_u8_t *__sign) {
	char *p = __p;
	char buf[128];
	char *bufp = buf;
	if ((*__sign = (*p == '-')))
		p++;
	ff_u8_t hex = 0;
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
char static*
copyln(char *__dst, char *__src, char *__end, ff_uint_t *__len) {
	char *p = __src;
	while(*p != '\n' && *p != '\0' && p < __end) {
		*(__dst+(p-__src)) = *p;
		p++;
	}
	*__len = p-__src;
	return p;
}

ff_addr_t rgadr(char const*);
void adaptreg(symbolp __sy) {
	if (!__sy) return;
	if (is_syreg(__sy)) {
		ff_addr_t *ra = (ff_addr_t*)ff_as_al(sizeof(ff_addr_t));
		*ra = rgadr((char const*)__sy->p);
		__sy->p = ra;
	}
}

ff_i8_t static epdeg = -1;
char const static *ep = NULL;
void
ff_as(char *__p, char *__end) {
	/*
		we parse line by line
	*/
	char buf[256]; // line buffer
	char *p = __p;
	ff_uint_t len;
	while(p < __end) {
	_bk:
		while ((*p == ' ' | *p == '\t' | *p == '\n') && p < __end) p++;
		if (*p == '\0') break;

		if (*p == ';') {
			while(*p != '\n') {
				if (*p == '\0')
					break;
				p++;
			}
			goto _bk;
		}

		p = copyln(buf, p, __end, &len)+1;
		*(buf+len) = '\0';

		symbolp sy;
		if ((sy = ff_as_parse(buf)) != NULL) {
			if (is_symac(sy)) {
				if (!ffly_str_cmp(sy->p, "define")) {
					printf("got: macro\n");
					
				}
			} else if (is_sylabel(sy)) {
				labelp la = (labelp)ff_as_hash_get(&env, sy->p, sy->len);
				ff_i8_t exist = !la?-1:0;

				if (exist == -1)
					la = (labelp)ff_as_al(sizeof(struct label));
				la->offset = offset;
				la->s_adr = ff_as_stackadr();
				la->adr = curadr();
				la->s = sy->p;
				la->reg = curreg;
				if (exist == -1)
					ff_as_hash_put(&env, sy->p, sy->len, la);

				symbolp s = ff_as_syt(sy->p, NULL);
				ff_as_putsymbol(s);
				la->sy = s;
				s->sort = SY_LABEL;
				s->type = FF_SY_LCA;
				curlabel = la;
				printf("label\n");
			} else if (is_sydir(sy)) {
				printf("directive, %s\n", sy->p);
				if (*(char*)sy->p == 'b') {
					if (curseg->offset != offset) {
						// err
					}
					//oustbyte((ff_u8_t*)sy->next->p);
					*(curseg->fresh++) = *(ff_u8_t*)sy->next->p;
					ff_as_isa(sizeof(ff_u8_t));
					curseg->size++;
				} else if (*(char*)sy->p == 'w') {
					*(ff_u16_t*)curseg->fresh = *(ff_u16_t*)sy->next->p;
					curseg->fresh+=sizeof(ff_u16_t);
					ff_as_isa(sizeof(ff_u16_t));
					curseg->size+=sizeof(ff_u16_t);
				} else if (*(char*)sy->p == 'd') {
					*(ff_u32_t*)curseg->fresh = *(ff_u32_t*)sy->next->p;
					curseg->fresh+=sizeof(ff_u32_t);
					ff_as_isa(sizeof(ff_u32_t));
					curseg->size+=sizeof(ff_u32_t);
				} else if (*(char*)sy->p == 'q') {
					*(ff_u64_t*)curseg->fresh = *(ff_u64_t*)sy->next->p;
					curseg->fresh+=sizeof(ff_u64_t);
					ff_as_isa(sizeof(ff_u64_t));
					curseg->size+=sizeof(ff_u64_t);
				} else if (!strcmp(sy->p, "frk")) {

				} else if (!strcmp(sy->p, "entry") && epdeg<0) {
					epdeg = 0;
					ep = sy->next->p;
				} else if (!strcmp(sy->p, "globl")) {
					*(globl++) = sy->next->p;
					printf("-->symbol: %s\n", sy->next->p);
				} else if (!strcmp(sy->p, "segment")) {
					segmentp sg = (segmentp)ff_as_al(sizeof(struct segment));
		//			sg->name = sy->next->p;
					sg->next = curseg;
					sg->size = 0;
					sg->adr = ff_as_stackadr();
					sg->offset = 0;
					sg->fresh = sg->buf;
					curseg = sg;
				} else if (!strcmp(sy->p, "region")) {
					regionp rg = (regionp)ff_as_al(sizeof(struct region));
					rg->name = sy->next->p;
					rg->next = curreg;
					rg->beg = offset;
					rg->adr = curadr();
					if (!curreg)
						rg->no = 1;
					else
						rg->no = curreg->no+1;
					curreg = rg;
				} else if (!strcmp(sy->p, "endof")) {
					curreg->end = offset;
				} else if (!strcmp(sy->p, "extern")) {
					labelp la = (labelp)ff_as_al(sizeof(struct label));
					la->flags = LA_LOOSE;
					la->s = (char const*)ff_as_memdup(sy->next->p, sy->next->len+1);
					ff_as_hash_put(&env, sy->next->p, sy->next->len, la);
					symbolp s = ff_as_syt(sy->next->p, NULL);
					ff_as_putsymbol(s);
					la->sy = s;
					s->type = FF_SY_IND;
					s->sort = 0;
					printf("extern %s\n", sy->next->p);
					*(extrn++) = sy->next->p;
				} else if (*(char*)sy->p == 'l') {
					local_labelp ll = (local_labelp)ff_as_hash_get(&env, sy->next->p, sy->next->len);	
					ff_i8_t exist = !ll?-1:0;
					ffly_printf("---> %d, %s\n", exist, sy->next->p);
					if (exist == -1) {
						ll = (local_labelp)ff_as_al(sizeof(struct local_label));
						ff_as_hash_put(&env, sy->next->p, sy->next->len, ll);
					}

					ll->adr = curadr();
				}
			} else {
				ffly_printf("--| %s\n", sy->p);
				struct berry *bar;
				struct flask fak;
				if ((bar = (struct berry*)ff_as_hash_get(&env, sy->p, sy->len)) != NULL) {
					fak.end = fak.sy;
					symbolp cur = sy->next;
					while(cur != NULL) {
						adaptreg(cur);
						*(fak.end++) = cur;
						cur = cur->next;
					}

					if (sy->next != NULL) {
						if (is_syll(sy->next)) {
							void *p;
							if (!(p = ff_as_hash_get(&env, sy->next->p, ffly_str_len(sy->next->p))))
								ff_as_hash_put(&env, sy->next->p, sy->next->len, p = ff_as_al(sizeof(struct local_label)));
							sy->next->p = p;
						} else if (is_sylabel(sy->next)) {
							void *p;
							if (!(p = ff_as_hash_get(&env, sy->next->p, ffly_str_len(sy->next->p))))
								ff_as_hash_put(&env, sy->next->p, sy->next->len, p = ff_as_al(sizeof(struct label)));
							sy->next->p = p;
						}
					}

					fak_ = &fak;
					op = bar->op;
					ff_u64_t beg = offset;
					post(bar->emit);
					ff_u64_t end = offset;
					iadr(end-beg);
					printf("got: %s\n", sy->p);
				} else
					printf("unknown.\n");
			}
		}
	}
}

void
ff_as_reloc(ff_u64_t __offset, ff_u8_t __l, symbolp *__sy, local_labelp __ll) {
	relocatep rl = (relocatep)ff_as_al(sizeof(struct relocate));
	rl->offset = __offset;
	rl->l = __l;
	rl->sy = __sy;
	rl->next = rel;
	rl->ll = __ll;
	rel = rl;
}

void
ff_as_hook(ff_u64_t __offset, ff_u8_t __l, symbolp *__to, local_labelp __ll) {
	hookp hk = (hookp)ff_as_al(sizeof(struct hook));

	hk->offset = __offset;
	hk->l = __l;
	hk->to = __to;
	hk->next = hok;
	hok = hk;
}

void outsegs() {
	segmentp cur = curseg;
	while(cur != NULL) {
		cur->offset = offset;
		ff_uint_t size = cur->fresh-cur->buf;
		if (size>0)
			ff_as_oust(cur->buf, size);
		cur = cur->next;
	}
}

# include "../exec.h"
void ff_as_final(void) {
	if (!ep)
		ep = "_start";
	printf("entry point: %s\n", ep);
	labelp entry;
	if (!(entry = (labelp)ff_as_hash_get(&env, ep, ffly_str_len(ep))))
		printf("entry point not found.\n");
	if (of == _of_ffef) {
		struct ffef_hdr hdr;
		*hdr.ident = FF_EF_MAG0;
		hdr.ident[1] = FF_EF_MAG1;
		hdr.ident[2] = FF_EF_MAG2;
		hdr.ident[3] = FF_EF_MAG3;
		hdr.ident[4] = '\0';
		hdr.routine = entry != NULL?entry->adr:FF_EF_NULL;
		hdr.format = _ffexec_bc;
		hdr.nsg = 0;
		hdr.nrg = 0;
		hdr.nrl = 0;
		hdr.nhk = 0;
		hdr.sg = FF_EF_NULL;
		hdr.rg = FF_EF_NULL;
		hdr.rl = FF_EF_NULL;
		hdr.hk = FF_EF_NULL;
		hdr.adr = curadr();
		outsegs();
		char const **cur = globl;
		while(*(--cur) != NULL) {
			printf("symbol: %s\n", *cur);
			symbolp sy = ff_as_getsymbol(*cur);
			sy->type = FF_SY_GBL;
		}

		cur = extrn;
		while(*(--cur) != NULL) {
			printf("extern: %s\n", *cur);
			hookp hk = hok;

			while(hk != NULL) {
				printf("symbol: %p, len: %u\n", hk->to, hk->l);
				if (!strcmp((*hk->to)->p, *cur)) {
					struct ffef_hok hok;
					hok.offset = hk->offset;
					hok.l = hk->l;
					hok.to = (*hk->to)->off;
					ff_as_oust((ff_u8_t*)&hok, ffef_hoksz);
					hdr.nhk++;
				}
				hk = hk->next;
			}
		}

		if (hok != NULL)
			hdr.hk = offset-ffef_hoksz;	

		relocatep rl = rel;
		while(rl != NULL) {
			struct ffef_rel rel;
			rel.offset = rl->offset;
			rel.l = rl->l;
			rel.addto = !rl->ll?0:(rl->ll->adr-(*rl->ll->p_adr));
			printf("reloc: %s\n", (*rl->sy)->p);
			rel.sy = (*rl->sy)->off;
			ff_as_oust((ff_u8_t*)&rel, ffef_relsz);
			rl = rl->next;
			hdr.nrl++;
		}

		if (rel != NULL)
			hdr.rl = offset-ffef_relsz;

		segmentp sg = curseg;
		while(sg != NULL) {
			struct ffef_seg_hdr seg;
			seg.adr = sg->adr;
			seg.offset = sg->offset;
			seg.sz = sg->size;
			ff_as_oust((ff_u8_t*)&seg, ffef_seg_hdrsz);
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
			ff_as_oust(rg->name, reg.l);
			reg.beg = rg->beg;
			reg.end = rg->end;
			reg.adr = rg->adr;
			if (!strcmp(rg->name, "text"))
				reg.type = FF_RG_PROG;
			else
				reg.type = FF_RG_NULL;

			ff_as_oust((ff_u8_t*)&reg, ffef_reg_hdrsz);
			hdr.nrg++;
			rg = rg->next;
		}
	
		// drop the region header hear
		ff_as_syt_drop();

		if (curreg != NULL)
			hdr.rg = offset-ffef_reg_hdrsz;
		hdr.nrg++;

		// put contents
		ff_as_syt_gut();
		// store header - save
		ff_as_syt_store();

		// string table region
		hdr.sttr = ff_as_stt_drop();
		lseek(out, 0, SEEK_SET);
		write(out, &hdr, ffef_hdr_size);
	}
	
	if (outbuf.off>0) {
		lseek(out, outbuf.dst, SEEK_SET);
		write(out, outbuf.p, outbuf.off);
	}
}

void static
drain() {
	lseek(out, outbuf.dst, SEEK_SET);
	write(out, outbuf.p, outbuf.off);
	outbuf.off = 0;
	outbuf.dst = offset;
}

void ff_as_oust(ff_u8_t *__p, ff_u8_t __n) {
	if (__n > OBSIZE) { //shoud remove
		printf("error: cant oust anything larger then %u\n", OBSIZE);
		return;
	}

	/* if offset has been changed by an alternative means drain it
	*/
	if (outbuf.dst+outbuf.off != offset)
		drain();

	ff_int_t overflow;
	if ((overflow = (ff_int_t)(outbuf.off+__n)-(ff_int_t)((outbuf.end-outbuf.p)-1))>0)
		__n-=overflow;

	ffly_mem_cpy(outbuf.p+outbuf.off, __p, __n);
	offset+=__n;
	outbuf.off+=__n;

	if (outbuf.p+outbuf.off == outbuf.end-1)
		drain();

	if (overflow>0) {
		ff_u8_t *p = __p+__n;
		ff_u8_t *end = p+overflow;
		while(p != end)
			ff_as_oust(p++, 1);
	}
}

void ff_as_oustbyte(ff_u8_t __byte) {
	ff_as_oust(&__byte, 1);
}

void ff_as_oust_16l(ff_u16_t __data) {
	ff_as_oust((ff_u8_t*)&__data, 2);
}

void ff_as_oust_32l(ff_u32_t __data) {
	ff_as_oust((ff_u8_t*)&__data, 4);
}

void ff_as_oust_64l(ff_u64_t __data) {
	ff_as_oust((ff_u8_t*)&__data, 8);
}
