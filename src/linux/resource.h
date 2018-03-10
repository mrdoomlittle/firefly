# ifndef __ffly__linux__resource__h
# define __ffly__linux__resource__h
# include "types.h"
# include "time.h"
# include <mdlint.h>
struct rusage {
	struct timeval ru_utime;
	struct timeval ru_stime;
	mdl_s64_t ru_maxrss;
	mdl_s64_t ru_ixrss;
	mdl_s64_t ru_idrss;
	mdl_s64_t ru_isrss;
	mdl_s64_t ru_minflt;
	mdl_s64_t ru_majflt;
	mdl_s64_t ru_nswap;
	mdl_s64_t ru_inblock;
	mdl_s64_t ru_oublock;
	mdl_s64_t ru_msgsnd;
	mdl_s64_t ru_msgrcv;
	mdl_s64_t ru_nsignals;
	mdl_s64_t ru_nvcsw;
	mdl_s64_t ru_nivcsw;
};

# endif /*__ffly__linux__resource__h*/
