# include <unistd.h>
# include <fcntl.h>
# include "../../elf.h"
# include <string.h>
int main() {
	int fd = open("elf", O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
	struct elf64_hdr hdr = {
	.type = ET_EXEC,
		.machine = EM_X86_64,
		.version = EV_CURR
	};

	char const s[] = ".text\0.symtab\0";
	hdr.ident[EI_MAG0] = 0x7f;
	hdr.ident[EI_MAG1] = 'E';
 	hdr.ident[EI_MAG2] = 'L';
	hdr.ident[EI_MAG3] = 'F';
	hdr.ident[EI_CLS] = ELFCLS64;
	hdr.ident[EI_DAT] = ELFDAT2LSB;
	hdr.ident[EI_VERS] = EV_CURR;
	hdr.ident[EI_OSABI] = ELFOSABI_NONE;
	hdr.ident[EI_ABIVERS] = 0;
	hdr.ident[EI_PAD] = 0;
	hdr.shsize = sizeof(struct elf64_shdr);
	hdr.shc = 2;
	hdr.shstdx = 1;

	hdr.hdr_size = sizeof(struct elf64_hdr);
	hdr.entry = 0;

	struct elf64_shdr syt, stt;
	bzero(&syt, sizeof(struct elf64_shdr));
	bzero(&stt, sizeof(struct elf64_shdr));
	syt.offset = sizeof(s)+sizeof(struct elf64_shdr)+sizeof(struct elf64_shdr)+sizeof(struct elf64_hdr);
	syt.type = SHT_SYMTAB;
	syt.flags = SHF_EXECINSTR;
	syt.entsize = sizeof(struct elf64_sym);
	syt.link = 1;
	syt.addralign = 1;
	syt.size = syt.entsize;
	syt.name = 0;

	stt.offset = sizeof(struct elf64_hdr);
	stt.type = SHT_STRTAB;
	stt.flags = 0;
	stt.entsize = 14;
	stt.link = 1;
	stt.addralign = 1;
	stt.size = 14;
	stt.name = 6;

	hdr.sh_off = sizeof(struct elf64_hdr);
	
	write(fd, &hdr, sizeof(struct elf64_hdr));

	write(fd, s, sizeof(s));

	write(fd, &syt, sizeof(struct elf64_shdr));
	write(fd, &stt, sizeof(struct elf64_shdr));

	struct elf64_sym sym;
	bzero(&sym, sizeof(struct elf64_sym));
	sym.size = sizeof(struct elf64_sym);
	sym.info = STB_GLOBAL;
	sym.other = STV_DEFAULT;
	write(fd, &sym, sizeof(struct elf64_sym));

/*
	struct elf64_sym s;
	bzero(&s, sizeof(struct elf64_sym));
	sym.size = sizeof(struct elf64_sym);
*/
	close(fd);
	}
