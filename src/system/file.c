# include "file.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "io.h"
# include <sys/stat.h>
# include <unistd.h>
# include "../data/mem_set.h"
# include "errno.h"
# include "../types/off_t.h"
struct ffly_file* ffly_fopen(char *__path, int __flags, mode_t __mode) {
	struct ffly_file *file = (struct ffly_file*)__ffly_mem_alloc(sizeof(struct ffly_file));
	if ((file->d = open(__path, __flags, __mode)) < 0) {
		ffly_printf(stderr, "file, failed to open file, error: %d, %s\n", errno, strerror(errno));
		return NULL;
	}
	return file;
}

void ffly_fstat(char *__path, struct ffly_stat *__stat) {
	struct stat st;
	ffly_mem_set(&st, 0, sizeof(struct stat));
	if (stat(__path, &st) < 0) {
		ffly_printf(stderr, "file, failed to stat file, errno: %d, %s\n", errno, strerror(errno));
		return;
	}

	*__stat = (struct ffly_stat) {
		.size = st.st_size
	};
}

ffly_off_t ffly_fseek(struct ffly_file *__f, ffly_off_t __off, int __whence) {
	return lseek(__f->d, __off, __whence);
}

void ffly_fcreat(char *__dir, mode_t __mode) {
	if (creat(__dir, __mode) < 0)
		ffly_printf(stderr, "file, failed to create file, errno: %d, %s\n", errno, strerror(errno));
}

void ffly_fwrite(struct ffly_file *__f, void *__p, ffly_size_t __bc) {
	write(__f->d, __p, __bc);
}


void ffly_fread(struct ffly_file *__f, void *__p, ffly_size_t __bc) {
	read(__f->d, __p, __bc);
}

void ffly_fclose(struct ffly_file *__f) {
	close(__f->d);
	__ffly_mem_free(__f);
}
