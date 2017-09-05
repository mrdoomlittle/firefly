# ifndef __ffly_pulse__h
# define __ffly_pulse__h
# include "../types/err_t.h"
# include <pulse/simple.h>
# include <pulse/error.h>
# include "../types/byte_t.h"
# include "../types/size_t.h"
# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_pulse_write(char*, char*, struct pa_sample_spec*, ffly_byte_t*, ffly_size_t);
# ifdef __cplusplus
}
# endif
# endif /*__ffly_pulse__h*/
