# include "as.h"
# include "../string.h"
# include "../malloc.h"
# include "../remf.h"
# include "../stdio.h"
# include "../remf.h"
# include "../exec.h"
struct remf_reg_hdr static reg;
void static
syt_store(void) {
	reg.name = syt_dst;
	reg.l = 4;
	reg.type = FF_RG_SYT;
	lseek(out, syt_dst, SEEK_SET);
	write(out,"syt", 4);
	write(out, &reg, remf_reghdrsz);
}

void static
syt_drop(void) {
	syt_dst = offset;
	offset+=4+remf_reghdrsz;
}

void static 
syt_gut(void) {
	reg.beg = offset;
	symbolp cur = syt_head;
	while(cur != NULL) {
		struct remf_sy sy;
		sy.name = ff_as_stt(cur->p, cur->len);
		sy.type = cur->type;
		sy.reg = 0;
		if (is_sylabel(cur)) {
			labelp la = (labelp)ff_as_hash_get(&env, cur->p, cur->len);
			sy.reg = la->reg->no;
			sy.loc = la->f->adr+la->foffset;
		}

		sy.l = cur->len+1;
		ff_as_oust((ff_u8_t*)&sy, remf_sysz);
		symbolp bk = cur;
		cur = cur->next;

		free(bk->p);
		free(bk);
	}
	reg.end = offset;
}

ff_u64_t static
stt_drop(void) {
	struct remf_reg_hdr reg;
	reg.beg = offset;
	stp cur = stt_head;
	while(cur != NULL) {
		ff_as_oust((ff_u8_t*)cur->p, cur->l);
		free((void*)cur->p);
		stp bk = cur;
		cur = cur->next;
		free(bk);
	}
	reg.end = offset;

	ff_u64_t ret;
	char const *name = "stt";
	reg.l = 3;
	reg.type = FF_RG_STT;
	reg.name = offset;
	ff_as_oust((ff_u8_t*)name, reg.l);

	ret = offset;
	ff_as_oust((ff_u8_t*)&reg, remf_reghdrsz);
	return ret;
}

extern char const *globl;
extern char const *extrn;
relocatep extern rel;
hookp extern hok;


// not working not the time at the moment
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

void
_remf_reloc(struct frag *__f, ff_u64_t __dis, ff_u32_t __ob, ff_u8_t __l) {
	printf("reloc.\n");
	relocatep rl = (relocatep)ff_as_al(sizeof(struct relocate));
	rl->dis = __dis;
	rl->l = __l;
	rl->sy = !_local?&((local_labelp)__label)->parent->sy:&((labelp)__label)->sy;
	rl->f = __f;
	rl->ob = __ob;
	rl->next = rel;
	rl->ll = !_local?(local_labelp)__label:NULL;
	rel = rl;
}

void
_remf_hook(struct frag *__f, ff_u64_t __dis, ff_u32_t __ob, ff_u8_t __l) {
	printf("hook.\n");
	hookp hk = (hookp)ff_as_al(sizeof(struct hook));
	
	hk->dis = __dis;
	hk->l = __l;
	hk->f = __f;
	hk->ob = __ob;
	hk->to = &((labelp)__label)->sy;
	hk->next = hok;
	hok = hk;
}

void static
forge(void) {
	struct remf_hdr hdr;
	*hdr.ident = FF_REMF_MAG0;
	hdr.ident[1] = FF_REMF_MAG1;
	hdr.ident[2] = FF_REMF_MAG2;
	hdr.ident[3] = FF_REMF_MAG3;
	hdr.ident[4] = '\0';
	hdr.routine = ff_as_entry != NULL?ff_as_entry->f->adr+ff_as_entry->foffset:FF_REMF_NULL;
	hdr.format = _ffexec_bc;
	hdr.nsg = 0;
	hdr.nrg = 0;
	hdr.nrl = 0;
	hdr.nhk = 0;
	hdr.sg = FF_REMF_NULL;
	hdr.rg = FF_REMF_NULL;
	hdr.rl = FF_REMF_NULL;
	hdr.hk = FF_REMF_NULL;

	/*
		bottom of program memory - where other data should be placed	
	*/
	hdr.adr = adr;
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
			printf("symbol: %s:%p, len: %u\n", (*hk->to)->p, hk->to, hk->l);
			if (!strcmp((*hk->to)->p, *cur)) {
				struct remf_hok hok;
				hok.offset = hk->f->dst+hk->dis;
				hok.l = hk->l;
				hok.adr = hk->f->adr+hk->ob;
				hok.to = (*hk->to)->off;
				ff_as_oust((ff_u8_t*)&hok, remf_hoksz);
				hdr.nhk++;
			}
			hk = hk->next;
		}
	}

	if (hok != NULL)
		hdr.hk = offset-remf_hoksz;	

	relocatep rl = rel;
	while(rl != NULL) {
		struct remf_rel rel;
		rel.offset = rl->f->dst+rl->dis;
		rel.l = rl->l;
		local_labelp ll = rl->ll;
		rel.addto = !ll?0:(ll->f->dst+ll->foffset)-((*ll->p_f)->dst+*ll->p_foffset);
		printf("reloc: %s\n", (*rl->sy)->p);
		rel.adr = rl->f->adr+rl->ob;
		rel.sy = (*rl->sy)->off;
		ff_as_oust((ff_u8_t*)&rel, remf_relsz);
		rl = rl->next;
		hdr.nrl++;
	}

	if (rel != NULL)
		hdr.rl = offset-remf_relsz;

	segmentp sg = curseg;
	while(sg != NULL) {
		struct remf_seg_hdr seg;
		seg.adr = sg->adr;
		seg.offset = sg->offset;
		seg.sz = sg->size;
		ff_as_oust((ff_u8_t*)&seg, remf_seghdrsz);
		hdr.nsg++;
		sg = sg->next;
	}

	if (curseg != NULL)
		hdr.sg = offset-remf_seghdrsz;

	regionp rg = curreg;
	while(rg != NULL) {
		struct remf_reg_hdr reg;
		reg.l = ffly_str_len(rg->name)+1;
		reg.name = offset;
		ff_as_oust(rg->name, reg.l);
		reg.beg = (rg->beg.f->dst+rg->beg.offset);
		reg.end = (rg->end.f->dst+rg->end.offset);
		reg.adr = rg->adr;
		if (!strcmp(rg->name, "text"))
			reg.type = FF_RG_PROG;
		else
			reg.type = FF_RG_NULL;

		ff_as_oust((ff_u8_t*)&reg, remf_reghdrsz);
		hdr.nrg++;
		rg = rg->next;
	}
	
	// drop the region header hear
	ff_as_syt_drop();

	if (curreg != NULL)
		hdr.rg = offset-remf_reghdrsz;
	hdr.nrg++;

	// put contents
	ff_as_syt_gut();
	// store header - save
	ff_as_syt_store();

	// string table region
	hdr.sttr = ff_as_stt_drop();
	lseek(out, 0, SEEK_SET);
	write(out, &hdr, remf_hdrsz);
}

void ff_as_remf(void) {
	ff_as_syt_store = syt_store;
	ff_as_syt_drop = syt_drop;
	ff_as_syt_gut = syt_gut;
	ff_as_forge = forge;
	ff_as_stt_drop = stt_drop;
}
