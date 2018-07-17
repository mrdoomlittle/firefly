# include "io.h"
# include <stdarg.h>
# include "mutex.h"
# include "errno.h"
# ifdef __fflib
# include "../linux/unistd.h"
# else
# include <unistd.h>
# endif
# include "../linux/signal.h"
FF_FILE *ffly_out = NULL;
FF_FILE *ffly_log = NULL;
FF_FILE *ffly_err = NULL;
FF_FILE *ffly_in = NULL;
# ifdef __fflib
void putchar(char __c) {
	ffly_fwrite(ffly_out, &__c, 1);
}

/*
	shoud print in chunks and not single bytes.
*/
void ppad(char __c, ff_uint_t __n) {
	while(__n-->0) putchar(__c);
}

ff_uint_t ffly_rdline(void *__buf, ff_uint_t __size, FF_FILE *__file) {
	ff_s32_t res;
_again:
	res = read(__file->fd, __buf, __size);
	if (res<0) {
		*(ff_u8_t*)__buf = '\0';
		return 0;
	}

	*((ff_u8_t*)__buf+res-1) = '\0';
	return res-1;
}
# endif
/*
ffly_fd_t ffly_open(char const *__path, int __flags, mode_t __mode) {
	return open(__path, __flags, __mode);
}

ff_err_t ffly_close(ffly_fd_t __fd) {
	if (close(__fd) == -1) {
		ffly_fprintf(ffly_err, "failed to close.\n");
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}

ffly_size_t ffly_write(ffly_fd_t __fd, void *__buf, ffly_size_t __size, ff_err_t *__err) {
    ssize_t res;
    if ((res = write(__fd, __buf, __size)) == -1) {
        ffly_fprintf(ffly_err, "failed to write.\n");
        *__err = FFLY_FAILURE;
        return 0;
    }
    *__err = FFLY_SUCCESS;
    return res;
}

ffly_size_t ffly_read(ffly_fd_t __fd, void *__buf, ffly_size_t __size, ff_err_t *__err) {
	ssize_t res;
    if ((res = read(__fd, __buf, __size)) == -1) {
        ffly_fprintf(ffly_err, "failed to read.\n");
        *__err = FFLY_FAILURE;
        return 0;
    }
    *__err = FFLY_SUCCESS;
    return res;
}
*/
ff_err_t ffly_io_init() {
    ff_err_t err;
	if (!(ffly_out = ffly_fopen("/dev/tty", FF_O_WRONLY, 0, &err))) {
		return FFLY_FAILURE;
	}

	if (!(ffly_in = ffly_fopen("/dev/tty", FF_O_RDONLY, 0, &err))) {
		return FFLY_FAILURE;
	}

	if (!(ffly_log = ffly_fopen("log", FF_O_WRONLY|FF_O_TRUNC|FF_O_CREAT, FF_S_IRUSR|FF_S_IWUSR, &err))) {
		return FFLY_FAILURE;
	}
	if (!(ffly_err = ffly_fopen("err", FF_O_WRONLY|FF_O_TRUNC|FF_O_CREAT, FF_S_IRUSR|FF_S_IWUSR, &err))) {
		return FFLY_FAILURE;
	}

	ffly_fopt(ffly_out, FF_STREAM);
	ffly_fopt(ffly_log, FF_STREAM);
	ffly_fopt(ffly_err, FF_STREAM);
	return FFLY_SUCCESS;
}

void ffly_io_closeup() {
# ifdef __fflib
    fsync(ffly_fileno(ffly_log));
    fsync(ffly_fileno(ffly_err));
# endif
	ffly_fclose(ffly_out);
	ffly_fclose(ffly_in);
	ffly_fclose(ffly_log);
	ffly_fclose(ffly_err);
}

# ifndef __fflib
ff_mlock_t static lock = FFLY_MUTEX_INIT;
void static
ffly_print(FF_FILE *__file, char const *__format, va_list __args) {
	ffly_mutex_lock(&lock);
	vfprintf(__file->libc_fp, __format, __args);
	fflush(__file->libc_fp);
	ffly_mutex_unlock(&lock);
}

void ffly_fprintf(FF_FILE *__file, char const *__format, ...) {
	va_list args;
	va_start(args, __format);
	ffly_print(__file, __format, args);
	va_end(args);
}

void ffly_fprintfs(FF_FILE *__file, char const *__format, ...) {
	va_list args;
	va_start(args, __format);
	ffly_print(__file, __format, args);
	va_end(args);
}

void ffly_printf(char const *__format, ...) {
	va_list args;
	va_start(args, __format);
	ffly_print(ffly_out, __format, args);
	va_end(args);
}
# endif
