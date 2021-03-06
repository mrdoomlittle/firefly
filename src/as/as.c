# include "as.h"
# include "../dep/str_len.h"
# include "../ffly_def.h"
# include "../system/string.h"
# include "../elf.h"
# include "../stdio.h"
# include "../string.h"
# include "../malloc.h"
# include "../dep/str_cmp.h"
# include "../remf.h"

/*
	TODO:
		fix glob and extn
*/
/*
	operation tray
*/
void **op_tray;


void(*ff_as_forge)(void);
void*(*ff_as_getreg)(char const*);
ff_u8_t(*ff_as_regsz)(void*);
ff_i8_t(*ff_as_suffix)(ff_u8_t);
void(*ff_as_fixins)(struct fix_s*);
ff_u8_t fix_flgs;
/*
	rename

	output instruction 
*/
void(*post)(void);

ff_i8_t _local = -1;
void *__label;
struct hash symbols;
struct hash defines;
struct hash env;

/*
	replace - using this for now as labels may not exist at the time 
	so we resolve them later.
*/
char const **globl;
char const **extrn;

struct flask *fak_;

struct fix_s *fx_head = NULL;
segmentp curseg = NULL;
regionp curreg = NULL;
relocatep rel = NULL;
hookp hok = NULL;

labelp curlabel = NULL;

/*
	oust uses this
*/
ff_u64_t offset = 0;

// used to keep track the end of program memory nothing else
ff_u32_t adr;

ff_u8_t of = _of_null;

# define OBSIZE 60
void fix(struct frag *__f, void *__arg, ff_u8_t __type, ff_u8_t __flags) {
	struct fix_s *s = (struct fix_s*)ff_as_al(sizeof(struct fix_s));
	s->f = __f;
	s->arg = __arg;
	s->type = __type;
	s->flags = __flags;
	s->next = fx_head;
	fx_head = s;
}

struct {
	ff_uint_t off, dst;
	ff_u8_t p[OBSIZE], *end;
} outbuf;

void ffas_ldsrc(void) {

}

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
	while((*p >= '0' && *p <= '9') || *p == 'x' || (*p >= 'a' && *p <= 'z')) {
		if (*p == 'x')
			hex = 1;

		*(bufp++) = *(p++);
	}

	*bufp = '\0';
	*__len = bufp-buf;

	if (hex) {
		printf("hex: %s:%u\n", buf, ffly_htint(buf+2));
		return ffly_htint(buf+2);
	}
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

ff_i8_t static epdeg = -1;
char const static *ep = NULL;

symbolp static sy;
void static
_macro(void) {
	if (!ffly_str_cmp(sy->p, "define")) {
		printf("got: macro\n");
					
	}
}

void static 
_label(void) {
	labelp la;
	
	la = (labelp)ff_as_hash_get(&env, sy->p, sy->len);
	ff_i8_t exist = !la?-1:0;

	if (exist == -1)
		la = (labelp)ff_as_al(sizeof(struct label));
	la->s_adr = ff_as_stackadr();
	la->s = sy->p;
	la->flags = 0x00;
	la->reg = curreg;
	la->f = curfrag;
	la->fn = curfrag->f;
	la->foffset = frag_offset(curfrag);
	if (exist == -1)
		ff_as_hash_put(&env, sy->p, sy->len, la);

	symbolp s = ff_as_syt(sy->p, NULL);
	ff_as_putsymbol(s);
	la->sy = s;
	s->sort = SY_LABEL;
	s->type = FF_SY_LCA;
	curlabel = la;

	printf("new label at frag: %u, offset: %u\n", la->f->f, la->foffset);
	ff_as_fdone(curfrag);
	ff_as_fnew();
}

void static
_directive(void) {
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
		rg->beg.f = curfrag;
		rg->beg.offset = frag_offset(curfrag);
		if (!curreg)
			rg->no = 1;
		else
			rg->no = curreg->no+1;
		curreg = rg;
	} else if (!strcmp(sy->p, "endof")) {
		curreg->end.f = curfrag;
		curreg->end.offset = frag_offset(curfrag);
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

		printf("local label frag-%u\n", curfrag->f);
		ll->fn = curfrag->f;
		ll->f = curfrag;
		ll->foffset = frag_offset(curfrag);
		ff_as_fdone(curfrag);
		ff_as_fnew();

	}
}

void
ff_as(char *__p, char *__end) {
	ff_as_fnew(); // start fragment
	/*
		we parse line by line
	*/
	char buf[256]; // line buffer
	char *p = __p;
	ff_uint_t len;
	while(p < __end) {
	_bk:
		while ((*p == ' ' | *p == '\t' | *p == '\n') && p < __end) p++;
		if (*p == '\0')
			break;

		if (*p == ';') {
			p++;
			while(*p != '\n') {
				if (++p>=__end)
					goto _r;
			}
			goto _bk;
		} else if (*p == '/') {
			p++;
			while(*p != '/') {
				if (++p>=__end)
					goto _r;
			}
			p++;
			goto _bk;
		}

		/*
			copy line to buffer
		*/
		p = copyln(buf, p, __end, &len)+1;
		*(buf+len) = '\0';
		printf(":: %s\n", buf);
		if ((sy = ff_as_parse(buf)) != NULL) {
			if (is_symac(sy)) {
				_macro();
			} else if (is_sylabel(sy)) {
				_label();
			} else if (is_sydir(sy)) {
				_directive();
			} else {
				ffly_printf("--| %s:%u\n", sy->p, sy->len);
				struct flask fak;
				if ((op_tray = (void**)ff_as_hash_get(&env, sy->p, sy->len)) != NULL) {
					fak.n = 0;
					symbolp cur = sy->next;
					void **p = fak.p;
					ff_u16_t info;
					while(cur != NULL) {
						if (is_syreg(cur)) {
							void *reg;
							if (!(reg = ff_as_getreg((char const*)cur->p))) {
								printf("register not recognized, %s\n", cur->p);
								return;
							}
							printf("-- reg.\n");
							switch(ff_as_regsz(*p = reg)) {
								case 1:
									info = _o_reg8;
								break;
								case 2:
									info = _o_reg16;
								break;
								case 4:
									info = _o_reg32;
								break;
								case 8:
									info = _o_reg64;
								break;
							}
						} else if (is_syint(cur)) {
							printf("-- imm.\n");
							*p = cur->p;
							ff_u64_t val = *(ff_u64_t*)*p;
							
							if (val >= 0 && val <= (ff_u8_t)~0)
								info = _o_imm8;
							else if (val > (ff_u8_t)~0 && val <= (ff_u16_t)~0)
								info = _o_imm16;
							else if (val > (ff_u16_t)~0 && val <= (ff_u32_t)~0)
								info = _o_imm32;
							else if (val > (ff_u32_t)~0 && val <= (ff_u64_t)~0)
								info = _o_imm64;
							printf("val: %u\n", val);
						} else if (is_syll(cur) || is_sylabel(cur)) {
							printf("-- label.\n");
							__label = ff_as_hash_get(&env, cur->p, ffly_str_len(cur->p));

							if (is_syll(cur)) {
								_local = 0;
								if (!__label)
									ff_as_hash_put(&env, cur->p, cur->len, __label = ff_as_al(sizeof(struct local_label)));

							} else if (is_sylabel(cur)) {
								_local = -1;
								if (!__label) {
									ff_as_hash_put(&env, cur->p, cur->len, __label = ff_as_al(sizeof(struct label)));
									((labelp)__label)->flags = 0x00;
								}
							}

							info = _o_label;
						}

						setinfo(&fak, info, p-fak.p);
						p++;
						cur = cur->next;
						fak.n++;
					}
		
					printf("operands: %u\n", fak.n);
					*p = NULL;
					fak_ = &fak;
					post();
					printf("got: %s\n", sy->p);
				} else
					printf("unknown op.\n");
			}
		}
	}
_r:
	return;
}

void static drain();

labelp ff_as_entry;
void ff_as_final(void) {
	ff_as_fdone(curfrag);
	if (outbuf.off>0) {
		drain();
	}
	ff_uint_t bs;
	struct frag *f;
	struct fix_s *fx;
	
_again:
	bs = 0;
	fix_flgs = 0x00;
	fx = fx_head;
	while(fx != NULL) { 
		ff_as_fixins(fx);
		printf("fix: BS: %u, M: %u, K: %u\n", fx->f->bs, fx->f->m, fx->f->k);
		fx = fx->next;
	}

	f = fr_head;
	while(f != NULL) {
		f->m = bs;
		bs+=f->bs+f->k;
		printf("frag-%u: BS: %u, M: %u, K: %u\n", f->f, f->bs, f->m, f->k);
		
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
	
	ff_as_foutput();

	if (of == _of_raw)
		goto _sk;
	if (!ep)
		ep = "_start";
	printf("entry point: %s\n", ep);
	if (!(ff_as_entry = (labelp)ff_as_hash_get(&env, ep, ffly_str_len(ep))))
		printf("entry point not found.\n");
	ff_as_forge();
_sk:
	if (outbuf.off>0) {
		lseek(out, outbuf.dst, SEEK_SET);
		write(out, outbuf.p, outbuf.off);
	}
}

void
drain() {
	printf("draining to %u, size: %u\n", outbuf.dst, outbuf.off);
	if (outbuf.off>0) {
		lseek(out, outbuf.dst, SEEK_SET);
		write(out, outbuf.p, outbuf.off);
	}
	outbuf.off = 0;
	outbuf.dst = offset;
}

void ff_as_oust(ff_u8_t *__p, ff_u16_t __n) {
	while(__n>OBSIZE) {
		ff_as_oust(__p, OBSIZE);
		__n-=OBSIZE; 
		__p+=OBSIZE;
	}

	if (!__n) {
		printf("nothing left.\n");
		return;
	}

	/* if offset has been changed by an alternative means drain it
	*/
	if (outbuf.dst+outbuf.off != offset) {
		printf("draining buffer as its no longer on track.\n");
		drain();
	}

	ff_int_t overflow;
	if ((overflow = (ff_int_t)(outbuf.off+__n)-(ff_int_t)((OBSIZE)-1))>0)
		__n-=overflow;

//	printf("filling in buffer at : %u : %u\n", outbuf.off, __n);
	ffly_mem_cpy(outbuf.p+outbuf.off, __p, __n);
	offset+=__n;
	outbuf.off+=__n;

	if (outbuf.off == OBSIZE-1)
		drain();

	if (overflow>0) {
		ff_u8_t *p;
		p = __p+__n;
		ff_u8_t *end;
		end = p+overflow;
		while(p != end) {
			ff_as_oust(p++, 1);
		}
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
