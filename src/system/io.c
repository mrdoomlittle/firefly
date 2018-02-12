# include "io.h"
# include <stdarg.h>
# include <mdlint.h>
# include "mutex.h"
# include "errno.h"
# include "../linux/unistd.h"
/*
# include <sys/stat.h>
# include <fcntl.h>
*/
# include "../types/size_t.h"
FF_FILE *ffly_out = NULL;
FF_FILE *ffly_log = NULL;
FF_FILE *ffly_err = NULL;
FF_FILE *ffly_in = NULL;
mdl_uint_t ffly_rdline(void *__buf, mdl_uint_t __size, FF_FILE *__file) {
    mdl_u8_t *p = (mdl_u8_t*)__buf;
    while((p-(mdl_u8_t*)__buf)<__size) {

        char c;
        if (read(__file->fd, &c, 1) <= 0) continue;
        if (c == '\n') break;
        *(p++) = c;
    } 

    *p = '\0';
}
/*
ffly_fd_t ffly_open(char const *__path, int __flags, mode_t __mode) {
	return open(__path, __flags, __mode);
}

ffly_err_t ffly_close(ffly_fd_t __fd) {
	if (close(__fd) == -1) {
		ffly_fprintf(ffly_err, "failed to close.\n");
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}

ffly_size_t ffly_write(ffly_fd_t __fd, void *__buf, ffly_size_t __size, ffly_err_t *__err) {
    ssize_t res;
    if ((res = write(__fd, __buf, __size)) == -1) {
        ffly_fprintf(ffly_err, "failed to write.\n");
        *__err = FFLY_FAILURE;
        return 0;
    }
    *__err = FFLY_SUCCESS;
    return res;
}

ffly_size_t ffly_read(ffly_fd_t __fd, void *__buf, ffly_size_t __size, ffly_err_t *__err) {
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
ffly_err_t ffly_io_init() {
    ffly_err_t err;
	if (!(ffly_out = ffly_fopen("/dev/tty", FF_O_RDWR, 0, &err))) {
		return FFLY_FAILURE;
	}
	if (!(ffly_log = ffly_fopen("log", FF_O_WRONLY|FF_O_TRUNC|FF_O_CREAT, FF_S_IRUSR|FF_S_IWUSR, &err))) {
		return FFLY_FAILURE;
	}
	if (!(ffly_err = ffly_fopen("err", FF_O_WRONLY|FF_O_TRUNC|FF_O_CREAT, FF_S_IRUSR|FF_S_IWUSR, &err))) {
		return FFLY_FAILURE;
	}

    ffly_in = ffly_out;
	return FFLY_SUCCESS;
}

void ffly_io_closeup() {
	fsync(ffly_fileno(ffly_out));
    fsync(ffly_fileno(ffly_log));
    fsync(ffly_fileno(ffly_err));

	ffly_fclose(ffly_out);
	ffly_fclose(ffly_log);
	ffly_fclose(ffly_err);
}
/*
ffly_mutex_t static mutex = FFLY_MUTEX_INIT;
void static ffly_print(FILE *__stream, char const *__s, va_list __args) {
	if (__stream == NULL || __s == NULL) return;
	ffly_mutex_lock(&mutex);
	fprintf(__stream, "firefly; ");
	fflush(__stream);

	vfprintf(__stream, __s, __args);
	fflush(__stream);
	ffly_mutex_unlock(&mutex);
}

void ffly_fprintf(FILE *__stream, char const *__s, ...) {
	va_list args;
	va_start(args, __s);
	ffly_print(__stream, __s, args);
	va_end(args);
}

void ffly_printf(char const *__s, ...) {
	va_list args;
	va_start(args, __s);
	ffly_print(ffly_out, __s, args);
	va_end(args);
}*/

