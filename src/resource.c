# include "resource.h"
# include "ffly_def.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "memory/mem_realloc.h"
# include "linux/unistd.h"
# include "linux/fcntl.h"
# include "system/error.h"
# include "system/io.h"
static ffly_resourcep *resources = NULL;
static ff_uint_t c = 0;
static ffly_resourcep dead = NULL;
# define get_resource(__id) \
	(*((resources)+*(__id)))

void ffly_resource_creat(ff_rid_t *__id) {
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
	res->id = c-1;
	res->flags = 0x0;
_sk:
	*__id = &res->id;
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

void ffly_resource_del(ff_rid_t __id) {
	ffly_resourcep res = get_resource(__id);
	if (resources+*__id == resources+c-1) {
		resources = (ffly_resourcep*)__ffly_mem_realloc(resources, (--c)*sizeof(ffly_resourcep));
		__ffly_mem_free(res);
		return;
	}
	res->fd = dead;
	dead = res;
}

# include "cache.h"
void static
cache_perish(void *__arg_p, void *__p) {
	ffly_resourcep res = (ffly_resourcep)__arg_p;
	res->flags ^= FFLY_RES_CACHED;
	__ffly_mem_free(__p);
	res->p = NULL;
}

void ffly_resource_load_bin(ff_rid_t __id, char const *__file) {
	ffly_resourcep res = get_resource(__id);
	if ((res->flags&FFLY_RES_CACHED) == FFLY_RES_CACHED) {
		ffly_fprintf(ffly_log, "cached version still living.\n");
		return;
	}

	ffly_fprintf(ffly_log, "no cached version exists, reloading.\n");
	
	char buf[1024];
	char *p = buf;
	p+=ffly_str_cpy(p, RESOURCE_DIR);
	*(p++) = '/';
	p+=ffly_str_cpy(p, __file);
	if (access(buf, F_OK) == -1) {
		ffly_fprintf(ffly_err, "file does not exist, %s\n", buf);
		return;
	}

	res->flags |= FFLY_RES_CACHED;
	int fd;
	fd = open(buf, O_RDONLY, 0);
	struct stat st;
	fstat(fd, &st);
	res->p = (ff_u8_t*)__ffly_mem_alloc(st.st_size);
	read(fd, res->p, st.st_size);
	close(fd);
	ffly_cache_put(&res->cache, res->p, 40, cache_perish, res);
}

ffly_resourcep ffly_resource_get(ff_rid_t __id) {
	return get_resource(__id);
}

void ffly_resource_cleanup(void) {
	ffly_resourcep *cur = resources;
	ffly_resourcep *end = cur+c;
	while(cur != end)
		__ffly_mem_free(*(cur++));
	if (resources != NULL)
		__ffly_mem_free(resources);
}
