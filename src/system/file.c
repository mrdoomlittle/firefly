# include "file.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "io.h"
# include "../dep/mem_set.h"
# include "errno.h"
# include "../types/off_t.h"
# include "../dep/str_len.h"
# include "../dep/mem_dup.h"
# include "err.h"

# include "../linux/unistd.h"
# include "../linux/fcntl.h"
# include "../linux/stat.h"
/*
	TODO:
	add buffer to read and write

*/

struct ffly_file*
ffly_fopen(char const *__path, int __flags, mdl_u32_t __mode, ffly_err_t *__err) {
	struct ffly_file *file = (struct ffly_file*)__ffly_mem_alloc(sizeof(struct ffly_file));
	if ((file->fd = open(__path, __flags, __mode)) < 0) {
//		ffly_fprintf(ffly_err, "file, failed to open file, error: %d, %s\n", errno, strerror(errno));
		return NULL;
	}

	if (_err(ffly_mem_dupe((void**)&file->path, __path, ffly_str_len(__path)+1))) {
		ffly_fprintf(ffly_err, "failed to dupe file path.\n");
		*__err = FFLY_FAILURE;
		return NULL;
	}
	*__err = FFLY_SUCCESS;
	return file;
}

ffly_bool_t static valid_fd(int __fd) {
	if (fcntl(__fd, F_GETFD, 0) == -1)
		return 0;
	return 1;
}

ffly_err_t ffly_fstat(char const *__path, struct ffly_stat *__stat) {
	struct stat st;
	ffly_mem_set(&st, 0, sizeof(struct stat));
	if (stat(__path, &st) < 0) {
//		ffly_fprintf(ffly_err, "file, failed to stat file, errno: %d, %s\n", errno, strerror(errno));
		return FFLY_FAILURE;
	}

	*__stat = (struct ffly_stat) {
		.size = st.st_size
	};
	return FFLY_SUCCESS;
}

ffly_off_t ffly_fseek(struct ffly_file *__f, ffly_off_t __off, int __whence) {
	__linux_off_t off = lseek(__f->fd, __off, __whence);
	if (off == (__linux_off_t)-1) {
		ffly_fprintf(ffly_err, "fseek failed.\n");
		return 0;
	}
	return (ffly_off_t)off;
}

ffly_err_t ffly_fcreat(char const *__path, mdl_u32_t __mode) {
	if (access(__path, F_OK)) {
		ffly_fprintf(ffly_err, "file at '%s' allready exists.\n");
		return FFLY_FAILURE;
	}

	if (creat(__path, __mode) < 0) {
//		ffly_fprintf(ffly_err, "file, failed to create file, errno: %d, %s\n", errno, strerror(errno));
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}

ffly_err_t ffly_fwrite(struct ffly_file *__f, void *__p, mdl_uint_t __bc) {
	if (!valid_fd(__f->fd)) {
		ffly_fprintf(ffly_err, "file descriptor not valid.\n");
		return FFLY_FAILURE;
	}
	if (write(__f->fd, __p, __bc) == -1) {
		ffly_fprintf(ffly_err, "failed to write to file, %s\n", __f->path);
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}

ffly_err_t ffly_fread(struct ffly_file *__f, void *__p, mdl_uint_t __bc) {
	if (!valid_fd(__f->fd)) {
		ffly_fprintf(ffly_err, "file descriptor not valid.\n");
		return FFLY_FAILURE;
	}

	if (read(__f->fd, __p, __bc) == -1) {
		ffly_fprintf(ffly_err, "failed to read file, %s\n", __f->path);
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}

ffly_err_t ffly_fclose(struct ffly_file *__f) {
	if (!valid_fd(__f->fd)) {
		ffly_fprintf(ffly_err, "file descriptor not valid.\n");
		return FFLY_FAILURE;
	}

	close(__f->fd);
	ffly_err_t err;
	__ffly_mem_free((void*)__f->path);
	if (_err(err = __ffly_mem_free(__f))) {
		ffly_fprintf(ffly_err, "failed to free file.\n");
		return err;
	}
	return FFLY_SUCCESS;
}
