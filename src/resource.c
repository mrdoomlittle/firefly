# include "resource.h"
# include "ffly_def.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "memory/mem_realloc.h"
# include "linux/unistd.h"
# include "linux/fcntl.h"
# include "system/error.h"
# include "system/io.h"
# include "storage/cistern.h"
# include "dep/mem_cpy.h"
# include "system/sched.h"
# include "clock.h"
# include "system/mutex.h"
# define LOAF_SHIFT 4
# define LOAF_SIZE (1<<LOAF_SHIFT)
# define LOAF_AGE 1100
static ffly_resourcep *resources = NULL;
static ff_uint_t c = 0;
static ffly_resourcep dead = NULL;
# define get_resource(__id) \
	(*(resources+(__id)))

static ffly_rs_loafp cache = NULL;

static struct ffly_cistern c0, c1, c2, c3;
static ff_mlock_t lock = FFLY_MUTEX_INIT;

static ffly_cisternp ct[] = {
	&c0,
	&c1,
	&c2,
	&c3
};

ff_u8_t static
cistern(void) {
	ff_u8_t r;
	r = 0;
	if (c1.off<ct[r]->off)
		r = 1;
	if (c2.off<ct[r]->off)
		r = 2;
	if (c3.off<ct[r]->off)
		r = 3;
	return r;
}

void ffly_rscis_init(void) {
	ffly_cistern_init(&c0, "rs.cis0");
	ffly_cistern_init(&c1, "rs.cis1");
	ffly_cistern_init(&c2, "rs.cis2");
	ffly_cistern_init(&c3, "rs.cis3");
}

void ffly_rscis_de_init(void) {
	ffly_cistern_de_init(&c0);
	ffly_cistern_de_init(&c1);
	ffly_cistern_de_init(&c2);
	ffly_cistern_de_init(&c3);
}

void ffly_resource_creat(ff_rs_t *__id) {
	ffly_resourcep res;
	if (dead != NULL) {
		res = dead;
		dead = dead->fd;
		goto _sk;
	}

	if (!resources) {
		resources = (ffly_resourcep*)__ffly_mem_alloc(sizeof(ffly_resourcep));
		c++;
	} else
		resources = (ffly_resourcep*)__ffly_mem_realloc(resources, (++c)*sizeof(ffly_resourcep));
	res = (*(resources+c-1) = (ffly_resourcep)__ffly_mem_alloc(sizeof(struct ffly_resource)));
	res->flags = 0x00;
_sk:
	*__id = c-1;
}

# include "copy.h"
# include "dep/str_cpy.h"
ff_err_t ffly_resource_move(char const *__file) {
	char buf[1024];
	char *p = buf;
	p+=ffly_str_cpy(p, RESOURCE_DIR);
	*(p++) = '/';
	p+=ffly_str_cpy(p, __file);

	if (!access(buf, F_OK)) {
		ffly_fprintf(ffly_log, "resource file already here.\n");
		retok;
	}

	if (mkdir(RESOURCE_DIR, S_IRWXU|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH) == -1) {
		struct stat st;
		if (stat(RESOURCE_DIR, &st) == -1) {
			ffly_fprintf(ffly_err, "somthing has gone wrong.\n");
			goto _fail;
		}
		if (!S_ISDIR(st.st_mode)) {
			ffly_fprintf(ffly_err, "current path being used.\n");
			goto _fail;
		}
	}

	// copy to resource directory
	ff_fcopy(__file, buf);
	retok;
_fail:
	reterr;
}

void ffly_resource_save(char const *__dir) {
}

void ffly_resource_del(ff_rs_t __id) {
	ffly_resourcep res = get_resource(__id);
	if (resources+__id == resources+c-1) {
		resources = (ffly_resourcep*)__ffly_mem_realloc(resources, (--c)*sizeof(ffly_resourcep));
		__ffly_mem_free(res);
		return;
	}
	res->fd = dead;
	dead = res;
}

ff_i8_t static
cache_perish(ffly_rs_loafp __loaf) {
	ff_u8_t res;

	res = 0;
	__asm__("lock btsw $0, (%1)\n"
		"setcb %0\n"
	: "=m"(res) : "r"(&__loaf->lock));

	if (!res) {
		__ffly_mem_free(__loaf->p);
		__loaf->flags &= ~FFLY_RES_CACHED;
		__loaf->lock = 0;
		ffly_printf("loaf was uncached.\n");
	} else {
		return -1;
	}
	return 0;
}

ff_u32_t static sched_id;

void static delink(ffly_rs_loafp __loaf) {
	*__loaf->bk = __loaf->link;
	if (__loaf->link != NULL)
		__loaf->link->bk = __loaf->bk;
}

ff_i8_t static
update(void *__arg_p) {
	ffly_mutex_lock(&lock);
	ffly_rs_loafp cur, loaf;
	cur = cache;
	while(cur != NULL) {
		loaf = cur;
		cur = cur->link;
		ffly_printf("%u, creation: %u, death: %u\n", clock, loaf->creation, loaf->death);
		if (clock>=loaf->creation+loaf->death) {
			if (cache_perish(loaf) == -1) {
				continue;
			}
			delink(loaf);
		}
	}
	ffly_mutex_unlock(&lock);
}

void ffly_resource_sched(void) {
	ffly_schedule(update, NULL, 8);
}

void ffly_resource_usched(void) {
	ffly_sched_rm(sched_id);
}

void static loaf(ffly_resourcep __r, ff_uint_t __size) {
	ff_uint_t lc;

	lc = (__size+(0xffffffffffffffff>>(64-LOAF_SHIFT)))>>LOAF_SHIFT;
	__r->loafs = (ffly_rs_loafp)__ffly_mem_alloc(lc*sizeof(struct ffly_rs_loaf));	

	ffly_rs_loafp loaf, end;

	loaf = __r->loafs;
	end = loaf+lc;
	ff_uint_t off;

	off = 0;
	while(loaf != end) {
		loaf->flags = 0x00;
		loaf->lock = 0;
		loaf->off = off++;
		loaf++;
	}
}

/*
	NOTE:
		all resource files should be copyed to its own directory
		to avoid issues if some idiot deletes it
*/

# define CHUNK_SHIFT 4
# define CHUNK_SIZE (1<<CHUNK_SHIFT)
void static
load_file(ffly_resourcep __r, void **__args) {
	char file[1024];
	char *p;
	p = file;

	p+=ffly_str_cpy(p, RESOURCE_DIR);
	*(p++) = '/';
	p+=ffly_str_cpy(p, (char const*)__args[0]);

	if (access(file, F_OK) == -1) {
		ffly_fprintf(ffly_err, "file does not exist, %s\n", file);
		return;
	}

	ffly_printf("file: %s\n", file);
	int fd;
	if ((fd = open(file, O_RDONLY, 0)) == -1) {

	}

	struct stat st;
	fstat(fd, &st);

	loaf(__r, st.st_size);

	ff_uint_t cc;
	cc = st.st_size>>CHUNK_SHIFT;

	ffly_cisternp c;
	void *r;
	r = (__r->r = ffly_cistern_alloc(c = ct[__r->c = cistern()], st.st_size));
	ffly_printf("using cistern %u, %p\n", __r->c, r);

	ff_u32_t off;
	ff_uint_t ch;
	ch = 0;
	off = 0;

	void *buf;

	buf = __ffly_mem_alloc(CHUNK_SIZE);
	while(ch != cc) {
		read(fd, buf, CHUNK_SIZE);
		ffly_cistern_write(c, r, buf, off, CHUNK_SIZE);
		off+=CHUNK_SIZE;
		ch++;
	}

	ff_uint_t left;
	left = st.st_size-off;
	if (left>0) {
		read(fd, buf, left);
		ffly_cistern_write(c, r, buf, off, left);
	}

	__ffly_mem_free(buf);
	close(fd);
}

static void(*load[])(ffly_resourcep, void**) = {
	load_file
};

void ffly_resource_load(ff_rs_t __id, ff_uint_t __loader, void **__args) {
	ffly_resourcep r;
	
	r = get_resource(__id);
	load[__loader](r, __args);
}

void static
loaf_read(ffly_resourcep __r, ffly_rs_loafp __loaf, void *__buf, ff_uint_t __offset, ff_uint_t __size) {
	ff_u8_t res;
_again:
	res = 0;
	__asm__("lock btsw $0, (%1)\n"
		"setcb %0\n"
	: "=m"(res) : "r"(&__loaf->lock));

	if (res) {
		goto _again;
	}

	__loaf->creation = clock;
	if ((__loaf->flags&FFLY_RES_CACHED)>0) {
		ffly_mem_cpy(__buf, ((ff_u8_t*)__loaf->p)+__offset, __size);
	} else {
		__loaf->p = __ffly_mem_alloc(LOAF_SIZE);
		ffly_cistern_read(ct[__r->c], __r->r, __loaf->p, __loaf->off*LOAF_SIZE, LOAF_SIZE);
		ffly_mem_cpy(__buf, ((ff_u8_t*)__loaf->p)+__offset, __size);
		__loaf->flags |= FFLY_RES_CACHED;

		ffly_printf("resource loaf being cached for later use.\n");
		ffly_mutex_lock(&lock);
		__loaf->bk = &cache;
		__loaf->link = cache;
		__loaf->death = LOAF_AGE;
		if (cache != NULL)
			cache->bk = &__loaf->link;
		cache = __loaf;
		ffly_mutex_unlock(&lock);
	}
	__loaf->lock = 0;
}

void ffly_resource_read(ff_rs_t __id, void *__buf, ff_u32_t __offset, ff_uint_t __size) {
	ffly_resourcep r;

	r = get_resource(__id);
	ffly_rs_loafp loaf, end;
	
	ff_u8_t *p;
	p = (ff_u8_t*)__buf;
	loaf = r->loafs+(__offset>>LOAF_SHIFT);
	end = loaf+(__size>>LOAF_SHIFT);
	ff_u32_t offset;

	offset = __offset-(loaf->off*LOAF_SIZE);
	if (offset>0 && __size>=LOAF_SIZE) {
		ff_uint_t sz;
		loaf_read(r, loaf, p, offset, sz = (LOAF_SIZE-offset));
		p+=sz;
		loaf++;
		offset = 0;
	}

	while(loaf<end) {
		loaf_read(r, loaf, p, 0, LOAF_SIZE);
		p+=LOAF_SIZE;
		loaf++;
	}

	ff_uint_t left;
	left = (__size-(p-(ff_u8_t*)__buf));
	if (left>0)
		loaf_read(r, loaf, p, offset, left);
}

ffly_resourcep ffly_resource_get(ff_rs_t __id) {
	return get_resource(__id);
}

void ffly_resource_cleanup(void) {
	ffly_resourcep *cur = resources;
	ffly_resourcep *end = cur+c;
	ffly_resourcep r;
	while(cur != end) {
		r = *(cur++);
		ffly_cistern_free(ct[r->c], r->r);
		__ffly_mem_free(r);
	}
	if (resources != NULL)
		__ffly_mem_free(resources);
}
/*
# include "system/nanosleep.h"
ff_err_t ffmain(int __argc, char const *__argv[]) {
	ffly_scheduler_init(SCHED_CORRODE);
	ffly_resource_sched();
	ff_rs_t res;
	ffly_resource_creat(&res);

	void *args[] = {
		"test.res"
	};
	ffly_rscis_init();
	ffly_resource_load(res, _rs_file, args);

	ff_uint_t i;
	i = 0;

	char buf[256];

	while(i++ != 22) {
		ffly_resource_read(res, buf, 0, 256);
//		*(buf+255) = '\0';
//		ffly_printf("%s\n", buf);
		ffly_nanosleep(0, 100000000);
	}

	ffly_resource_usched();
	ffly_rscis_de_init();
	ffly_resource_cleanup();
}*/
