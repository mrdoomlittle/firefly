# include "../ffef.h"
# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <malloc.h>
# include <stdio.h>
int fd;
void prhdr(ffef_hdrp __hdr) {
	char *i = __hdr->ident;
	printf("ident: ");
	while(i != __hdr->ident+FF_EF_IL-1 && *(i+1) != '\0')
		printf("%c, ", *(i++));
	printf("%c\n", *i);	

	printf("code segment, start: %u, end: %u, size: %u\n", __hdr->routine, __hdr->end, __hdr->end-__hdr->routine);
}

void prsy(ffef_sym_hdrp __sy) {
	char *name = (char*)malloc(__sy->l);
	lseek(fd, __sy->name, SEEK_SET);
	read(fd, name, __sy->l);

	printf("symbol, name: %s, loc: %u\n", name, __sy->loc);

	free(name);
}

int main(int __argc, char const *__argv[]) {
	fd = open(__argv[1], O_RDONLY);

	struct ffef_hdr hdr;
	read(fd, &hdr, ffef_hdr_size);

	prhdr(&hdr);
	
	struct ffef_sym_hdr sy;
	if (hdr.sy != FF_EF_NULL) {
		mdl_uint_t i = 0;
		mdl_u64_t offset = hdr.sy;
		while(i != hdr.nsy) {
			lseek(fd, offset, SEEK_SET);
			read(fd, &sy, ffef_sym_hdrsz);
			prsy(&sy);				
			offset-=ffef_sym_hdrsz+sy.l;
			i++;
		}
	}

	close(fd);

}
