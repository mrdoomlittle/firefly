# include "bond.h"
# include "../linux/unistd.h"
# include "../linux/fcntl.h"
# include "../linux/stat.h"
# include "../stdio.h"
# include "../ffly_def.h"
int static s;
int static d;

char const *extsrcfl(char const **__p) {
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

mdl_i8_t validate(ffef_hdrp __hdr) {
	if (*__hdr->ident != FF_EF_MAG0) {
		printf("mag0 corrupted\n");
		return -1;
	}

	if (__hdr->ident[1] != FF_EF_MAG1) {
		printf("mag1 corrupted\n");
		return -1;
	}

	if (__hdr->ident[2] != FF_EF_MAG2) {
		printf("mag2 corrupted\n");
		return -1;
	}

	if (__hdr->ident[3] != FF_EF_MAG3) {
		printf("mag3 corrupted\n");
		return -1;
	}
	return 0;
}

# include "../malloc.h"
# include "../dep/bzero.h"
mdl_uint_t offset = ffef_hdr_size;
mdl_uint_t get_offset() {
	return offset;
}

void incr_offset(mdl_uint_t __by) {
	offset+=__by;
}

# include "../dep/mem_cpy.h"


typedef struct segment {
	struct segment *next;
	struct ffef_seg_hdr hdr;
} *segmentp;

typedef struct region {
	struct region *next;
	char const *name;
	struct ffef_reg_hdr hdr;
} *regionp;

segmentp sg = NULL;
regionp rg = NULL;

void oust(void *__p, mdl_uint_t __size) {
	lseek(d, offset, SEEK_SET);
	write(d, __p, __size);
	offset+=__size;
}

void bond(char const *__s, char const *__dst) {
	char const *p = __s;

	d = open(__dst, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);

	struct ffef_hdr dhdr;
	ffly_bzero(&dhdr, ffef_hdr_size);

	*dhdr.ident = FF_EF_MAG0;
	dhdr.ident[1] = FF_EF_MAG1;
	dhdr.ident[2] = FF_EF_MAG2;
	dhdr.ident[3] = FF_EF_MAG3;


	dhdr.nsg = 0;
	dhdr.nrg = 0;
	dhdr.nsy = 0;
	dhdr.nrl = 0;
	dhdr.sg = FF_EF_NULL;
	dhdr.rg = FF_EF_NULL;
	dhdr.sy = FF_EF_NULL;
	dhdr.rl = FF_EF_NULL;

	mdl_i8_t epdeg = -1;
	while(*p != '\0') {
		char const *fl;
		if (!(fl = extsrcfl(&p)))
			break;
		printf("source: %s\n", fl);
		if ((s = open(fl, O_RDONLY, 0)) == -1) {
			printf("failed to open source file.\n");
			break;
		}

		struct ffef_hdr hdr;
		read(s, &hdr, ffef_hdr_size);
		if (validate(&hdr) == -1) {
			printf("%s: validation failed ffef header not pressent or is corrupted, skipping.\n", fl);
			goto _no;
		}

		if (hdr.routine != FF_EF_NULL) {
			if (!epdeg) {
				printf("entry point already designated, skipping.\n");
				goto _no;
			}

			dhdr.routine = (offset-ffef_hdr_size)+hdr.routine;
		}

		segmentp seg;
		if (hdr.sg != FF_EF_NULL) {
			mdl_uint_t i = 0;
			mdl_u64_t offset = hdr.sg;
			while(i != hdr.nsg) {
				(seg = (segmentp)malloc(sizeof(struct segment)))->next = sg;
				sg = seg;
				seg->hdr.offset+=get_offset()-ffef_hdr_size;
				i++;
			}	
		}

		regionp reg;
		if (hdr.rg != FF_EF_NULL) {
			mdl_uint_t i = 0;
			mdl_u64_t offset = hdr.rg;
			while(i != hdr.nrg) {
				(reg = (regionp)malloc(sizeof(struct region)))->next = rg;
				rg = reg;

				lseek(s, offset, SEEK_SET);
				read(s, &reg->hdr, ffef_reg_hdrsz);

				reg->hdr.beg+=get_offset()-ffef_hdr_size;
				reg->hdr.end+=get_offset()-ffef_hdr_size;

				reg->name = (char const*)malloc(reg->hdr.l);
				lseek(s, reg->hdr.name, SEEK_SET);
				read(s, reg->name, reg->hdr.l);

				mdl_uint_t size;
				void *exch = malloc(size = (reg->hdr.end-reg->hdr.beg));
				lseek(s, reg->hdr.beg, SEEK_SET);
				read(s, exch, size);
				oust(exch, size);
				free(exch);
				i++;
			}
		}

		_no:
		close(s);
	}

	segmentp seg = sg;
	while(seg != NULL) {
		segmentp bk = seg;
		oust(&seg->hdr, ffef_seg_hdrsz);
		seg = seg->next;
		free(bk);
		dhdr.nsg++;
	}

	if (sg != NULL)
		dhdr.sg = offset-ffef_seg_hdrsz;
	regionp reg = rg;
	while(reg != NULL) {
		regionp bk = reg;
		reg->hdr.name = offset;
		oust(reg->name, reg->hdr.l);
		free(reg->name);
		oust(&reg->hdr, ffef_reg_hdrsz);
		reg = reg->next;
		free(bk);
		dhdr.nrg++;
	}

	if (rg != NULL)
		dhdr.rg = offset-ffef_reg_hdrsz;

	lseek(d, 0, SEEK_SET);
	write(d, &dhdr, ffef_hdr_size);
	close(d);
}
