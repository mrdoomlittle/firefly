# ifndef __io__h
# define __io__h



# ifdef __cplusplus
extern "C" {
# endif
void ffly_printf(FILE*, char*, ...);
# ifdef __cplusplus
}
# endif

# ifdef __cplusplus
namespace mdl {
namespace firefly {
namespace system {
namespace io {
static void(*printf)(FILE*, char*, ...) = &ffly_printf;
}
}
}
}
# endif

# endif /*__io__h*/
