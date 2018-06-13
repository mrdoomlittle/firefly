# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <malloc.h>
# include <elf.h>
# include "../ffint.h"
char const *hdrtyps(ff_u16_t __type) {
	switch(__type) {
		case ET_NONE: return "none";
		case ET_REL: return "relocatable";
		case ET_EXEC: return "executable";
		case ET_DYN: return "shared object";
		case ET_CORE: return "core";
	}
	return "unknown";
}


void print_hdr(Elf64_Ehdr *__hdr) {
	ff_u8_t *i = __hdr->e_ident;
	printf("magic: ");
	while(i != __hdr->e_ident+EI_NIDENT-1)
		printf("%02x ", *(i++));
	printf("%02x\n", *i);

	printf("type: %s\n", hdrtyps(__hdr->e_type));
}

int main(int __argc, char const *__argv[]) {
	if (__argc < 2) {
		fprintf(stderr, "please provide a file.\n");
		return -1;
	}

	int fd;
	if ((fd = open(__argv[1], O_RDONLY)) == -1) {
		fprintf(stderr, "failed to open.\n");
		return -1;
	}
	struct stat st;
	fstat(fd, &st);

	ff_u8_t *p = malloc(st.st_size);
	read(fd, p, st.st_size);

	Elf64_Ehdr *hdr = (Elf64_Ehdr*)p;
	print_hdr(hdr);
	Elf64_Shdr *sec = (Elf64_Shdr*)(p+hdr->e_shoff);
    
	char *stt = (char*)(p+sec->sh_offset);
	printf("%s\n", stt+sec->sh_name);
	printf("entry: %lu\n", hdr->e_entry);

	free(p);
	close(fd);
	return 0;
}
