# include "config.h"
# include "../config.h"
# include "errno.h"
# include "err.h"
# include "io.h"
# include "../memory/mem_free.h"
# include "../mode.h"
# define DEF_MAX_THREADS 20
# define ALLOCA_SSIZE 400
struct ffly_sysconf __ffly_sysconf__ = {
	.version = NULL, .root_dir = NULL
};

void static
ld_def_max_threads(void) {
	__ffly_sysconf__.max_threads = DEF_MAX_THREADS;
}

void static
ld_def_alssize(void) {
	__ffly_sysconf__.alssize = ALLOCA_SSIZE;
}

void ffly_ld_sysconf_def() {
	ld_def_max_threads();
	ld_def_alssize();
}

void static
ld_max_threads(ffconf *__cf) {
	void const *p;
	if (!(p = ffly_conf_get(__cf, "max_threads")))
		ld_def_max_threads();
	else
		__ffly_sysconf__.max_threads = ffly_conf_int_u(p);
	ffly_fprintf(ffly_log, "max threads: %u\n", __ffly_sysconf__.max_threads);
}

void static
ld_alssize(ffconf *__cf) {
	void const *p;
	if (!(p = ffly_conf_get(__cf, "alssize")))
		ld_def_alssize();
	else
		__ffly_sysconf__.alssize = ffly_conf_int_u(p);
	ffly_fprintf(ffly_log, "alloca ssize: %u\n", __ffly_sysconf__.alssize);
}

ffly_err_t ffly_ld_sysconf(char const *__path) {
	struct ffly_conf conf;
	ffly_err_t err = FFLY_SUCCESS;
	if (_err(err = ffly_conf_init(&conf))) {
		ffly_fprintf(ffly_err, "failed to init config.\n");
		return err;
	}

	if (_err(err = ffly_conf_ld(&conf, __path))) {
		ffly_fprintf(ffly_err, "failed to load config.\n");
		return err;
	}

	if (_err(err = ffly_conf_read(&conf))) {
		ffly_fprintf(ffly_err, "failed to read config.\n");
		return err;
	}

	ffconf cf;
	ffly_conf_depos(&conf, &cf);
	void const *version = ffly_conf_get(&cf, "version");
	if (!ffly_conf_is_str(version)) {
		ffly_fprintf(ffly_err, "can't read version as type does not match.\n");
		err = FFLY_FAILURE;
		goto _failure;
	}

	void const *root_dir = ffly_conf_get(&cf, "root_dir");
	if (!ffly_conf_is_str(root_dir)) {

	}
	
	ld_max_threads(&cf);
	ld_alssize(&cf);

	__ffly_sysconf__.version = (char const*)ffly_str_dupe(ffly_conf_str(version));
	__ffly_sysconf__.root_dir = (char const*)ffly_str_dupe(ffly_conf_str(root_dir));

	_failure:
	if (_err(err = ffly_conf_free(&conf))) {
		ffly_fprintf(ffly_err, "failed to free config:0.\n");
		return err;
	}

	if (_err(err = ffconf_free(&cf))) {
		ffly_fprintf(ffly_err, "failed to free config:1.\n");
		return err;
	}

	return FFLY_SUCCESS;
}

# include "../macros.h"
void ffly_free_sysconf() {
	__ffly_finn(__ffly_sysconf__.version);
	__ffly_finn(__ffly_sysconf__.root_dir);
}
