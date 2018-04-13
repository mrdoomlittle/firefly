# include "file.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "io.h"
# include "../dep/mem_set.h"
# include "errno.h"
# include "../dep/str_len.h"
# include "../dep/mem_dup.h"
# include "err.h"

/*
	TODO:
	add buffer to read and write

*/

# ifndef __fflib
# include <string.h>
typedef ff_s32_t __linux_off_t;
# define is_flag(__flags, __flag) \
	(((__flags)&(__flag))==(__flag))
char const static* mode_str(ff_u32_t __flags) {
	char static buf[24];
	char *p = buf; 
	if (is_flag(__flags, FF_O_RDWR)) {
		*(p++) = 'r';
		*(p++) = '+';
	} else if (is_flag(__flags, FF_O_WRONLY) && is_flag(__flags, FF_O_CREAT) && is_flag(__flags, FF_O_TRUNC))
		*(p++) = 'w';
	else if (is_flag(__flags, FF_O_RDWR) && is_flag(__flags, FF_O_CREAT) && is_flag(__flags, FF_O_TRUNC)) {
		*(p++) = 'w';
		*(p++) = '+';
	} else {
		*(p++) = 'r';
	}
	*p = '\0';
	return (char const*)buf;
}
# endif

struct ffly_file*
ffly_fopen(char const *__path, int __flags, ff_u32_t __mode, ff_err_t *__err) {
	struct ffly_file *file = (struct ffly_file*)__ffly_mem_alloc(sizeof(struct ffly_file));
	if ((file->fd = open(__path, __flags, __mode)) == -1) {
		ffly_fprintf(ffly_err, "file, failed to open file, error: %d, %s\n", errno, strerror(errno));
		return NULL;
	}

	if (_err(ffly_mem_dupe((void**)&file->path, __path, ffly_str_len(__path)+1))) {
		ffly_fprintf(ffly_err, "failed to dupe file path.\n");
		*__err = FFLY_FAILURE;
		return NULL;
	}
	*__err = FFLY_SUCCESS;
# ifndef __fflib
	printf("%s\n", mode_str(__flags));
	file->libc_fp = fdopen(dup(file->fd), mode_str(__flags));
	fchmod(fileno(file->libc_fp), __mode);
# endif
	return file;
}

ff_bool_t static valid_fd(int __fd) {
	if (fcntl(__fd, F_GETFD, 0) == -1)
		return 0;
	return 1;
}

ff_err_t ffly_fstat(char const *__path, struct ffly_stat *__stat) {
	struct stat st;
	ffly_mem_set(&st, 0, sizeof(struct stat));
	if (stat(__path, &st) == -1) {
		ffly_fprintf(ffly_err, "file, failed to stat file, errno: %d, %s\n", errno, strerror(errno));
		return FFLY_FAILURE;
	}

	*__stat = (struct ffly_stat) {
		.size = st.st_size
	};
	return FFLY_SUCCESS;
}

ff_off_t ffly_fseek(struct ffly_file *__f, ff_off_t __off, int __whence) {
	__linux_off_t off = lseek(__f->fd, __off, __whence);
	if (off == (__linux_off_t)-1) {
		ffly_fprintf(ffly_err, "fseek failed.\n");
		return 0;
	}
	return (ff_off_t)off;
}

ff_err_t ffly_fcreat(char const *__path, ff_u32_t __mode) {
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

ff_err_t ffly_fwrite(struct ffly_file *__f, void *__p, ff_uint_t __bc) {
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

ff_err_t ffly_fread(struct ffly_file *__f, void *__p, ff_uint_t __bc) {
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

ff_err_t ffly_fclose(struct ffly_file *__f) {
	if (!valid_fd(__f->fd)) {
		ffly_fprintf(ffly_err, "file descriptor not valid.\n");
		return FFLY_FAILURE;
	}

	close(__f->fd);
# ifndef __fflib
	fclose(__f->libc_fp);
# endif
	ff_err_t err;
	__ffly_mem_free((void*)__f->path);
	if (_err(err = __ffly_mem_free(__f))) {
		ffly_fprintf(ffly_err, "failed to free file.\n");
		return err;
	}
	return FFLY_SUCCESS;
}
