# include "config.h"
# include "../config.h"
# include "errno.h"
# include "err.h"
# include "io.h"
# include "../memory/mem_free.h"
# define DEF_MAX_THREADS 20
struct ffly_sysconf __ffly_sysconf__ = {
	.version = NULL, .root_dir = NULL
};
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
    void *version = ffly_conf_get(&cf, "version");
    if (!ffly_conf_is_str(version)) {
        ffly_fprintf(ffly_err, "can't read version as type does not match.\n");
        err = FFLY_FAILURE;
        goto _failure;
    }

	void *root_dir = ffly_conf_get(&cf, "root_dir");
	if (!ffly_conf_is_str(root_dir)) {

	}

    void *max_threads = ffly_conf_get(&cf, "max_threads");
    if (!max_threads)
        __ffly_sysconf__.max_threads = ffly_conf_int_u(max_threads);
    else
        __ffly_sysconf__.max_threads = DEF_MAX_THREADS;
    ffly_fprintf(ffly_log, "max threads: %u\n", __ffly_sysconf__.max_threads);

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

    return FFLY_SUCCESS;;
}

# include "../macros.h"
void ffly_free_sysconf() {
	__ffly_finn(__ffly_sysconf__.version);
	__ffly_finn(__ffly_sysconf__.root_dir);
}
