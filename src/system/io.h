# ifndef __ffly__io__h
# define __ffly__io__h
# include <mdlint.h>
# include "../types/err_t.h"
# include "../types/fd_t.h"
# include "../types/size_t.h"
# include "file.h"
# ifdef __fflib
# include "printf.h"
# else
# include <stdio.h>
# endif
# include "../ffly_def.h"
# ifdef __cplusplus
extern "C" {
# endif
extern FF_FILE *ffly_out;
extern FF_FILE *ffly_in;
extern FF_FILE *ffly_log;
extern  FF_FILE *ffly_err;
ffly_err_t ffly_io_init();
void ffly_io_closeup();
# ifdef __fflib
void putchar(char);
void ppad(char, mdl_uint_t);
mdl_uint_t ffly_rdline(void*, mdl_uint_t, FF_FILE*);
# else
void ffly_fprintf(FF_FILE*, char const*, ...);
void ffly_printf(char const*, ...);
# endif
/*
ffly_size_t ffly_write(ffly_fd_t, void*, ffly_size_t, ffly_err_t*);
ffly_size_t ffly_read(ffly_fd_t, void*, ffly_size_t, ffly_err_t*);
*/
# ifdef __cplusplus
}
# endif

# ifdef __cplusplus
# define ffly_endl '\n'
namespace mdl {
namespace firefly {
namespace system {
namespace io {
static types::err_t(*fprintf)(FF_FILE*, char const*, ...) = &ffly_fprintf;
static types::err_t(*printf)(char const*, ...) = &ffly_printf;
struct _cout {
	_cout& operator<<(char const *__s) {
		printf(__s);
		return *this;
	}

	_cout& operator<<(int __v) {
		printf("%d", __v);
		return *this;
	}

	_cout& operator<<(int unsigned __v) {
		printf("%u", __v);
		return *this;
	}

	_cout& operator<<(char __c) {
		printf("%c", __c);
		return *this;
	}

	_cout& operator<<(double __v) {
		printf("%f", __v);
		return *this;
	}

	_cout& operator<<(float __v) {
		printf("%f", __v);
		return *this;
	}

};
_cout static cout;
}
}
}
}
# endif
# endif /*__ffly__io__h*/
