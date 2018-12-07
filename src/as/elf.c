# include "as.h"
# include "../string.h"
# include "../malloc.h"
# include "../elf.h"
# include "../stdio.h"

/*
only using this for experimentation
*/
ff_uint_t extern stt_off;
struct elf64_shdr static syt;
ff_uint_t static stt;
void static
syt_store(void) {
	syt.name = stt_off;
	syt.type = SHT_SYMTAB;
	syt.link = stt;
	syt.entsize = elf64_sym_size;
	syt.addralign = 1;
	syt.info = 1;
	char buf[128];
	strcpy(buf, ".symtab");
	ff_as_stt(buf, 0);
	lseek(out, syt_dst, SEEK_SET);
	write(out, &syt, elf64_shdr_size);
}

void static
syt_drop(void) {
	syt_dst = offset;
	offset+=elf64_shdr_size;
}

void static
syt_gut(void) {
	syt.offset = offset;
	symbolp cur = syt_head;
	struct elf64_sym sy;
	bzero(&sy, elf64_sym_size);
	sy.name = stt_off;
	ff_as_stt("uk\0", 3);
	sy.info = ELF32_ST_INFO(STB_LOCAL, STT_NOTYPE);
	ff_as_oust((ff_u8_t*)&sy, elf64_sym_size);
	syt.size+=elf64_sym_size;
	while(cur != NULL) {
		struct elf64_sym sy;
		bzero(&sy, elf64_sym_size);
		sy.name = stt_off;
		labelp la = (labelp)ff_as_hash_get(&env, cur->p, cur->len);
		sy.val = la->f->adr+la->foffset;
		printf("symbol %u\n", sy.val);
		sy.info = ELF32_ST_INFO(STB_GLOBAL, STT_NOTYPE);
		sy.sdx = 1;
		ff_as_stt(cur->p, 0);	
		ff_as_oust((ff_u8_t*)&sy, elf64_sym_size);
		symbolp bk = cur;
		cur = cur->next;
		syt.size+=elf64_sym_size;

		free(bk->p);
		free(bk);
	}
}

void static
forge(void) {
	bzero(&syt, elf64_shdr_size);
	struct elf64_hdr hdr;
	bzero(&hdr, elf64_hdr_size);
	*hdr.ident = ELF_MAG0;
	hdr.ident[EI_MAG1] = ELF_MAG1;
	hdr.ident[EI_MAG2] = ELF_MAG2;
	hdr.ident[EI_MAG3] = ELF_MAG3;
	hdr.ident[EI_CLS] = ELFCLS64;
	hdr.ident[EI_DAT] = ELFDAT2LSB;
	hdr.ident[EI_VERS] = EV_CURR;
	hdr.ident[EI_OSABI] = ELFOSABI_NONE;

	hdr.type = ET_REL;
	hdr.machine = EM_X86_64;

	ff_as_syt_gut();
	hdr.sh_off = offset;
	struct elf64_shdr sec;
	bzero(&sec, elf64_shdr_size);
	sec.name = stt_off;
	ff_as_stt("uk\0", 3);
	ff_as_oust((ff_u8_t*)&sec, elf64_shdr_size);
	hdr.shc++;
	regionp rg = curreg;
	while(rg != NULL) {
		struct elf64_shdr sec;
		bzero(&sec, elf64_shdr_size);
		sec.name = stt_off;
		char buf[128];
		*buf = '.';
		strcpy(buf+1, rg->name);
		ff_as_stt(buf, 0);
		printf("region: %s\n", rg->name);
		if (!strcmp(rg->name, "text")) {
			sec.size = (rg->end.f->dst+rg->end.offset)-(rg->beg.f->dst+rg->beg.offset);
			sec.offset = rg->beg.f->dst+rg->beg.offset;
			sec.addralign = 1;
			sec.flags = SHF_EXECINSTR|SHF_ALLOC;
			sec.type = SHT_PROGBITS;
		}
		rg = rg->next;
		ff_as_oust((ff_u8_t*)&sec, elf64_shdr_size);
		hdr.shc++;
	}

	stt = hdr.shc+1;
	ff_as_syt_drop();
	ff_as_syt_store();
	hdr.shc++;

	hdr.hdr_size = elf64_hdr_size;
	hdr.shsize = elf64_shdr_size;
	ff_as_stt_drop();
	hdr.shc++;
	hdr.shstdx = hdr.shc-1;

	lseek(out, 0, SEEK_SET);
	write(out, &hdr, elf64_hdr_size);
}

ff_u64_t static
stt_drop(void) {
	ff_u64_t ret = offset;
	offset+=elf64_shdr_size;
	struct elf64_shdr sec;
	bzero(&sec, elf64_shdr_size);

	sec.name = stt_off;
	ff_as_stt(".strtab", 0);
	sec.addralign = 1;
	sec.offset = offset;
	sec.type = SHT_STRTAB;
	stp cur = stt_tail;
	printf("stt_off: %u, name: %u\n", sec.offset, sec.name);
	while(cur != NULL) {
		printf("---> %s, %u\n", cur->p, cur->l);
		ff_as_oust((ff_u8_t*)cur->p, cur->l);
		sec.size+=cur->l;
		free((void*)cur->p);
		stp bk = cur;
		cur = cur->prev;
		free(bk);
	}

	lseek(out, ret, SEEK_SET);
	write(out, &sec, elf64_shdr_size);
	return ret;
}

void
_elf_reloc(void) {

}

void ff_as_elf(void) {
	ff_as_syt_store = syt_store;
	ff_as_syt_drop = syt_drop;
	ff_as_syt_gut = syt_gut;
	ff_as_forge = forge;
	ff_as_stt_drop = stt_drop;
}
