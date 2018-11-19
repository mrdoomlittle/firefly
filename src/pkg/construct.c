# include "../pkg.h"
# include "../dep/str_cpy.h"
# include "../linux/unistd.h"
# include "../linux/fcntl.h"
# include "../linux/stat.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../system/io.h"
# include "../system/errno.h"
# include "../linux/limits.h"
int static out;

ff_u32_t static dof = 0;
struct entry {
	void *p;
	ff_uint_t len;
	struct entry *next;
};

struct entry static *top = NULL;
ff_u32_t new(void *__p, ff_uint_t __n) {
	struct entry *ent;

	ent = (struct entry*)__ffly_mem_alloc(sizeof(struct entry));
	ent->next = top;
	top = ent;
	ent->p = __p;
	ent->len = __n;
	ff_u32_t r;

	r = dof;
	dof+=__n;
	return r;
}

ff_u32_t static ro = sizeof(struct pkc_header);
ff_u32_t static ro_(ff_uint_t __n) {
	ff_u32_t r;
	r = ro;
	ro+=__n;
	return r;
}

void ff_pkc_construct(char const *__dir, char const *__file) {
    struct pkc_plan *plan;
    plan = pkc_get_plan();

	struct pkc_header h;
	out = open(__file, O_CREAT|O_TRUNC|O_WRONLY, S_IRUSR|S_IWUSR);

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
		ig.path = new(i->path, i->plen);
		ig.plen = i->plen;
		ig.type = i->type;
	
		ffly_printf("path: %s : %u\n", i->path, i->plen);
		if (IG_ISFILE(i->type)) {
			int fd;
			fd = open(i->path, O_RDONLY, 0);
			if (fd == -1) {
				ffly_printf("error couldent open file, %s\n", strerror(errno));
			}
			struct stat st;

			fstat(fd, &st);
			if (st.st_size>0) {
				ig.ts = st.st_size;
				ig.tract = ro_(ig.ts);
				void *buf;
				buf = __ffly_mem_alloc(st.st_size);
				read(fd, buf, st.st_size);
				pwrite(out, buf, st.st_size, ig.tract);
				__ffly_mem_free(buf);
			
				ffly_printf("filesize: %u\n", st.st_size);
			}
			close(fd);
		}
		*(it+ix) = ig;
		ix++;
	}
	
	h.it = ro_(h.ic*sizeof(struct __pkc_ingot));
	h.dof = ro;
	h.dlen = dof; 
	pwrite(out, it, h.ic*sizeof(struct __pkc_ingot), h.it);

	struct entry *cur;
	cur = top;
	lseek(out, h.dof, SEEK_SET);
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

	ffly_printf("%u\n", h.ic);
	
	ff_uint_t x;
	x = 0;
	while(x != h.ic) {
		i = it+x;

		char path[PATH_MAX];
		pread(fd, path, i->plen, (h.dof+h.dlen)-(i->path+i->plen));
		*(path+i->plen) = '\0';
		ffly_printf("--> %u\n", i->plen);

		switch(i->type) {
			case IG_FILE: {
				void *buf;
				buf = __ffly_mem_alloc(i->ts);
				int fd0;
				fd0 = open(path, O_CREAT|O_TRUNC|O_WRONLY, S_IRUSR|S_IWUSR);
				pread(fd, buf, i->ts, i->tract);
				write(fd0, buf, i->ts);
				close(fd0);
				__ffly_mem_free(buf);
				break;
			}
			case IG_DIR:
				mkdir(path, S_IRWXU);
			break;
		}
	
		ffly_printf("%s\n", path);
		x++;
	}
	
	__ffly_mem_free(it);
	close(fd);
}
