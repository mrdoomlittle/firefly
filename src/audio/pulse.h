# ifndef __ffly_pulse__h
# define __ffly_pulse__h
# include "../types/err_t.h"
# include <pulse/simple.h>
# include <pulse/error.h>
# include "../types/byte_t.h"
# include "../types/size_t.h"
struct ffly_pulse {
	pa_simple *simple;
	struct pa_sample_spec sample_spec;
};

# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_pulse_init(struct ffly_pulse*, char*, char*, struct pa_sample_spec*);
ffly_err_t ffly_pulse_write(struct ffly_pulse*, ffly_byte_t*, mdl_uint_t);
ffly_err_t ffly_pulse_drain(struct ffly_pulse*);
ffly_err_t ffly_pulse_free(struct ffly_pulse*);
# ifdef __cplusplus
}
# endif
# endif /*__ffly_pulse__h*/
