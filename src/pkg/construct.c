# include "../pkg.h"
# include "../dep/str_cpy.h"
# include "../dep/str_len.h"
# include "../dep/mem_cpy.h"
# include "../linux/unistd.h"
# include "../linux/fcntl.h"
# include "../linux/stat.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../system/io.h"
# include "../system/errno.h"
# include "../linux/limits.h"
int static out;

ff_u32_t static grof = 0;
struct entry {
	void *p;
	ff_uint_t len;
	struct entry *next;
};

ff_u8_t static *grain;

/*
	all data needed for deconstruction e.g. paths to file/dir/...
*/
struct entry static *top = NULL;
ff_u32_t grain_alloc(void *__p, ff_uint_t __n) {
	struct entry *ent;

	ent = (struct entry*)__ffly_mem_alloc(sizeof(struct entry));
	ent->next = top;
	top = ent;
	ent->p = __p;
	ent->len = __n;
	ff_u32_t r;

	r = grof;
	grof+=__n;
	return r;
}

ff_u32_t static f_off = sizeof(struct pkc_header);

/*
	rename
*/
ff_u32_t static cleave(ff_uint_t __n) {
	ff_u32_t r;
	r = f_off;
	f_off+=__n;
	return r;
}

void ff_pkc_construct(char const *__plan, char const *__dst) {
    struct pkc_plan *plan;
	pkc_plan = __plan;
	pkc_pfl = ffly_str_len(__plan);

    plan = pkc_get_plan();

	struct pkc_header h;
	out = open(__dst, O_CREAT|O_TRUNC|O_WRONLY, S_IRUSR|S_IWUSR);

	struct pkc_ingot *i, **ii;
	struct pkc_ingot **e;

	h.ic = plan->n;
	struct __pkc_ingot *it;
	it = (struct __pkc_ingot*)__ffly_mem_alloc(h.ic*sizeof(struct __pkc_ingot));

	ff_uint_t ix = 0;
	ii = plan->i;
	e = ii+plan->n;
	while(ii != e) {
		i = *(ii++);
		
		struct __pkc_ingot ig;
		ig.path = grain_alloc(i->path, i->plen);
		ig.plen = i->plen;
		ig.type = i->type;
	
		struct stat st;
		ffly_printf("path: %s : %u\n", i->path, i->plen);
		if (IG_ISFILE(i->type)) {
			int fd;
			fd = open(i->path, O_RDONLY, 0);
			if (fd == -1) {
				ffly_printf("error couldent open file, %s\n", strerror(errno));
			}
		
			fstat(fd, &st);
			if (st.st_size>0) {
				ig.ts = st.st_size;
				ig.tract = cleave(ig.ts);
				void *buf;
				buf = __ffly_mem_alloc(st.st_size);
				read(fd, buf, st.st_size);
				pwrite(out, buf, st.st_size, ig.tract);
				__ffly_mem_free(buf);
			
				ffly_printf("filesize: %u\n", st.st_size);
			}
			close(fd);
		} else {
			stat(i->path, &st);
		}
		ig.mode = st.st_mode&00777;
		*(it+ix) = ig;
		ix++;
	}
	
	h.it = cleave(h.ic*sizeof(struct __pkc_ingot));
	h.grain = cleave(grof);
	h.glen = grof; 
	pwrite(out, it, h.ic*sizeof(struct __pkc_ingot), h.it);

	struct entry *cur;
	cur = top;
	lseek(out, h.grain, SEEK_SET);
	while(cur != NULL) {
		write(out, cur->p, cur->len);
		cur = cur->next;
	}
	pwrite(out, &h, sizeof(struct pkc_header), 0);
	close(out);
}

void ff_pkc_deconstruct(char const *__dir, char const *__file) {
	int fd;
	fd = open(__file, O_RDONLY, 0);
	struct pkc_header h;
	read(fd, &h, sizeof(struct pkc_header));

	struct __pkc_ingot *it, *i;
	it = (struct __pkc_ingot*)__ffly_mem_alloc(h.ic*sizeof(struct __pkc_ingot));
	pread(fd, it, h.ic*sizeof(struct __pkc_ingot), h.it);

	grain = (ff_u8_t*)__ffly_mem_alloc(h.glen);
	pread(fd, grain, h.glen, h.grain);

	ffly_printf("%u\n", h.ic);
	
	char path[PATH_MAX];
	char *pp;
	ff_uint_t s;
	pp = path+(s = ffly_str_len(__dir));
	ffly_mem_cpy(path, __dir, s);
	*(pp++) = '/';
	ff_uint_t x;
	x = 0;
	while(x != h.ic) {
		i = it+x;

		ffly_mem_cpy(pp, (grain+h.glen)-(i->path+i->plen), i->plen);
		*(pp+i->plen) = '\0';
		ffly_printf("--> %u\n", i->plen);

		switch(i->type) {
			case IG_FILE: {
				void *buf;
				buf = __ffly_mem_alloc(i->ts);
				int fd0;
				fd0 = open(path, O_CREAT|O_TRUNC|O_WRONLY, i->mode);
				pread(fd, buf, i->ts, i->tract);
				write(fd0, buf, i->ts);
				close(fd0);
				__ffly_mem_free(buf);
				break;
			}
			case IG_DIR:
				mkdir(path, i->mode);
			break;
		}

		ffly_printf("%s\n", path);
		x++;
	}

	__ffly_mem_free(grain);
	__ffly_mem_free(it);
	close(fd);
}
