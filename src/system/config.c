# include "config.h"
# include "../config.h"
# include "errno.h"
# include "err.h"
# include "io.h"
struct ffly_sysconf __ffly_sysconf__;
ffly_err_t ffly_ld_sysconf(char *__path) {
    struct ffly_conf conf;
    ffly_err_t err = FFLY_SUCCESS;
    if (_err(err = ffly_conf_init(&conf))) {
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
    void* version = ffly_conf_get(&cf, "version");
    if (!ffly_conf_is_str(version)) {
        ffly_fprintf(ffly_err, "can't read version as type does not match.\n");
        err = FFLY_FAILURE;
        goto _failure;
    }

    __ffly_sysconf__.version = (char const*)ffly_str_dupe(ffly_conf_str(version));

    _failure:
    if (_err(err = ffly_conf_free(&conf))) {
        return err;
    }

    if (_err(err = ffconf_free(&cf))) {
        return err;
    }
    return err;
}
