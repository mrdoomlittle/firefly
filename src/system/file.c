# include "file.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "io.h"
# include "../dep/mem_set.h"
# include "errno.h"
# include "../dep/str_len.h"
# include "../dep/mem_dup.h"
# include "../dep/mem_cpy.h"
# include "error.h"

# define is_flag(__flags, __flag) \
	(((__flags)&(__flag))==(__flag))
# ifndef __fflib
# include <string.h>
/*
	allow for flags to be passed to write to bypass the buffer for errors
*/
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
	} else if (is_flag(__flags, FF_O_WRONLY)) {
		*(p++) = 'w';
	} else {
		*(p++) = 'r';
	}
	*p = '\0';
	return (char const*)buf;
}
# endif

void ffly_fopt(struct ffly_file *__f, ff_u8_t __flag) {
	__f->flags |= __flag;
}

ff_err_t ffly_fdrain(struct ffly_file *__f) {	
	ff_uint_t size;
	if (!(size = __f->ob_p-__f->obuf))
		return FFLY_SUCCESS;
	if (is_flag(__f->flags, FF_STREAM)) {
		if (write(__f->fd, __f->obuf, size) == -1) {

		}
	} else {
		// not tested
		if (pwrite(__f->fd, __f->obuf, size, __f->bufdst) == -1) {
			ffly_fprintfs(ffly_err, "failed to write to file, %s\n", __f->path);
			return FFLY_FAILURE;
		}
		__f->bufdst+=size;
	}
	__f->ob_p = __f->obuf;
	return FFLY_SUCCESS;
}

struct ffly_file*
ffly_fopen(char const *__path, int __flags, ff_u32_t __mode, ff_err_t *__err) {
	struct ffly_file *file = (struct ffly_file*)__ffly_mem_alloc(sizeof(struct ffly_file));
	if ((file->fd = open(__path, __flags, __mode)) == -1) {
		ffly_fprintfs(ffly_err, "file, failed to open file, error: %d, %s\n", errno, strerror(errno));
		return NULL;
	}

	if (_err(ffly_mem_dupe((void**)&file->path, __path, ffly_str_len(__path)+1))) {
		ffly_fprintfs(ffly_err, "failed to dupe file path.\n");
		*__err = FFLY_FAILURE;
		return NULL;
	}
	*__err = FFLY_SUCCESS;
# ifndef __fflib
	printf("%s\n", mode_str(__flags));
	file->libc_fp = fdopen(dup(file->fd), mode_str(__flags));
	fchmod(fileno(file->libc_fp), __mode);
# endif
	file->drain = -1;
	file->bufdst = 0;
	file->off = 0;
	file->flags = 0x0;
	file->ob_p = file->obuf;
	file->lock = FFLY_MUTEX_INIT;
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
		ffly_fprintfs(ffly_err, "file, failed to stat file, errno: %d, %s\n", errno, strerror(errno));
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
		ffly_fprintfs(ffly_err, "fseek failed.\n");
		return 0;
	}

	if (!__whence && __off != __f->off) {
		__f->off = __off;
		__f->drain = 0;
	}
	return (ff_off_t)off;
}

ff_err_t ffly_fcreat(char const *__path, ff_u32_t __mode) {
	if (access(__path, F_OK)) {
		ffly_fprintfs(ffly_err, "file at '%s' allready exists.\n");
		return FFLY_FAILURE;
	}

	if (creat(__path, __mode) < 0) {
		ffly_fprintfs(ffly_err, "file, failed to create file, errno: %d, %s\n", errno, strerror(errno));
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}

# define bufput(__f, __p, __size) \
	ffly_mem_cpy(__f->ob_p, __p, __size); \
	__f->ob_p+=__size

// solid write will skip buffer
ff_err_t ffly_fwrites(struct ffly_file *__f, void *__p, ff_uint_t __bc) {
	ffly_mutex_lock(&__f->lock);
	if (__f->ob_p>__f->obuf)
		ffly_fdrain(__f);
	if (write(__f->fd, __p, __bc) == -1) {

	}
	ffly_mutex_unlock(&__f->lock);
	return FFLY_SUCCESS;
}

ff_err_t ffly_fpwrite(struct ffly_file *__f, void *__p, ff_uint_t __bc, ff_uint_t __offset) {
	pwrite(__f->fd, __p, __bc, __offset);
}

ff_err_t ffly_fwrite(struct ffly_file *__f, void *__p, ff_uint_t __bc) {
	if (!valid_fd(__f->fd)) {
		ffly_fprintfs(ffly_err, "file descriptor not valid.\n");
		return FFLY_FAILURE;
	}

	ffly_mutex_lock(&__f->lock);
	if (is_flag(__f->flags, FF_NOBUFF)) {
		write(__f->fd, __p, __bc);		
	} else {
	if (!__f->drain) {
		ffly_fdrain(__f);
		__f->bufdst = __f->off;
		__f->drain = -1;
	}
	ffly_mutex_unlock(&__f->lock);

	ff_u8_t *src = (ff_u8_t*)__p;
	while(__bc>OBUFSZ) {
		ffly_fwrite(__f, src, OBUFSZ);
		src+=OBUFSZ;
		__bc-=OBUFSZ;
	}

	ffly_mutex_lock(&__f->lock);
	ff_uint_t left;
	ff_int_t overflow;

	if (!__bc) {
		goto _end;
	}

	left = OBUFSZ-(__f->ob_p-__f->obuf);
	if (!left) {
		ffly_fdrain(__f);
		left = OBUFSZ;
	}

	overflow = (ff_int_t)__bc-(ff_int_t)left;
	__f->off+=__bc;
	if (overflow>0) {
		bufput(__f, src, left);
		ffly_fdrain(__f);
		src+=left;
		__bc = overflow;
	}

	bufput(__f, src, __bc);
	}
_end:
	ffly_mutex_unlock(&__f->lock);
	return FFLY_SUCCESS;
}

ff_err_t ffly_fread(struct ffly_file *__f, void *__p, ff_uint_t __bc) {
	if (!valid_fd(__f->fd)) {
		ffly_fprintfs(ffly_err, "file descriptor not valid.\n");
		return FFLY_FAILURE;
	}

	if (read(__f->fd, __p, __bc) == -1) {
		ffly_fprintfs(ffly_err, "failed to read file, %s\n", __f->path);
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}

ff_err_t ffly_fclose(struct ffly_file *__f) {
	if (!valid_fd(__f->fd)) {
		ffly_fprintfs(ffly_err, "file descriptor not valid.\n");
		return FFLY_FAILURE;
	}

	ffly_fdrain(__f);

	close(__f->fd);
# ifndef __fflib
	fclose(__f->libc_fp);
# endif
	ff_err_t err;
	__ffly_mem_free((void*)__f->path);
	if (_err(err = __ffly_mem_free(__f))) {
//ffly_fprintf(ffly_err, "failed to free file.\n");
		return err;
	}
	return FFLY_SUCCESS;
}
