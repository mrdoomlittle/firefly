# include "ss.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "memory/mem_realloc.h"
# include "types.h"
ffly_ss_dirp __ffly_ssrt__;
ffly_ss_dirp __ffly_sscur__;

static struct ffly_dict dict;

struct chip {
	struct chip *link;
	void *p;
};

# define N 200
static struct chip chips[N];
static struct chip *fresh = chips;
static struct chip *fc = NULL;

void static
chip_set(ff_u64_t __chip, void *__p) {
	(chips+__chip)->p = __p;
}

void static*
chip_get(ff_u64_t __chip) {
	return (chips+__chip)->p;
}

ff_u64_t static chip_new(void) {
	struct chip *c;
	if (fc != NULL) {
		c = fc;
		fc = fc->link;
	} else {
		c = fresh++;
	}

	return c-chips;
}

void static chip_free(ff_u64_t __chip) {
	struct chip *c;
	c = chips+__chip;

	if (c == fresh-1) {
		fresh--;
	} else {
		c->link = fc;
		fc = c;
	}
}

void ffly_ss_prime(void) {
	__ffly_ssrt__ =  (ffly_ss_dirp)__ffly_mem_alloc(sizeof(struct ffly_ss_dir));
	__ffly_sscur__ = __ffly_ssrt__;
	ffly_dict_init(&dict);

	__ffly_ssrt__->files = NULL;
	__ffly_ssrt__->bk = NULL;

}

void ffly_ss_fin(void) {
	void *p = ffly_dict_head(&dict);
	while(p != NULL) {
		__ffly_mem_free((void*)ffly_dict_getp(p));	
		ffly_dict_fd(&p);
	}

	ffly_dict_de_init(&dict);
	ffly_ss_filep cur, f;
	cur = __ffly_ssrt__->files;
	while(cur != NULL) {
		f = cur;
		cur = cur->fd;
		if (f->sort == _ffly_ss_dir)
			ffly_ss_dir_del(f->chip);
		else
			ffly_ss_file_del(f->chip);
	}
	__ffly_mem_free(__ffly_ssrt__);
}

ff_u64_t ffly_ss_file_creat(void *__p, ff_u8_t __sort, ff_u8_t __flags) {
	ffly_ss_dirp dir;
	ffly_ss_filep f;
	f = (ffly_ss_filep)__ffly_mem_alloc(sizeof(struct ffly_ss_file));
	dir = __ffly_sscur__;
	
	ff_u64_t c;
	c = chip_new();
	chip_set(c, f);
	f->flags = __flags;
	f->sort = __sort;
	f->p = __p;

	f->bk = &dir->files;
	f->fd = dir->files;
	if (dir->files != NULL) {
		dir->files->bk = &f->fd;
	}

	dir->files = f;
	f->chip = c;
	return c;
}


ff_u64_t* ffly_ss_alias(char const *__name) {
	ff_u64_t *c;
	c = (ff_u64_t*)__ffly_mem_alloc(sizeof(ff_u64_t));
	ffly_dict_put(&dict, __name, c);
	return c;
}

ff_u64_t* ffly_ss_lookup(char const *__name) {
	ff_err_t err;
	return (ff_u64_t*)ffly_dict_get(&dict, __name, &err);
}

void ffly_ss_file_del(ff_u64_t __file) {
	ffly_ss_filep f;
	f = (ffly_ss_filep)chip_get(__file);
	chip_free(__file);

	if ((f->flags&SS_TOFREE)>0) {
		__ffly_mem_free(f->p);
	}	

	*f->bk = f->fd;
	if (f->fd != NULL) {
		f->fd->bk = f->bk;
	}

	__ffly_mem_free(f);
}

ff_u64_t ffly_ss_dir_creat(void) {
	ffly_ss_dirp dir;

	dir = (ffly_ss_dirp)__ffly_mem_alloc(sizeof(struct ffly_ss_dir));

	ff_u64_t c;
	c = ffly_ss_file_creat(dir, _ffly_ss_dir, SS_TOFREE);	
	dir->files = NULL;
	dir->bk = __ffly_sscur__;

	dir->chip = c;
	return c;
}

# include "stdio.h"
char pad[240];
char *pp = pad;
void ffly_ss_tree(ffly_ss_dirp __root) {
	*pp = '\0';
	ffly_ss_dirp dir;
	ffly_ss_filep cur;

	dir = __root;
	cur = dir->files;
	while(cur != NULL) {
		
		printf("%sfile: %u\n", pad, cur->chip);
		if (cur->sort == _ffly_ss_dir) {
			*(pp++) = '-';
			ffly_ss_tree((ffly_ss_dirp)cur->p);
			*(--pp) = '\0';
		}
		cur = cur->fd;
	}
}

void ffly_ss_tun(ff_u64_t __dir) {
	__ffly_sscur__ = (ffly_ss_dirp)((ffly_ss_filep)chip_get(__dir))->p;
}

void ffly_ss_dir_del(ff_u64_t __dir) {
	ffly_ss_dirp dir;
	ffly_ss_filep cur, f;

	dir = (ffly_ss_dirp)((ffly_ss_filep)chip_get(__dir))->p;
	cur = dir->files;
	while(cur != NULL) {
		f = cur;
		cur = cur->fd;
		if (f->sort == _ffly_ss_dir) {
			ffly_ss_dir_del(f->chip);
		} else
			ffly_ss_file_del(f->chip);
	}

	ffly_ss_file_del(__dir);
}

void ffly_ss_bk(void) {
	__ffly_sscur__ = __ffly_sscur__->bk;
}

/*
ff_err_t ffmain(int __argc, char const *__argv) {
	ffly_ss_prime();
	ss_dir_creat("help");
	ss_dir_creat("unknown");

	ss_tun("help");
	ss_file_creat("wtf", NULL, _ffly_ss_def, 0);
	ss_dir_creat("help2");
	ss_tun("help2");
	ss_file_creat("wtf2", NULL, _ffly_ss_def, 0);
	ss_bk;
	ss_bk;
	ss_dir_del("help");

	ffly_ss_tree(__ffly_ssrt__);
	ffly_ss_fin();
}*/
