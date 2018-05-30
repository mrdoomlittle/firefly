# include "as.h"
# include "../string.h"
# include "../malloc.h"
# include "../ffef.h"
# include "../stdio.h"
# include "../ffef.h"
# include "../exec.h"
struct ffef_reg_hdr static reg;
void static
syt_store(void) {
	reg.name = syt_dst;
	reg.l = 4;
	reg.type = FF_RG_SYT;
	lseek(out, syt_dst, SEEK_SET);
	write(out,"syt", 4);
	write(out, &reg, ffef_reg_hdrsz);
}

void static
syt_drop(void) {
	syt_dst = offset;
	offset+=4+ffef_reg_hdrsz;
}

void static 
syt_gut(void) {
	reg.beg = offset;
	symbolp cur = syt_head;
	while(cur != NULL) {
		struct ffef_sy sy;
		sy.name = ff_as_stt(cur->p, cur->len);
		sy.type = cur->type;
		sy.reg = 0;
		if (is_sylabel(cur)) {
			labelp la = (labelp)ff_as_hash_get(&env, cur->p, cur->len);
			sy.reg = la->reg->no;
			sy.loc = la->adr;
		}

		sy.l = cur->len+1;
		ff_as_oust((ff_u8_t*)&sy, ffef_sysz);
		symbolp bk = cur;
		cur = cur->next;

		free(bk->p);
		free(bk);
	}
	reg.end = offset;
}

ff_u64_t static
stt_drop(void) {
	struct ffef_reg_hdr reg;
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
	ff_as_oust((ff_u8_t*)&reg, ffef_reg_hdrsz);
	return ret;
}

extern char const *globl;
extern char const *extrn;
void extern outsegs();
relocatep extern rel;
hookp extern hok;

void static
forge(void) {
	struct ffef_hdr hdr;
	*hdr.ident = FF_EF_MAG0;
	hdr.ident[1] = FF_EF_MAG1;
	hdr.ident[2] = FF_EF_MAG2;
	hdr.ident[3] = FF_EF_MAG3;
	hdr.ident[4] = '\0';
	hdr.routine = ff_as_entry != NULL?ff_as_entry->adr:FF_EF_NULL;
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

void ff_as_ffef(void) {
	ff_as_syt_store = syt_store;
	ff_as_syt_drop = syt_drop;
	ff_as_syt_gut = syt_gut;
	ff_as_forge = forge;
	ff_as_stt_drop = stt_drop;
}
